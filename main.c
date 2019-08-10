/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Simplify software DATA-2019-08-07
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_pwm.h"
#include "pin_mux.h"
#include "fsl_xbara.h"
#include "led.h"
#include "key.h"
#include "pwm.h"
#include "adc.h"
#include "output.h"
   
   

/*******************************************************************************
 *
 * Funtion Name: main(void)
 *
 *
 *
 ******************************************************************************/
int main(void)
{
    uint8_t key; 
	
    output_t recoder_number;
  

    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
   
    LED_Init();
    KEY_Init();
    DelayInit();
    HALL_Init();
    HallSensor_GetPinState();
    ADC_CADC_Init();
    OUTPUT_Fucntion_Init();
	
    /* Set the PWM Fault inputs to a low value */
    PWM_BLDC_Init();
   
    

    while (1)
    {
        
    
     key = KEY_Scan(0);
     PRINTF("key = %d \r\n",key);
     switch(key)
     {
     
      case START_PRES:
	   recoder_number.start_number++ ;
       LED1 =1;
       LED2 = 0 ;
	   if(recoder_number.start_number==1)
	   	{
		   uwStep = HallSensor_GetPinState();
		   PRINTF("uwStep = %d \r\n",uwStep);
		   HALLSensor_Detected_BLDC(uwStep);
	   	}
	   else 
	   {
             recoder_number.start_number=0;

	   }
       
        
        break;
     case KEY3_PRES:
       
       LED2 = !LED2 ;
       DelayMs(200U);
        break;
     case KEY4_PRES:
        LED1 = !LED1;
        LED2 = !LED2 ;
        DelayMs(100U);
        break;
     case KEY5_PRES:
         LED2 = !LED2;
         DelayMs(100U);
     break;
     case KEY6_PRES: //�����ź�
     
        DOOR = 1;
        LED1=0;
        DelayMs(200U);
        LED1=1;
        break;
    case KEY7_PRES:
        LED1=1;
		LED2=1;
        DelayMs(300U);
        LED2=0;
		LED1=0;
        break;
   case KEY8_PRES:  //����̽���ź����
        WHEEL =1;
        LED1=0;
        LED2=0;
        DelayMs(100U);
        LED2=1;
        LED1=1;
        DelayMs(100U);
        break;
    case KEY9_PRES: //��ˢ��
        recoder_number.wiper_number ++;
		LED1 =!LED1;
        LED2 =!LED2;
        DelayMs(100U);
	if(recoder_number.wiper_number ==1)
	{
           WIPER_2  = 0;  
           WIPER_1 = 1;
	}
	else if(recoder_number.wiper_number >=2 )
	{
           WIPER_1 = 0;
           WIPER_2  = 1;
	   recoder_number.wiper_number =0;
            
	}
		
        
        break;
     case KEY10_PRES: //�յ�����
       recoder_number.air_number++;
        
       if(recoder_number.air_number==1)
	{
           AIR = 1;
		   LED2=1;
           DelayMs(500U);
           LED2=0;
	}
       else if( recoder_number.air_number==2 || recoder_number.air_number> 2)
	{
           AIR = 0;
		   LED1=1;
           DelayMs(500U);
           LED1=0;
           recoder_number.air_number =0;
		   
	}
		
		 
        break;
     
     
     }

    }
 
}
/******************************************************************************
 *
 * Function Name: 
 * Function Active: Interrpt brake input key 
 * @brief Interrupt service fuction of switch.
 *
 * This function toggles the LED
 *
******************************************************************************/
#if 0
void BARKE_KEY_IRQ_HANDLER(void )//void BOARD_BRAKE_IRQ_HANDLER(void)
{
    /* Clear external interrupt flag. */
    GPIO_PortClearInterruptFlags(BRAKE_KEY_GPIO, 1U << BRAKE_KEY_GPIO_PIN );
    /* Change state of button. */
   
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}
#endif 

