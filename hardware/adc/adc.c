#include "adc.h"

/* Initialize CADC */

volatile uint16_t g_Adc16ConversionValue;
volatile uint16_t g_Adc16InterruptCounter;
volatile uint16_t g_Adc16InterruptZeroFlag;
volatile uint16_t g_Adc16InterruptHighFlag;


//__IO uint16_t  PWM_Duty	; //占空比
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
 volatile bool g_Transfer_Done = false; /* DMA transfer completion flag. */
uint16_t g_cadcSampleDataArray[DEMO_CADC_SAMPLE_COUNT];
uint32_t g_avgADCValue = 0U; /* Average ADC value .*/
edma_handle_t g_EDMA_Handle; /* Edma handler. */
edma_transfer_config_t g_transferConfig;
/*!
 * @brief Initialize the EDMA.
 */
static void EDMA_Configuration(void);

/*!
 * @brief Initialize the DMAMUX.
 */
static void DMAMUX_Configuration(void);

/*!
 * @brief Initialize the CADC.
 */
static void CADC_Configuration(void);

/*!
 * @brief Process ADC values.
 */
static void ProcessSampleData(void);

/*!
 * @brief Callback function for EDMA.
 */
static void Edma_Callback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds);

//static void ADC_Channel_Select(void);


/*******************************************************************************
 * 
 *Function Name: ADC_DMA_Init(void)
 *
 *
 *
 ******************************************************************************/

void ADC_DMA_Init(void)
{

    EDMA_Configuration();   /* Initialize EDMA. */
    DMAMUX_Configuration(); /* Initialize DMAMUX. */
    CADC_Configuration();   /* Initialize CADC. */

    /* Trigger the converter.
     * Trigger converter A would execute the loop sequential conversion when in
     * "kCADC_DualConverterWorkAsLoopSequential" work mode. */
    CADC_DoSoftwareTriggerConverter(DEMO_CADC_BASEADDR, kCADC_ConverterA);

   // PRINTF("Press any key to get user channel's ADC value ...\r\n");



}

/*******************************************************************************
 * 
 *Function Name: ADC_Channel_Select() 
 *
 *
 *
 ******************************************************************************/

uint16_t ADC_DMA_ReadValue(void)
{

       uint16_t adc_value;
       g_Transfer_Done = false;
        while (!g_Transfer_Done)
        {
        }
        ProcessSampleData();
       // PRINTF("ADC value: %d\r\n", g_avgADCValue >> ADC_RSLT_RSLT_SHIFT);
        PRINTF("ADC value: %d\r\n", ((g_avgADCValue >> ADC_RSLT_RSLT_SHIFT)*100)/4090);
       adc_value =(uint16_t) ((g_avgADCValue >> ADC_RSLT_RSLT_SHIFT)*100)/4090;
       return adc_value ;
       


}
/*********************************************************************************************************************/
static void DMAMUX_Configuration(void)
{
    /* Configure DMAMUX */
    DMAMUX_Init(DEMO_DMAMUX_BASEADDR);
    DMAMUX_SetSource(DEMO_DMAMUX_BASEADDR, DEMO_DMA_CHANNEL, DEMO_DMA_ADC_SOURCE); /* Map ADC source to channel 0 */
    DMAMUX_EnableChannel(DEMO_DMAMUX_BASEADDR, DEMO_DMA_CHANNEL);
}

