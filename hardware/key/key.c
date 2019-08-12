#include "key.h"

gpio_pin_config_t key_config = {
	  kGPIO_DigitalInput ,
	  0,
  };
/****************************************************
 *
 *Function Name: KEY_Init()
 *
 *
**********************************************/
void KEY_Init(void)
{

   /* 将GPIO设置为输入模式，芯片内部自动配置上拉电阻*/
    GPIO_QuickInit(HW_GPIOE,  29, kGPIO_Mode_IPU);
    GPIO_QuickInit(HW_GPIOE,  30, kGPIO_Mode_IPU);
	GPIO_QuickInit(HW_GPIOE,  24, kGPIO_Mode_IPU);
    GPIO_QuickInit(HW_GPIOE,  25, kGPIO_Mode_IPU);
	GPIO_QuickInit(HW_GPIOA,  1,  kGPIO_Mode_IPU);
    GPIO_QuickInit(HW_GPIOA,  2,  kGPIO_Mode_IPU);
	GPIO_QuickInit(HW_GPIOA,  4,  kGPIO_Mode_IPU);
    GPIO_QuickInit(HW_GPIOA,  5,  kGPIO_Mode_IPU);
	GPIO_QuickInit(HW_GPIOA,  12, kGPIO_Mode_IPU);
    GPIO_QuickInit(HW_GPIOA,  13, kGPIO_Mode_IPU);
#if 0
/* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);
    
    CLOCK_EnableClock(kCLOCK_PortA);

    /* PORTD0 (pin 93) is configured as PTD0, */
    PORT_SetPinMux(PORTE, 30U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTE, 29U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTE, 24U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTE, 25U, kPORT_MuxAsGpio);

	/* PORTA (pin 93) is configured as PTD0, */
	PORT_SetPinMux(PORTA, 1U,  kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTA, 2U,  kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTA, 4U,  kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTA, 5U,  kPORT_MuxAsGpio);
    PORT_SetPinMux(PORTA, 12U, kPORT_MuxAsGpio);
	PORT_SetPinMux(PORTA, 13U, kPORT_MuxAsGpio);
	

        GPIO_PinInit(KEY1_GPIO,    KEY1_GPIO_PIN, 		&key_config);
        GPIO_PinInit(KEY2_GPIO,    KEY2_GPIO_PIN, 		&key_config);
		GPIO_PinInit(KEY3_GPIO,    KEY3_GPIO_PIN, 		&key_config);
		GPIO_PinInit(KEY4_GPIO,    KEY4_GPIO_PIN, 		&key_config);
		GPIO_PinInit(KEY5_GPIO,    KEY5_GPIO_PIN, 		&key_config);
		GPIO_PinInit(KEY6_GPIO,    KEY6_GPIO_PIN, 		&key_config);
        GPIO_PinInit(KEY7_GPIO,    KEY7_GPIO_PIN, 		&key_config);
		GPIO_PinInit(KEY8_GPIO,    KEY8_GPIO_PIN, 		&key_config);
		GPIO_PinInit(KEY9_GPIO,    KEY9_GPIO_PIN,   	&key_config);
		GPIO_PinInit(KEY10_GPIO,   KEY10_GPIO_PIN, 	    &key_config);
#endif 

}


/*****************************************************/

/* ����ɨ����� */
//mode: 0 - ��֧����������1-֧��������
// 0��û���κΰ�������
//1�����°���
// ����Ȩ ��KEY0>KEY1>KEY2>KEY>3>KEY4>
/****************************************************/
uint8_t KEY_Scan(uint8_t mode)
{
   static uint8_t key_up = 1; //�����ɿ���־
   if(mode == 1) key_up =1;
   if(key_up &&(KEY1 == 1 || KEY2 ==1 ||KEY3 ==1|| KEY4==1 ||KEY5==1||KEY6==1||KEY7==1||\
  	           	KEY8==1|| KEY9==1 || KEY10 == 1))
  
   {
     DelayMs(10);
     key_up =1;
     if(KEY1 ==1)           	 return KEY1_PRES;
     else if(KEY2 == 1)     	 return KEY2_PRES;
	 else if(KEY3 == 1)       	 return KEY3_PRES;
	 else if(KEY4 == 1)       	 return KEY4_PRES;
	 else if(KEY5 == 1)       	 return KEY5_PRES;
	 else if(KEY6 == 1)       	 return KEY6_PRES;
	 else if(KEY7 == 1)       	 return KEY7_PRES;
	 else if(KEY8 == 1)       	 return KEY8_PRES;
	 else if(KEY9 == 1)          return KEY9_PRES;
	 else if(KEY10 ==1)          return KEY10_PRES;
   }
   else if(KEY1 == 0 && KEY2 == 0 && KEY3==0 && KEY4==0 && KEY5==0 &&KEY6==0 && KEY7==0|| \
   	              KEY8 == 0 && KEY9== 0 && KEY10 == 0)
   {
      key_up = 0;
   }
   return KEY_NONE;//return 0;
}