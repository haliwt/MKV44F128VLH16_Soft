#include "hall.h"


gpio_pin_config_t hall_config = {
	  kGPIO_DigitalInput ,
	  1,
  };

void HALL_Init(void)
{
   GPIO_QuickInit(HW_GPIOE, 16, kGPIO_Mode_IPU);
   GPIO_QuickInit(HW_GPIOC, 6, kGPIO_Mode_IPU);
   GPIO_QuickInit(HW_GPIOB, 3, kGPIO_Mode_IPU);

}