static void EDMA_Configuration(void)
{
    edma_config_t userConfig;

    EDMA_GetDefaultConfig(&userConfig);
    EDMA_Init(DEMO_DMA_BASEADDR, &userConfig);
    EDMA_CreateHandle(&g_EDMA_Handle, DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL);
    EDMA_SetCallback(&g_EDMA_Handle, Edma_Callback, NULL);
    EDMA_PrepareTransfer(&g_transferConfig, (void *)CADC_RESULT_REG_ADDR, sizeof(uint16_t),
                         (void *)g_cadcSampleDataArray, sizeof(uint16_t), sizeof(uint16_t),
                         sizeof(g_cadcSampleDataArray), kEDMA_PeripheralToMemory);
    EDMA_SubmitTransfer(&g_EDMA_Handle, &g_transferConfig);
    /* Enable interrupt when transfer is done. */
    EDMA_EnableChannelInterrupts(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, kEDMA_MajorInterruptEnable);
#if defined(FSL_FEATURE_EDMA_ASYNCHRO_REQUEST_CHANNEL_COUNT) && FSL_FEATURE_EDMA_ASYNCHRO_REQUEST_CHANNEL_COUNT
    /* Enable async DMA request. */
    EDMA_EnableAsyncRequest(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, true);
#endif /* FSL_FEATURE_EDMA_ASYNCHRO_REQUEST_CHANNEL_COUNT */
    /* Enable transfer. */
    EDMA_StartTransfer(&g_EDMA_Handle);
}

static void CADC_Configuration(void)
{
    cadc_config_t               cadcConfigStruct;
    cadc_converter_config_t     cadcConverterConfigStruct;
    cadc_sample_config_t        cadcSampleConfigStruct;
    uint16_t sampleMask;

    /*
     * cadcConfigStruct.dualConverterScanMode = kCADC_DualConverterWorkAsTriggeredParallel;
     * cadcConfigStruct.enableSimultaneousMode = true;
     * cadcConfigStruct.DMATriggerSoruce = kCADC_DMATriggerSourceAsEndOfScan;
     * cadcConfigStruct.idleWorkMode = kCADC_IdleKeepNormal;
     * cadcConfigStruct.powerUpDelay = 26U;
     */
    CADC_GetDefaultConfig(&cadcConfigStruct);
    cadcConfigStruct.dualConverterScanMode = kCADC_DualConverterWorkAsLoopSequential;
  
    cadcConverterConfigStruct.lowReferenceVoltageSource=kCADC_ReferenceVoltageVrefPad; //WT.EDIT 
    cadcConverterConfigStruct.speedMode=kCADC_SpeedMode3; //WT.EDIT
      
      CADC_Init(DEMO_CADC_BASEADDR, &cadcConfigStruct);

    /* Configure converterA. */
    CADC_GetDefaultConverterConfig(&cadcConverterConfigStruct);

    CADC_SetConverterConfig(DEMO_CADC_BASEADDR, kCADC_ConverterA, &cadcConverterConfigStruct);
    /* Enable the power for converterA. */
    CADC_EnableConverterPower(DEMO_CADC_BASEADDR, kCADC_ConverterA, true);
    while (kCADC_ConverterAPowerDownFlag == (kCADC_ConverterAPowerDownFlag & CADC_GetStatusFlags(ADC)))
    {
    }
    /* Open the gate for converterA. */
    CADC_EnableConverter(DEMO_CADC_BASEADDR, kCADC_ConverterA, true);

    /* Configure the samples. */
    cadcSampleConfigStruct.channelGain      = kCADC_ChannelGainx1;
     cadcConverterConfigStruct.speedMode=  kCADC_SpeedMode3;
    cadcSampleConfigStruct.zeroCrossingMode = kCADC_ZeroCorssingDisabled;
    cadcSampleConfigStruct.highLimitValue   = 0x3E80;//0xFFFFU; //WT.EDIT
    cadcSampleConfigStruct.lowLimitValue    = 0x0U;
    cadcSampleConfigStruct.offsetValue      = 0x0U;
    cadcSampleConfigStruct.enableWaitSync   = false;

    /* For converter A. */
    cadcSampleConfigStruct.channelNumber          = DEMO_CADC_USER_CHANNEL;
    cadcSampleConfigStruct.enableDifferentialPair = false;
    CADC_SetSampleConfig(DEMO_CADC_BASEADDR, 0U, &cadcSampleConfigStruct);

    /* Enable the sample slot. */
    sampleMask = CADC_SAMPLE_MASK(0U); /* For converter A. */
    CADC_EnableSample(DEMO_CADC_BASEADDR, sampleMask, true);
    CADC_EnableSample(DEMO_CADC_BASEADDR, (uint16_t)(~sampleMask), false); /* Disable other sample slot. */

    /* Enable the DMA for converterA. */
    CADC_EnableConverterDMA(DEMO_CADC_BASEADDR, kCADC_ConverterA, true);
}

