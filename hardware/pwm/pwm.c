#include "pwm.h"

pwm_config_t pwmConfig;

static void PWM_DRV_Init3PhPwm(void);


BLDC_Typedef BLDCMotor = {0,CW,0,0,100,0,0,0};
__IO int32_t uwStep = 0;      //六步引脚状态
__IO int32_t Dir = CCW;       // 方向
__IO uint32_t Lock_Time = 0;  // 堵转电流





/**********************************************************
 *
 *Function Name:HALL_Init(void)
 *
 *
 *
***********************************************************/
void HALL_Init(void)
{
   GPIO_QuickInit(HW_GPIOE, 16, kGPIO_Mode_IPU);
   GPIO_QuickInit(HW_GPIOC, 6, kGPIO_Mode_IPU);
   GPIO_QuickInit(HW_GPIOB, 3, kGPIO_Mode_IPU);


}
/*********************************************************
 *
 *Funtion Name: HallSensor_GetPinState
 *
 *
 *
*********************************************************/
uint32_t HallSensor_GetPinState(void)
{
   __IO static uint32_t State ;
  State  = 0;
  if(GPIO_PinRead(GPIOE, 16)!= GPIO_PIN_RESET)//????′??D?÷×′ì???è?
  {
    State |= 0x01U;
  }
  if(GPIO_PinRead(GPIOC,6) != GPIO_PIN_RESET)  //????′??D?÷×′ì???è?
  {
    State |= 0x02U;
  }
  if(GPIO_PinRead(GPIOB,3) != GPIO_PIN_RESET)  //????′??D?÷×′ì???è?
  {
    State |= 0x04U;
  }
  return State;



}
/******************************************************************
 *
 *Function Name: void PWM_BLDC_Init()
 *
 *
 *
******************************************************************/
void PWM_BLDC_Init(void)
{
    
    pwm_config_t pwmConfig;
    XBARA_Init(XBARA);
    XBARA_SetSignalsConnection(XBARA, kXBARA_InputVdd, kXBARA_OutputPwmAFault0);
    XBARA_SetSignalsConnection(XBARA, kXBARA_InputVdd, kXBARA_OutputPwmAFault1);
    XBARA_SetSignalsConnection(XBARA, kXBARA_InputVdd, kXBARA_OutputPwmAFault2);
    XBARA_SetSignalsConnection(XBARA, kXBARA_InputVdd, kXBARA_OutputPwmAFault3);

    PRINTF("FlexPWM driver example\n");

    /*
     * pwmConfig.enableDebugMode = false;
     * pwmConfig.enableWait = false;
     * pwmConfig.reloadSelect = kPWM_LocalReload;
     * pwmConfig.faultFilterCount = 0;
     * pwmConfig.faultFilterPeriod = 0;
     * pwmConfig.clockSource = kPWM_BusClock;
     * pwmConfig.prescale = kPWM_Prescale_Divide_1;
     * pwmConfig.initializationControl = kPWM_Initialize_LocalSync;
     * pwmConfig.forceTrigger = kPWM_Force_Local;
     * pwmConfig.reloadFrequency = kPWM_LoadEveryOportunity;
     * pwmConfig.reloadLogic = kPWM_ReloadImmediate;
     * pwmConfig.pairOperation = kPWM_Independent;
     */
    PWM_GetDefaultConfig(&pwmConfig);
    PRINTF("FlexPWM driver example 2\n");
	pwmConfig.enableDebugMode = true;  //WT.EDIT 2019-07-13
    /* Use full cycle reload */
    pwmConfig.reloadLogic =kPWM_ReloadPwmHalfCycle; //WT.EDIT 2019-07-13 //kPWM_ReloadPwmFullCycle;
    /* PWM A & PWM B form a complementary PWM pair */
    pwmConfig.pairOperation   = kPWM_Independent; //WT.EDIT kPWM_ComplementaryPwmA;
    pwmConfig.enableDebugMode = true;
    pwmConfig.forceTrigger =  kPWM_Force_LocalReload;//kPWM_Force_Local;  //WT.EDIT
    

    /* Initialize submodule 0 */
    if (PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module_0, &pwmConfig) == kStatus_Fail)
    {
        PRINTF("PWM initialization failed\n");
       // return 1;
    }

   /* Initialize submodule 1 */
    //pwmConfig.clockSource           = kPWM_Submodule0Clock;
    pwmConfig.clockSource           =  kPWM_BusClock; //WT.EDIT 2019-06-29
    //pwmConfig.initializationControl = kPWM_Initialize_MasterSync;
    pwmConfig.initializationControl = kPWM_Initialize_ExtSync;//kPWM_Initialize_LocalSync;   //WT.EDIT 2019-06-29
    if (PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module_1, &pwmConfig) == kStatus_Fail)
    {
        PRINTF("PWM initialization failed\n");
      //  return 1;
    }

    /* Initialize submodule 2 the same way as submodule 1 */
    if (PWM_Init(BOARD_PWM_BASEADDR, kPWM_Module_2, &pwmConfig) == kStatus_Fail)
    {
        PRINTF("PWM initialization failed\n");
      //  return 1;
    }
    
    PRINTF("PWM init b3\n");
    /* Call the init function with demo configuration */
    PWM_DRV_Init3PhPwm();
    PRINTF("PWM init 3\n");
    
    /* Set the load okay bit for all submodules to load registers from their buffer */
    //PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 | kPWM_Control_Module_1 | kPWM_Control_Module_2, true);

    /* Start the PWM generation from Submodules 0, 1 and 2 */
   // PWM_StartTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 | kPWM_Control_Module_1 | kPWM_Control_Module_2);
    
    PRINTF("PWMA init end \n");
}
/**************************************************************
 *
 *Function Name: PWM_DRV_Init3PhPwm(void)
 *Function Active :be used to kPWM_Control_Module_0 
 *
 *
 *
**************************************************************/


