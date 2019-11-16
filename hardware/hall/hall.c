#include "hall.h"



__IO uint16_t uSaHall;
__IO uint16_t vSaHall;
__IO uint16_t wSaHall; //电机 W相，采样电压值





void NO_HALL_GPIO_Init(void)
{
      gpio_pin_config_t key_config = {
	  kGPIO_DigitalInput ,
	  0,
    };

   /* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);
    
    CLOCK_EnableClock(kCLOCK_PortB);
	CLOCK_EnableClock(kCLOCK_PortC);

	//PORT_SetMultiplePinsConfig();

    /* PORTD0 (pin 93) is configured as PTD0, */
    PORT_SetPinMux(PORTB, 18U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTC, 8U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTE, 16U, kPORT_MuxAsGpio);
		
    GPIO_PinInit(NO_HALL_A_GPIO, NO_HALL_A_GPIO_PIN, &key_config);
    GPIO_PinInit(NO_HALL_B_GPIO, NO_HALL_B_GPIO_PIN, &key_config);
	GPIO_PinInit(NO_HALL_C_GPIO, NO_HALL_C_GPIO_PIN, &key_config);
	
    
    PORT_PinPullConfig(HW_GPIOB, 18,  kPORT_PullUp);
    PORT_PinPullConfig(HW_GPIOC, 8,   kPORT_PullUp);
    PORT_PinPullConfig(HW_GPIOE, 16,  kPORT_PullUp);
        
}

/*********************************************************
 *
 *Funtion Name: NO_Sensorless_GetPinState
 *
 *
 *
*********************************************************/
uint8_t NO_HallSensor_GetPinState(void)
{
  __IO  static uint16_t State ;
  State  = 0;

  ADC_UVW_Sample_HALL_Value();

 
  
  if(uSaHall < 10000)  //U
  {
    State |= 0x01U;
  }
  if(vSaHall < 10000)  //V
  {
    State |= 0x02U;
  }
  if(wSaHall < 10000)  //W
  {
    State |= 0x04U;
  }
   return State;

}





