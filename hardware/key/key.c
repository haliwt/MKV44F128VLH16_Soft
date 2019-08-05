#include "key.h"


/****************************************************
 *
 *Function Name: KEY_Init()
 *
 *
**********************************************/
void KEY_Init(void)
{
   gpio_pin_config_t key_config = {
	  kGPIO_DigitalInput ,
	  1,
  };

#if 0
   /* 将GPIO设置为输入模式，芯片内部自动配置上拉电阻*/
    GPIO_QuickInit(HW_GPIOE, 29, kGPIO_Mode_IPD);
    GPIO_QuickInit(HW_GPIOE, 30, kGPIO_Mode_IPD);
    GPIO_QuickInit(HW_GPIOE, 24, kGPIO_Mode_IPD);
    GPIO_QuickInit(HW_GPIOE, 25, kGPIO_Mode_IPD);
    GPIO_QuickInit(HW_GPIOA, 1, kGPIO_Mode_IPD);
    GPIO_QuickInit(HW_GPIOA,  2, kGPIO_Mode_IPD);
    GPIO_QuickInit(HW_GPIOA, 4, kGPIO_Mode_IPD);
    GPIO_QuickInit(HW_GPIOA,  5, kGPIO_Mode_IPD);
    GPIO_QuickInit(HW_GPIOA, 12, kGPIO_Mode_IPD);
    GPIO_QuickInit(HW_GPIOA,  13, kGPIO_Mode_IPD);
#endif 
      
#if 1
/* Port D Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);
    
    CLOCK_EnableClock(kCLOCK_PortA);

	//PORT_SetMultiplePinsConfig();

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
	

        GPIO_PinInit(BRAKE_GPIO,    BRAKE_GPIO_PIN, 		&key_config);
        GPIO_PinInit(START_GPIO,    START_GPIO_PIN, 		&key_config);
		GPIO_PinInit(KEY3_GPIO,     KEY3_GPIO_PIN, 		    &key_config);
		GPIO_PinInit(KEY4_GPIO,     KEY4_GPIO_PIN, 		    &key_config);
		GPIO_PinInit(KEY5_GPIO,     KEY5_GPIO_PIN, 		    &key_config);
		GPIO_PinInit(KEY6_GPIO,     KEY6_GPIO_PIN, 		    &key_config);
        GPIO_PinInit(KEY7_GPIO,     KEY7_GPIO_PIN, 		    &key_config);
		GPIO_PinInit(KEY8_GPIO,     KEY8_GPIO_PIN, 		    &key_config);
		GPIO_PinInit(KEY9_GPIO,     KEY9_GPIO_PIN,   	    &key_config);
		GPIO_PinInit(KEY10_GPIO,    KEY10_GPIO_PIN, 	    &key_config);
        
        PORT_PinPullConfig(HW_GPIOE, 29, kPORT_PullDown);
        PORT_PinPullConfig(HW_GPIOE, 30, kPORT_PullDown);
        PORT_PinPullConfig(HW_GPIOE, 24, kPORT_PullDown);
        PORT_PinPullConfig(HW_GPIOE, 25, kPORT_PullDown);
        
        PORT_PinPullConfig(HW_GPIOA, 1,  kPORT_PullDown);
        PORT_PinPullConfig(HW_GPIOA, 2,  kPORT_PullDown);
        PORT_PinPullConfig(HW_GPIOA, 4,  kPORT_PullDown);
        PORT_PinPullConfig(HW_GPIOA, 5,  kPORT_PullDown);
        PORT_PinPullConfig(HW_GPIOA, 12, kPORT_PullDown);
        PORT_PinPullConfig(HW_GPIOA, 13, kPORT_PullDown);
                
                

#endif 

}


/*****************************************************

* ����ɨ����� 
* mode: 0 - ��֧����������1-֧��������
* 0��û���κΰ�������
*�����°���
* ����Ȩ ��BRAKE>START_KEY>DIR_KEY>DIGITAL_ADD_KEY>DIGITAL_REDUCE_RKEY>
*
****************************************************/
uint8_t KEY_Scan(uint8_t mode)
{
   static uint8_t key_up = 1; //�����ɿ���־
   if(mode == 1) key_up =1;
  if(key_up &&(BRAKE_KEY == 1 || START_KEY ==1 ||DIR_KEY ==1 || DIGITAL_ADD_KEY==1||\
  	            DIGITAL_REDUCE_KEY==1||DOOR_KEY==1||HALL_SWITCH_KEY==1||\
  	           	WHEEL_KEY==1|| WIPERS_KEY==1 || AIR_KEY == 1))
  
   {
     DelayMs(10);
     key_up =0;
     if(BRAKE_KEY ==1)
 	 {
		/* �ȴ������������˳�����ɨ�躯�� */
	      while(BRAKE_KEY==1);
	       /* ����ɨ����ϣ�ȷ���������� */
		 return BRAKE_PRES;
 	 }
     else if(START_KEY == 1)
     {
	     /* �ȴ������������˳�����ɨ�躯�� */
	      while(START_KEY==1);
	       /* ����ɨ����ϣ�ȷ���������� */
	      return START_PRES;
      }
	 else if(DIR_KEY==1)
	 {
         /* �ȴ������������˳�����ɨ�躯�� */
	     while(DIR_KEY==1);
	       /* ����ɨ����ϣ�ȷ���������� */

		 return KEY3_PRES;
	 }
	 else if(DIGITAL_ADD_KEY==1)       	
	 {
          /* �ȴ������������˳�����ɨ�躯�� */
	     while(DIGITAL_ADD_KEY==1);
	       /* ����ɨ����ϣ�ȷ���������� */
	    return KEY4_PRES;
	 }
	 
	 else if(DIGITAL_REDUCE_KEY==1)
	 {
         /* �ȴ������������˳�����ɨ�躯�� */
	   while(DIGITAL_REDUCE_KEY==1);
	       /* ����ɨ����ϣ�ȷ���������� */
	   return KEY5_PRES;
	 }
	 
	 else if(DOOR_KEY==1)
	 {
         /* �ȴ������������˳�����ɨ�躯�� */
	     while(DOOR_KEY==1);
	       /* ����ɨ����ϣ�ȷ���������� */
	     return KEY6_PRES;
	 }
	 
	 else if(HALL_SWITCH_KEY==1)       	
	 {
         /* �ȴ������������˳�����ɨ�躯�� */
	   while(HALL_SWITCH_KEY==1);
	       /* ����ɨ����ϣ�ȷ���������� */
	   return KEY7_PRES;
	 }
	 
	 else if(WHEEL_KEY==1) 
	 {
          /* �ȴ������������˳�����ɨ�躯�� */
	     while(WHEEL_KEY==1);
	       /* ����ɨ����ϣ�ȷ���������� */
		return KEY8_PRES;
	 }
	 else if(WIPERS_KEY==1) 
	 {
          /* �ȴ������������˳�����ɨ�躯�� */
	     while(WIPERS_KEY==1);
	       /* ����ɨ����ϣ�ȷ���������� */
	    return KEY9_PRES;
	 }
	 
	 else if(AIR_KEY==1)         
	 {
          /* �ȴ������������˳�����ɨ�躯�� */
	     while(AIR_KEY==1);
	       /* ����ɨ����ϣ�ȷ���������� */
		return KEY10_PRES;
	 }
   }
   else if(BRAKE_KEY == 0 && START_KEY == 0 && DIR_KEY==0 && DIGITAL_ADD_KEY==0 &&\
   	           DIGITAL_REDUCE_KEY==0 &&DOOR_KEY==0 && HALL_SWITCH_KEY==0 && \
   	              WHEEL_KEY == 0 && WIPERS_KEY== 0 && AIR_KEY == 0) key_up = 1;
   
     return KEY_UP;
   
   
}
#if 0
/****************************************************************************
  * ��������: 
  * ���뺯����������
  * ����: KEY_DOWN -��������
  *       KEY_UP  -����û�а���
  * ����ֵ��
*****************************************************************************/
KEYState_TypeDef KEY3_StateRead(void)
{
  /* �0�9�0�9�0�6�0�3�0�7�0�9�0�8����0�7�0�4���0�0�0�8�0�5�0�4�0�3�0�4�0�9�0�3�0�8�0�5���0�9�0�8�0�5���0�3��0�7�0�3�0�0���0�7�0�0�0�1�0�5�0�1�0�6�0�4�0�1�0�4�0�8�0�5���0�3��0�7�0�3�0�0���0�7�0�0�0�1�0�5�0�3�0�6�0�5�0�2�0�4�0�8�0�5�0�2�0�3 */
  if(HAL_GPIO_ReadPin(KEY3_GPIO,KEY3_GPIO_PIN)==KEY3_DOWN_LEVEL)
  {
    /* �0�5�0�7�0�8���0�6�0�3�0�4�0�3�0�9�0�2�0�8���0�4�0�1�0�5�0�1�0�3�0�4�0�6�0�5�0�9�0�9�0�9�0�4 */
    HAL_Delay(10);
    /*�ж��Ƿ񰴼����� */
    if(HAL_GPIO_ReadPin(KEY3_GPIO,KEY3_GPIO_PIN)==KEY3_DOWN_LEVEL)
    {
      /* �ȴ������������˳�����ɨ�躯�� */
      while(HAL_GPIO_ReadPin(KEY3_GPIO,KEY3_GPIO_PIN)==KEY3_DOWN_LEVEL);
       /* ����ɨ����ϣ�ȷ���������� */
      return KEY_DOWN;
    }
  }
  /* ����û�б����� */
  return KEY_UP;
}
#endif 