static void PWM_DRV_Init3PhPwm(void)
{
    

    uint16_t deadTimeVal;
    pwm_signal_param_t pwmSignal[2];
    uint32_t pwmSourceClockInHz;
    uint32_t pwmFrequencyInHz = 3000; //5KHZ PWM of frequency  1khz ~ 4.5khz 1.5khz =1.5khz 2khz = 2khz 3kh=3kz


    pwmSourceClockInHz = PWM_SRC_CLK_FREQ;

    /* Set deadtime count, we set this to about 650ns */
    deadTimeVal = ((uint64_t)pwmSourceClockInHz * 650) / 1000000000;

    pwmSignal[0].pwmChannel       = kPWM_PwmA;
    pwmSignal[0].level            = kPWM_HighTrue;
    pwmSignal[0].dutyCyclePercent = 80; /* 1 percent dutycycle */
    pwmSignal[0].deadtimeValue    = deadTimeVal;

    pwmSignal[1].pwmChannel = kPWM_PwmB;
    pwmSignal[1].level      = kPWM_HighTrue;
    /* Dutycycle field of PWM B does not matter as we are running in PWM A complementary mode */
    pwmSignal[1].dutyCyclePercent = 80;
    pwmSignal[1].deadtimeValue    = deadTimeVal;

    PWM_SetupSwCtrlOut(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmA|kPWM_PwmB, false);
    PWM_SetupSwCtrlOut(BOARD_PWM_BASEADDR, kPWM_Module_1, kPWM_PwmA|kPWM_PwmB, false);
    PWM_SetupSwCtrlOut(BOARD_PWM_BASEADDR, kPWM_Module_2, kPWM_PwmA|kPWM_PwmB, false);

    /*********** PWMA_SM0 - phase A, configuration, setup 2 channel as an example ************/
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_0, pwmSignal, 2, kPWM_EdgeAligned , pwmFrequencyInHz,
                 pwmSourceClockInHz);
	

    /*********** PWMA_SM1 - phase B configuration, setup PWM A channel only ************/
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_1, pwmSignal, 2, kPWM_EdgeAligned , pwmFrequencyInHz,
                 pwmSourceClockInHz);


    /*********** PWMA_SM2 - phase C configuration, setup PWM A channel only ************/
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_2, pwmSignal, 2, kPWM_EdgeAligned , pwmFrequencyInHz,
                 pwmSourceClockInHz);

}