static void ProcessSampleData(void)
{
    uint32_t i = 0U;

    g_avgADCValue = 0;
    /* Get average adc value. */
    for (i = 0; i < DEMO_CADC_SAMPLE_COUNT; i++)
    {
        g_avgADCValue += g_cadcSampleDataArray[i];
    }
    g_avgADCValue = g_avgADCValue / DEMO_CADC_SAMPLE_COUNT;

    /* Reset old value. */
    for (i = 0; i < DEMO_CADC_SAMPLE_COUNT; i++)
    {
        g_cadcSampleDataArray[i] = 0U;
    }
}

static void Edma_Callback(edma_handle_t *handle, void *userData, bool transferDone, uint32_t tcds)
{
    /* Clear Edma interrupt flag. */
    EDMA_ClearChannelStatusFlags(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, kEDMA_InterruptFlag);
    /* Setup transfer */
    EDMA_PrepareTransfer(&g_transferConfig, (void *)CADC_RESULT_REG_ADDR, sizeof(uint16_t),
                         (void *)g_cadcSampleDataArray, sizeof(uint16_t), sizeof(uint16_t),
                         sizeof(g_cadcSampleDataArray), kEDMA_PeripheralToMemory);
    EDMA_SetTransferConfig(DEMO_DMA_BASEADDR, DEMO_DMA_CHANNEL, &g_transferConfig, NULL);
    /* Enable transfer. */
    EDMA_StartTransfer(&g_EDMA_Handle);
    g_Transfer_Done = true;
}

/*******************************************************************************
 * 
 *Function Name: ADC_CADC_Init() 
 *
 *
 *
 ******************************************************************************/
