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
   GPIO_QuickInit(HW_GPIOE,  19, kGPIO_Mode_OPP);
   GPIO_QuickInit(HW_GPIOE,  18, kGPIO_Mode_OPP);
   GPIO_QuickInit(HW_GPIOE,  17, kGPIO_Mode_OPP);
   GPIO_QuickInit(HW_GPIOA,  19, kGPIO_Mode_OPP);
   
   GPIO_QuickInit(HW_GPIOB,  0, kGPIO_Mode_OPP);
   WHEEL_OUTPUT =0;
   DOOR_OUTPUT  =0;
   WIPER_OUTPUT_1 =0;
   WIPER_OUTPUT_2 =0;
   AIR_OUTPUT = 0;
}

/*******************************************
 *
 *Function Name:AIR_OUTPUT_F(void)
 *Function : air conditioner output
 *
 *
*******************************************/











