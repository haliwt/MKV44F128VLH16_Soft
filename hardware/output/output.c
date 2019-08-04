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
}

/*******************************************
 *
 *Function Name:AIR_OUTPUT_F(void)
 *Function : air conditioner output
 *
 *
*******************************************/