void ADC_CADC_Init(void)
{
    
    cadc_config_t cadcConfigStruct;
    cadc_converter_config_t cadcConverterConfigStruct;
    cadc_sample_config_t cadcSampleConfigStruct;
    uint16_t sampleMask;

     /* Initialize the CADC common digital control.
     * "kCADC_DualConverterWorkAsTriggeredSequential" is used in this case. The a long conversion sequence would be
     * executed by both converter. The sequence would be controlled by converter A's control logic, including the
     * trigger input.
     */
    /*
     * cadcConfigStruct.dualConverterScanMode = kCADC_DualConverterWorkAsTriggeredParallel;
     * cadcConfigStruct.enableSimultaneousMode = true;
     * cadcConfigStruct.DMATriggerSoruce = kCADC_DMATriggerSourceAsEndOfScan;
     * cadcConfigStruct.idleWorkMode = kCADC_IdleKeepNormal;
     * cadcConfigStruct.powerUpDelay = 26U;
     */
    CADC_GetDefaultConfig(&cadcConfigStruct);
    cadcConfigStruct.dualConverterScanMode = kCADC_DualConverterWorkAsLoopSequential;//WT.EDIT kCADC_DualConverterWorkAsTriggeredSequential;

     cadcConverterConfigStruct.lowReferenceVoltageSource=kCADC_ReferenceVoltageVrefPad; //WT.EDIT 
    cadcConverterConfigStruct.speedMode=kCADC_SpeedMode3; //WT.EDIT
    CADC_Init(CADC_BASEADDR, &cadcConfigStruct);

    /* Configure each converter. */
    CADC_GetDefaultConverterConfig(&cadcConverterConfigStruct);
    CADC_SetConverterConfig(CADC_BASEADDR, kCADC_ConverterA | kCADC_ConverterB, &cadcConverterConfigStruct);
    /* Enable the power for each converter. */
    CADC_EnableConverterPower(CADC_BASEADDR, kCADC_ConverterA | kCADC_ConverterB, true);
    while ((kCADC_ConverterAPowerDownFlag | kCADC_ConverterBPowerDownFlag) ==
           ((kCADC_ConverterAPowerDownFlag | kCADC_ConverterBPowerDownFlag) & CADC_GetStatusFlags(ADC)))
    {
    }
    /* Open the gate for each converter. */
    CADC_EnableConverter(CADC_BASEADDR, kCADC_ConverterA | kCADC_ConverterB, true);

    /* Configure the samples. */
    cadcSampleConfigStruct.channelGain      = kCADC_ChannelGainx1;
     cadcConverterConfigStruct.speedMode=  kCADC_SpeedMode3;
    cadcSampleConfigStruct.zeroCrossingMode = kCADC_ZeroCorssingForAnySignChanged;//WT.EDIT 20191116//kCADC_ZeroCorssingDisabled;
    cadcSampleConfigStruct.highLimitValue   = 0xFDE8;//3.0V //0xFFFFU;  //高电平极限设置(中断)
    cadcSampleConfigStruct.lowLimitValue    = 0x0U;     //低电平极限设置(中断)
    cadcSampleConfigStruct.offsetValue      = 0x64U;
    cadcSampleConfigStruct.enableWaitSync   = false;

    /* For converter A ADCA_CHC7 12PIN. */
    cadcSampleConfigStruct.channelNumber          = CADC_CHANNEL1_NUMBER; //ADCA_CHC7 --A 
    cadcSampleConfigStruct.enableDifferentialPair = CADC_CHANNEL1_ENABLE_DIFF;
    CADC_SetSampleConfig(CADC_BASEADDR, 0U, &cadcSampleConfigStruct);

    /* ADCA_CHC6 11PIN*/
    cadcSampleConfigStruct.channelNumber          = CADC_CHANNEL2_NUMBER;  //ADCA_CHC6 ---B
    cadcSampleConfigStruct.enableDifferentialPair = CADC_CHANNEL2_ENABLE_DIFF;
    CADC_SetSampleConfig(CADC_BASEADDR, 1U, &cadcSampleConfigStruct);
    
    /*ADCA_CH3 10PIN*/
    cadcSampleConfigStruct.channelNumber          = CADC_CHANNEL3_NUMBER; //ADCA_CH3  ---C
    cadcSampleConfigStruct.enableDifferentialPair = CADC_CHANNEL3_ENABLE_DIFF;
    CADC_SetSampleConfig(DEMO_CADC_BASEADDR, 2U, &cadcSampleConfigStruct);
    
    /*ADCA_CH2 9PIN*/
    cadcSampleConfigStruct.channelNumber          = CADC_CHANNEL4_NUMBER;
    cadcSampleConfigStruct.enableDifferentialPair = CADC_CHANNEL4_ENABLE_DIFF;
    CADC_SetSampleConfig(DEMO_CADC_BASEADDR, 3U, &cadcSampleConfigStruct);

    /* Enable the sample slot. */
    sampleMask = CADC_SAMPLE_MASK(0U)    /* For converter A. */
                 | CADC_SAMPLE_MASK(1U)  /* For converter A. */
                 | CADC_SAMPLE_MASK(2U)  /* For converter A. */
                 | CADC_SAMPLE_MASK(3U);  /* For converter A. */
    CADC_EnableSample(CADC_BASEADDR, sampleMask, true);
    CADC_EnableSample(CADC_BASEADDR, (uint16_t)(~sampleMask), false); /* Disable other sample slot. */
    
    /* The available sample slots are sample 0, 1, 2, 3. */

   // PRINTF("Press any key to trigger the conversion ...\r\n");
   // PRINTF("\r\nSample 0\tSample 1\tSample 2\tSample 3\r\n");


}