/*********************************************************
 *
 *Funtion Name: HallSensor_Detected_BLDC
 *Function Active: HALL sensor detected signal 
 *Input Reference :No
 *
 *
*********************************************************/
void HALLSensor_Detected_BLDC(uint32_t uvw)
{
        //C+ A- 
       /*  PWM_A0 B 0 stop  */ 
     //  PWMA_Close_ABC_Channel(1);  //close B channel 
      
      /*  PWM_A1 and PWM_B1 output */
     //  PWMA_Select_C_Channel(0);    //open C upper half-bridge
      /*  PWM_A2 and PWM_B2 output  */
    //  PWMA_Select_A_Channel(1);   

  #if 0
	/**************************PWM_A0****************************************/
    PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmA, kPWM_SignedCenterAligned, 0); 
	PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_0, true);
   
	PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmB, kPWM_SignedCenterAligned, 0); 
	PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_0, true);


   /**************************PWM_A1****************************************/
   PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_1, kPWM_PwmA, kPWM_SignedCenterAligned, 0); 
   PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_1, true);

   PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_1, kPWM_PwmB, kPWM_SignedCenterAligned, 50); 
   PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_1, true);
  

  /**************************PWM_A2****************************************/
  PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_2, kPWM_PwmB, kPWM_SignedCenterAligned, 0); 
   PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_2, true); 


  PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_2, kPWM_PwmA, kPWM_SignedCenterAligned, 50); 
   PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_2, true);

   
 #endif 

  /***********************************************************************************/
#if 1
/* hall  */
  //BLDCMotor.uwStep = HallSensor_GetPinState();
   __IO uint32_t tmp = 0;
  //uwStep = HallSensor_GetPinState();
  uwStep = HallSensor_GetPinState();
  
//  uwStep = uvw;
  if(Dir == CW)
  {
    uwStep = (uint32_t)7 - uwStep;        // ?ù?Y?3Dò±íμ?1??é CW = 7 - CCW;
  }
  else 
    uwStep = uwStep-(uint32_t)7 ; 
  

  
  
  /*---- six step changed phase */
  /*---- 1(001,U),IC2(010,V),IC3(100,W) ----*/
 // PRINTF("uwStep = %d\n",uwStep);
 #if 1
  switch(uvw)//switch(BLDCMotor.uwStep)
 {
    case 1://C+ A-
       /*  PWM_A0 B 0 stop  */ 
       PWMA_Close_ABC_Channel(1);  //close B channel 
      
      /*  PWM_A1 and PWM_B1 output */
       PWMA_Select_C_Channel(0);    //open C upper half-bridge
      /*  PWM_A2 and PWM_B2 output  */
      PWMA_Select_A_Channel(1);    
      PRINTF("uwStep = %d\n",uwStep);
      break;
    
    case 2: //A+  B-
      /*  Channe3 configuration */ 
        PWMA_Close_ABC_Channel(2); //close C channel 
   
    
      /*  Channel configuration A+ */
       PWMA_Select_A_Channel(0);
      
      /*  Channe2 configuration B- */
       PWMA_Select_B_Channel(1);
      PRINTF("uwStep = %d\n",uwStep);
      break;
    
    case 3:// C+ B-
      /*  Channel configuration */ 
         PWMA_Close_ABC_Channel(0); //close A channel 
     
      /*  Channe3 configuration  */
         PWMA_Select_C_Channel(0);
     /*  Channe2 configuration  */
        PWMA_Select_B_Channel(1);
   PRINTF("uwStep = %d\n",uwStep);
      break;
    
    case 4:// B+ C-
      /*  Channel configuration */ 
          PWMA_Close_ABC_Channel(0); //close A channel 
      
      /*  Channe2 configuration */
           PWMA_Select_B_Channel(0);
      
      /*  Channe3 configuration */
          PWMA_Select_C_Channel(1);
         PRINTF("uwStep = %d\n",uwStep);
      break;
    
    case 5: // B+ A-

      /*  Channe3 configuration */       
        PWMA_Close_ABC_Channel(2); //close C channel 
       //  DelayMs(500U);
      /*  Channe2 configuration */
         PWMA_Select_B_Channel(0);
      
      /*  Channel configuration */
         PWMA_Select_A_Channel(1);
        PRINTF("uwStep = %d\n",uwStep);
      break;
    
    case 6: // A+ C-

         /*  Channe2 configuration */ 
         PWMA_Close_ABC_Channel(1); //close B channel 
       //DelayMs(500U);
        /*  Channel configuration */
         PWMA_Select_A_Channel(0);
      
      /*  Channe3 configuration */
       PWMA_Select_C_Channel(1);
	//   DelayMs(500U);
      PRINTF("uwStep = %d\n",uwStep);
 
      break;
  }
 
 #endif   
  /* á￠?ì′￥·￠???à */
 // HAL_TIM_GenerateEvent(&htimx_BLDC, TIM_EVENTSOURCE_COM);
 // __HAL_TIM_CLEAR_IT(htim, TIM_FLAG_COM);
//  BLDCMotor.Lock_Time = 0;
 #endif 
 }





