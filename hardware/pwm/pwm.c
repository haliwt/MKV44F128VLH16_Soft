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
    /* Use full cycle reload */
    pwmConfig.reloadLogic = kPWM_ReloadPwmFullCycle;
    /* PWM A & PWM B form a complementary PWM pair */
    pwmConfig.pairOperation   = kPWM_Independent; //WT.EDIT kPWM_ComplementaryPwmA;
    pwmConfig.enableDebugMode = true;
    pwmConfig.forceTrigger = kPWM_Force_Local;  //WT.EDIT
    

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
    pwmConfig.initializationControl = kPWM_Initialize_LocalSync;   //WT.EDIT 2019-06-29
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
   PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 | kPWM_Control_Module_1 | kPWM_Control_Module_2, true);

    /* Start the PWM generation from Submodules 0, 1 and 2 */
    PWM_StartTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 | kPWM_Control_Module_1 | kPWM_Control_Module_2);
    
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
    uint32_t pwmFrequencyInHz = 1500; //5KHZ PWM of frequency  1khz ~ 4.5khz 1.5khz =1.5khz 2khz = 2khz 3kh=3kz


    pwmSourceClockInHz = PWM_SRC_CLK_FREQ;

    /* Set deadtime count, we set this to about 650ns */
    deadTimeVal = ((uint64_t)pwmSourceClockInHz * 650) / 1000000000;

    pwmSignal[0].pwmChannel       = kPWM_PwmA;
    pwmSignal[0].level            = kPWM_HighTrue;
    pwmSignal[0].dutyCyclePercent = 50; /* 1 percent dutycycle */
    pwmSignal[0].deadtimeValue    = deadTimeVal;

    pwmSignal[1].pwmChannel = kPWM_PwmB;
    pwmSignal[1].level      = kPWM_HighTrue;
    /* Dutycycle field of PWM B does not matter as we are running in PWM A complementary mode */
    pwmSignal[1].dutyCyclePercent = 50;
    pwmSignal[1].deadtimeValue    = deadTimeVal;

    /*********** PWMA_SM0 - phase A, configuration, setup 2 channel as an example ************/
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_0, pwmSignal, 2, kPWM_SignedCenterAligned, pwmFrequencyInHz,
                 pwmSourceClockInHz);

    /*********** PWMA_SM1 - phase B configuration, setup PWM A channel only ************/
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_1, pwmSignal, 1, kPWM_SignedCenterAligned, pwmFrequencyInHz,
                 pwmSourceClockInHz);

    /*********** PWMA_SM2 - phase C configuration, setup PWM A channel only ************/
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_2, pwmSignal, 1, kPWM_SignedCenterAligned, pwmFrequencyInHz,
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
void HALLSensor_Detected_BLDC(void)
{
  /* hall  */
  //BLDCMotor.uwStep = HallSensor_GetPinState();
   __IO uint32_t tmp = 0;
  //uwStep = HallSensor_GetPinState();
   uwStep = HallSensor_GetPinState();
  if(Dir == CW)
  {
    uwStep = (uint32_t)7 - uwStep;        // ?ù?Y?3Dò±íμ?1??é CW = 7 - CCW;
  }
#if 0  //WT.EDIT
  if( PWM_ChangeFlag == 1)
  {
    if(BLDCMotor.Dir == CW)
    {      
      BLDCMotor.uwStep = (uint32_t)7 - BLDCMotor.uwStep;        // ?ù?Y?3Dò±íμ?1??é CW = 7 - CCW;
    }
  }
  else 
  {
    if(BLDCMotor.Dir == CW)
    {
      BLDCMotor.Hall_PulNum ++;                                           // ??3?êyà??ó
      BLDCMotor.uwStep = (uint32_t)7 - BLDCMotor.uwStep;        // ?ù?Y?3Dò±íμ?1??é CW = 7 - CCW;
    }
    else
      BLDCMotor.Hall_PulNum --;                                           // ??3?êyà???
    
  }
  
#endif   
  /*---- six step changed phase */
  /*---- 1(001,U),IC2(010,V),IC3(100,W) ----*/
  PRINTF("uwStep = %d\n",uwStep);
  switch(uwStep)//switch(BLDCMotor.uwStep)
 {
    case 1://C+ A-
      /*  PWM_A0 B 0 stop  */ 
       PWMABC_Close_Channel(1);  //close B channel 
     //  DelayMs(500U);
      /*  PWM_A1 and PWM_B1 output */
       PWMABC_Select_Channel(2);    //open C upper half-bridge
      /*  PWM_A2 and PWM_B2 output  */
      PWMABC_Select_Channel(0);    
      //DelayMs(500U);
      break;
    
    case 2: //A+  B-
      /*  Channe3 configuration */ 
        PWMABC_Close_Channel(2); //close C channel 
      //  DelayMs(500U);
    
      /*  Channel configuration  */
        PWMABC_Select_Channel(0);
      
      /*  Channe2 configuration */
       PWMABC_Select_Channel(1);
      // DelayMs(500U);
      break;
    
    case 3:// C+ B-
      /*  Channel configuration */ 
         PWMABC_Close_Channel(0); //close A channel 
       //  DelayMs(500U);
      /*  Channe3 configuration  */
         PWMABC_Select_Channel(2);
     /*  Channe2 configuration  */
		 PWMABC_Select_Channel(1);
   // DelayMs(500U);
      break;
    
    case 4:// B+ C-
      /*  Channel configuration */ 
          PWMABC_Close_Channel(0); //close A channel 
        //  DelayMs(500U);
      /*  Channe2 configuration */
            PWMABC_Select_Channel(1);
      
      /*  Channe3 configuration */
          PWMABC_Select_Channel(2);
         // DelayMs(500U);
      break;
    
    case 5: // B+ A-

      /*  Channe3 configuration */       
        PWMABC_Close_Channel(2); //close C channel 
       //  DelayMs(500U);
      /*  Channe2 configuration */
         PWMABC_Select_Channel(1);
      
      /*  Channel configuration */
         PWMABC_Select_Channel(0);
        //  DelayMs(500U);
      break;
    
    case 6: // A+ C-

      /*  Channe2 configuration */ 
      PWMABC_Close_Channel(1); //close B channel 
       //DelayMs(500U);
      /*  Channel configuration */
         PWMABC_Select_Channel(0);
      
      /*  Channe3 configuration */
       PWMABC_Select_Channel(2);
	//   DelayMs(500U);
      
 
      break;
  }
   
  /* á￠?ì′￥·￠???à */
 // HAL_TIM_GenerateEvent(&htimx_BLDC, TIM_EVENTSOURCE_COM);
 // __HAL_TIM_CLEAR_IT(htim, TIM_FLAG_COM);
//  BLDCMotor.Lock_Time = 0;
 }




/**************************************************************
 *
 *Function Name: PWMABC_Selection 
 *Function Active :be used to kPWM_Control_Module_0 
 *
 *
 *
**************************************************************/
void PWMABC_Select_Channel(uint8_t s_pwma)
{

 if(s_pwma == 0)
 {
   PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmA, kPWM_SignedCenterAligned, 50); 
   PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_0, true);
 }
 else if (s_pwma == 1)
 	{
      PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_1, kPWM_PwmA, kPWM_SignedCenterAligned, 50); 
     PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_1, true);
	
 	}
 else if(s_pwma == 2)
 {
    PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_2, kPWM_PwmA, kPWM_SignedCenterAligned, 50); 
   PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_2, true);
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
void PWMABC_Close_Channel(uint8_t f_pwma)
{
   if(f_pwma == 0)
 {
   PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmA, kPWM_SignedCenterAligned, 0); 
   PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_0, true);
 }
 else if (f_pwma == 1)
 	{
      PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_1, kPWM_PwmA, kPWM_SignedCenterAligned, 0); 
     PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_1, true);
	
 	}
 else if(f_pwma == 2)
 {
    PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_2, kPWM_PwmA, kPWM_SignedCenterAligned, 0); 
   PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_2, true);
 }
    
}