/*******************************************************************************
 * 
 *Function Name: void CADC_Read_ADC_Value(void)
 *
 *
 *
 ******************************************************************************/
uint16_t CADC_Read_ADC_Value(void)
{

         
    uint16_t pwm_duty;

        /* Enable the sample slot. */
    pwm_duty =  CADC_SAMPLE_MASK(0U)    /* For converter A. */
                | CADC_SAMPLE_MASK(1U)  /* For converter A. */
                 | CADC_SAMPLE_MASK(2U)  /* For converter A. */
                  |CADC_SAMPLE_MASK(3U);  /* For converter A. */
    CADC_EnableSample(CADC_BASEADDR, pwm_duty, true);
    CADC_EnableSample(CADC_BASEADDR, (uint16_t)(~pwm_duty), false); /* Disable other sample slot. */
 
       CADC_DoSoftwareTriggerConverter(CADC_BASEADDR, kCADC_ConverterA);

	  /* Wait the conversion to be done. */
       while (kCADC_ConverterAEndOfScanFlag !=
               (kCADC_ConverterAEndOfScanFlag & CADC_GetStatusFlags(CADC_BASEADDR)))
        {
        }
       /* Read the result value. */
       // if (pwm_duty == (pwm_duty & CADC_GetSampleReadyStatusFlags(CADC_BASEADDR)))
       // {

         //   PRINTF("PWM_Duty2 = %d\r\n",(uint16_t)((CADC_GetSampleResultValue(CADC_BASEADDR, 0U))/ 330));
          //  PRINTF("PWM_Duty3 = %d\r\n",(uint16_t)((CADC_GetSampleResultValue(CADC_BASEADDR, 1U))/ 330));
           
            
      //  }
        pwm_duty = (uint16_t)((CADC_GetSampleResultValue(CADC_BASEADDR, 3U))/ 330);//ADCA_CH2-9PIN
	    if(pwm_duty > 98)
			pwm_duty = 100;
        
        CADC_ClearStatusFlags(CADC_BASEADDR, kCADC_ConverterAEndOfScanFlag);//kCADC_ConverterAInProgressFlag
      //  CADC_ClearStatusFlags(CADC_BASEADDR, kCADC_ConverterAInProgressFlag);
       
   		return pwm_duty;
     
 }
/*******************************************************************************
 * 
 *Function Name: void ADC_UVW_Sample_HALL_Value(void)
 *Function : no hall element ,sample of Motor of phase_U,V,W.
 *
 *
 ******************************************************************************/
