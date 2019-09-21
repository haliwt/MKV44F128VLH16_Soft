#include "input.h"

void SD315AI_SO12_Init(void)
{
    gpio_pin_config_t sd315ai_config = {
	  kGPIO_DigitalInput ,
	  0,
    };

   

   /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);
  

	//PORT_SetMultiplePinsConfig();

    /* PORTD0 (pin 93) is configured as PTD0, */
        PORT_SetPinMux(PORTE, 19U, kPORT_MuxAsGpio);
		PORT_SetPinMux(PORTE, 18U, kPORT_MuxAsGpio);
		PORT_SetPinMux(PORTE, 17U, kPORT_MuxAsGpio);
		
        GPIO_PinInit(SD315AI_SO12_A_GPIO,    19, 	&sd315ai_config);
        GPIO_PinInit(SD315AI_SO12_B_GPIO,    18 , 	&sd315ai_config);
		GPIO_PinInit(SD315AI_SO12_C_GPIO,    17 , 	&sd315ai_config);

        //输入
        PORT_PinPullConfig(HW_GPIOE, 19, kPORT_PullDown);
        PORT_PinPullConfig(HW_GPIOE, 18, kPORT_PullDown);
        PORT_PinPullConfig(HW_GPIOE, 17, kPORT_PullDown);
}