/**************************************************************
 *
 *Function Name: PWMA_A_Selection 
 *Function Active :first cycle of up -bridge for A channel
 *
 *
 *
**************************************************************/
void PWMA_Select_A_Channel(uint8_t s_pwma)
{

    
 if(s_pwma == 0) //A = PWMA_A0 B0 ,selected PWMA_A0 turn off PWMA_B0
 {
 
   PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmB,   kPWM_EdgeAligned, 0); 
   //PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_1, true);

    
   PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmA,   kPWM_EdgeAligned, 50); 
   PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_0, true);
   PWM_StartTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 );
    
 }
 else if (s_pwma == 1) //selected PWM_B0 turn off PWMA_A1 next bridge half cycle
 {
      /*PWMA A1 channel PWM output*/
	 PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmA,   kPWM_EdgeAligned, 0); 
     //PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_1, true);

     PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmB,   kPWM_EdgeAligned, 50); 
     PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_0, true);
	 PWM_StartTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 );
 }

  
}
/**************************************************************
 *
 *Function Name: PWMA_B_Selection 
 *Function Active :second  cycle of up -bridge for B channel
 *Input Reference : s_pwmb is Chanel B = PWMA_A1/B1
 *
 *
**************************************************************/

void PWMA_Select_B_Channel(uint8_t s_pwmb)
{
  
 if (s_pwmb == 0) //B = PWMA_A1/B1 = selected PWM_B1 turn off PWMA_A1
 {
      /*PWMA A1 channel PWM output*/
     PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_1, kPWM_PwmB,   kPWM_EdgeAligned, 0); 
    // PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_1, true);

     PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_1, kPWM_PwmA,   kPWM_EdgeAligned, 50); 
     PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_1, true);
     PWM_StartTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_1 );
 }
 else if(s_pwmb == 1) //selected PWMA_B2  trun off PWMA_A2 the second half cycle bridge
 {
   PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_1, kPWM_PwmA,   kPWM_EdgeAligned, 0); 
  // PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_1, true);

   PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_1, kPWM_PwmB,   kPWM_EdgeAligned, 50); 
   PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_1, true);
   PWM_StartTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_1 );
 }


}
/**************************************************************
 *
 *Function Name: PWMA_C_Selection 
 *Function Active :second  cycle of up -bridge for C channel
 *Input Reference :channel C =PWMA_A2 B2 ,
 *Return Reference: No
 *
**************************************************************/

void PWMA_Select_C_Channel(uint8_t s_pwmc)
{
  /*PWMA_C = PWMA_ A2/B2*/ 
 if(s_pwmc == 0) //selected PWMA_B2  trun off PWMA_A2
 {
   PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_2, kPWM_PwmB,   kPWM_EdgeAligned, 0); 
  

   PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_2, kPWM_PwmA,   kPWM_EdgeAligned, 50); 
   PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_2, true);
   PWM_StartTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_2 );
 }
 else if(s_pwmc == 1)
 {
   PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_2, kPWM_PwmA,   kPWM_EdgeAligned, 0); 
  

   PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_2, kPWM_PwmB,   kPWM_EdgeAligned, 50); 
   PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_2, true);
   PWM_StartTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_2 );

 }
}

/**************************************************************
 *
 *Function Name: PWMABC_Selection 
 *Function Active :be used to kPWM_Control_Module_0 
 *
 *
 *
**************************************************************/
void PWMA_Close_ABC_Channel(uint8_t f_pwmabC)
{
  if(f_pwmabC == 0) //turen off PWMA_A0 and B0
 {

    //close PWMA_A0/B0 channel
    PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmA,   kPWM_EdgeAligned, 0); 
	PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_0, true);
   
	PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmB,   kPWM_EdgeAligned, 0); 
	PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_0, true);
	//PWM_StopTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_0);
	 PWM_StartTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 );

 }
 else if (f_pwmabC == 1) //turn off PWMA_A1,B1, 
 {
   PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_1, kPWM_PwmA,   kPWM_EdgeAligned, 0); 
   PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_1, true);

   PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_1, kPWM_PwmB,   kPWM_EdgeAligned, 0); 
   PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_1, true);
   //PWM_StopTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_1);
   PWM_StartTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_1 );
 }
 else if(f_pwmabC == 2) //turn off PWMA_A2 ,B2 channel open PWMA_A2 channel 
 {
   PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_2, kPWM_PwmA,   kPWM_EdgeAligned, 0); 
   PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_1, true);

   PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_2, kPWM_PwmB,   kPWM_EdgeAligned, 0); 
   PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_2, true);
   //PWM_StopTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_2);
   PWM_StartTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_2 );
 }
    
}