void ADC_UVW_Sample_HALL_Value(void)
{
        uint8_t i=0;
        uint16_t u[10],v[10],w[10];
        uint16_t sampleMask ;

             /* Enable the sample slot. */
    sampleMask = CADC_SAMPLE_MASK(0U)    /* For converter A.  */  
                 | CADC_SAMPLE_MASK(1U)  /* For converter A. */
                 | CADC_SAMPLE_MASK(2U)  /* For converter A. */
                 | CADC_SAMPLE_MASK(3U);  /* For converter A. */
    CADC_EnableSample(CADC_BASEADDR, sampleMask, true);
    CADC_EnableSample(CADC_BASEADDR, (uint16_t)(~sampleMask), false); /* Disable other sample slot. */
  
       CADC_DoSoftwareTriggerConverter(CADC_BASEADDR, kCADC_ConverterA);
       

        /* Wait the conversion to be done. */
        while (kCADC_ConverterAEndOfScanFlag !=
               (kCADC_ConverterAEndOfScanFlag & CADC_GetStatusFlags(DEMO_CADC_BASEADDR)))
        {
        }
    
        /* Read the result value. */
       if (sampleMask == (sampleMask & CADC_GetSampleReadyStatusFlags(DEMO_CADC_BASEADDR))) // flags =1 准备读取样板
        {
        
           u[i] = (int16_t)CADC_GetSampleResultValue(DEMO_CADC_BASEADDR, 0U); //ADCA_CHC7 --A
           v[i] = (int16_t)CADC_GetSampleResultValue(DEMO_CADC_BASEADDR, 1U); //ADCA_CHC6  -B 
           w[i] = (int16_t)CADC_GetSampleResultValue(DEMO_CADC_BASEADDR, 2U); //ADCA_CH3  - C

            g_Adc16InterruptZeroFlag=CADC_GetSampleZeroCrossingStatusFlags(DEMO_CADC_BASEADDR);
            
        
            g_Adc16InterruptHighFlag=CADC_GetSampleHighLimitStatusFlags(DEMO_CADC_BASEADDR);
            
             
            // g_Adc16IneterruptLowFlag=CADC_GetSampleLowLimitStatusFlags(DEMO_CADC_BASEADDR);
        }
          uSaHall = u[i];
          vSaHall = v[i];
          wSaHall = w[i];
          i++;
         if(i==10) i=0;
         
         #if 0
         #ifdef DEBUG_PRINT  
            PRINTF("u= %d\r\n", uSaHall);
            PRINTF("v= %d\r\n", vSaHall);
            PRINTF("w= %d\r\n", wSaHall);
            
            PRINTF("h= %d\r\n", g_Adc16InterruptZeroFlag);
            PRINTF("ze= %d\r\n",  g_Adc16InterruptHighFlag);
         #endif
         #endif  
        CADC_ClearSampleHighLimitStatusFlags(DEMO_CADC_BASEADDR, 0U);
        CADC_ClearSampleHighLimitStatusFlags(DEMO_CADC_BASEADDR, 1U);
        CADC_ClearSampleHighLimitStatusFlags(DEMO_CADC_BASEADDR, 2U);

        CADC_ClearSampleZeroCrossingStatusFlags(DEMO_CADC_BASEADDR, 0U);
        CADC_ClearSampleZeroCrossingStatusFlags(DEMO_CADC_BASEADDR, 1U);
        CADC_ClearSampleZeroCrossingStatusFlags(DEMO_CADC_BASEADDR, 2U);
        #if 0
          if(   g_Adc16InterruptZeroFlag > 0xf )
            {
                 PRINTF("11111111111\r\n");
                 PRINTF("\r\n");
                  EnableIRQ(ADC_LIMIT_IRQn);;
                 //CADC_EnableInterrupts(DEMO_CADC_BASEADDR, kCADC_HighLimitInterruptEnable);
                 CADC_EnableInterrupts(DEMO_CADC_BASEADDR, kCADC_ZeroCrossingInterruptEnable);
                 
            }
          else 
          {
            // CADC_DisableInterrupts(DEMO_CADC_BASEADDR, kCADC_LowLimitInterruptEnable);
             CADC_DisableInterrupts(DEMO_CADC_BASEADDR, kCADC_ZeroCrossingInterruptEnable);
             DisableIRQ(ADC_LIMIT_IRQn);
          }
        #endif 
        #if 0
          if(g_Adc16InterruptHighFlag > 0xe)
          {
            
            PRINTF("2222222222\r\n");
                 PRINTF("\r\n");
            EnableIRQ(ADC_LIMIT_IRQn);
            CADC_EnableInterrupts(DEMO_CADC_BASEADDR, kCADC_HighLimitInterruptEnable);
          }
          else
          {
             DisableIRQ(ADC_LIMIT_IRQn);
             CADC_DisableInterrupts(DEMO_CADC_BASEADDR, kCADC_HighLimitInterruptEnable);
          }
       #endif 
       
       
       CADC_ClearStatusFlags(DEMO_CADC_BASEADDR, kCADC_ConverterAEndOfScanFlag);
      // EnableIRQ(ADC_LIMIT_IRQn);
     //  CADC_EnableInterrupts(DEMO_CADC_BASEADDR, kCADC_HighLimitInterruptEnable);
            

}
