#include "output.h"


/*******************************************
 *
 *Function Name:OUTPUT_Function_Init()
 *Function : initial
 *
 *
*******************************************/
void OUTPUT_Fucntion_Init(void)
{

     gpio_pin_config_t output_config = {
	  kGPIO_DigitalOutput ,
	  1,
    };

   

   /* Port D Clock Gate Control: Clock enabled */
  
    CLOCK_EnableClock(kCLOCK_PortA);
    CLOCK_EnableClock(kCLOCK_PortC);
     CLOCK_EnableClock(kCLOCK_PortB);
	//PORT_SetMultiplePinsConfig();

    /* PORTD0 (pin 93) is configured as PTD0, */
        PORT_SetPinMux(PORTC, 7U, kPORT_MuxAsGpio);
		PORT_SetPinMux(PORTC, 6U, kPORT_MuxAsGpio);
		PORT_SetPinMux(PORTC, 4U, kPORT_MuxAsGpio);

        PORT_SetPinMux(PORTB, 19U, kPORT_MuxAsGpio);
		PORT_SetPinMux(PORTB, 17U, kPORT_MuxAsGpio);
		PORT_SetPinMux(PORTB, 0U, kPORT_MuxAsGpio);
        PORT_SetPinMux(PORTB, 0U, kPORT_MuxAsGpio);
        PORT_SetPinMux(PORTA, 19U, kPORT_MuxAsGpio);
		
        GPIO_PinInit(SD315AI_VL_A_GPIO,    7, 	&output_config );
        GPIO_PinInit(SD315AI_VL_B_GPIO,    6 , &output_config );
		GPIO_PinInit(SD315AI_VL_C_GPIO,    4 , &output_config );

        GPIO_PinInit(GPIOB,    19, 	&output_config );
        GPIO_PinInit(GPIOB,    17 , &output_config );
		GPIO_PinInit(GPIOB,    0 , &output_config );
        GPIO_PinInit(GPIOA,    19 , &output_config );

     
        PORT_PinPullConfig(HW_GPIOC, 7, kPORT_PullDown);
        PORT_PinPullConfig(HW_GPIOC, 6, kPORT_PullDown);
        PORT_PinPullConfig(HW_GPIOC, 4, kPORT_PullDown);

        PORT_PinPullConfig(HW_GPIOB, 19, kPORT_PullDown);
        PORT_PinPullConfig(HW_GPIOB, 17, kPORT_PullDown);
        PORT_PinPullConfig(HW_GPIOB, 0, kPORT_PullDown);

        PORT_PinPullConfig(HW_GPIOA, 19, kPORT_PullDown);

   WHEEL_OUTPUT =0;
   DOOR_OUTPUT  =0;
   WIPER_OUTPUT_1 =0;
   AIR_OUTPUT = 0;

   SD315_VL_A_OUTPUT = 0;
   SD315_VL_B_OUTPUT = 0;
   SD315_VL_C_OUTPUT = 0;
}

/*******************************************
 *
 *Function Name:AIR_OUTPUT_F(void)
 *Function : air conditioner output
 *
 *
*******************************************/
void ABC_POWER_OUTPUT_Init(void)
{
   GPIO_QuickInit(HW_GPIOC,  10, kGPIO_Mode_OPP);
   GPIO_QuickInit(HW_GPIOC,  11, kGPIO_Mode_OPP);
   GPIO_QuickInit(HW_GPIOC,  0, kGPIO_Mode_OPP);
   A_POWER_OUTPUT =0;
   B_POWER_OUTPUT =0;
   C_POWER_OUTPUT =0;

}











