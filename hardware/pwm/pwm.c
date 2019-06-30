#include "pwm.h"

pwm_config_t pwmConfig;

static void PWM_DRV_Init3PhPwm(void);


void PWM_BLDC_Init(void)
{
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

    /* Use full cycle reload */
    pwmConfig.reloadLogic = kPWM_ReloadPwmFullCycle;
    /* PWM A & PWM B form a complementary PWM pair */
    pwmConfig.pairOperation   = kPWM_ComplementaryPwmA;
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

    /* Call the init function with demo configuration */
    PWM_DRV_Init3PhPwm();

    /* Set the load okay bit for all submodules to load registers from their buffer */
   PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 | kPWM_Control_Module_1 | kPWM_Control_Module_2, true);

    /* Start the PWM generation from Submodules 0, 1 and 2 */
    PWM_StartTimer(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 | kPWM_Control_Module_1 | kPWM_Control_Module_2);

}



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
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_1, pwmSignal, 2, kPWM_SignedCenterAligned, pwmFrequencyInHz,
                 pwmSourceClockInHz);

    /*********** PWMA_SM2 - phase C configuration, setup PWM A channel only ************/
    PWM_SetupPwm(BOARD_PWM_BASEADDR, kPWM_Module_2, pwmSignal, 2, kPWM_SignedCenterAligned, pwmFrequencyInHz,
                 pwmSourceClockInHz);
}

