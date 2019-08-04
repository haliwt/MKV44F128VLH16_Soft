/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
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
   // uint32_t pwmVal = 4;
   // uint32_t i;

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
	
    /* Set the PWM Fault inputs to a low value */
    PWM_BLDC_Init();
   
    

    while (1)
    {
        


     key = KEY_Scan(0);
     printf("key = %d \r\n",key);
     switch(key)
     {
     
     case KEY1_PRES:
       
      LED1 = !LED1; // GPIO_PortToggle(GPIOD,6);
      DelayMs(100U);
     
     // LED2 = 1;
     //  printf("UART%d OK! Hello Kinetis\r\n", instance);
       break;
     case KEY2_PRES:
       LED1 =1;
       LED2 = 0 ;
        
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
     case KEY6_PRES:
      
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
   case KEY8_PRES:
        LED1=0;
        LED2=0;
        DelayMs(500U);
        LED2=1;
        LED1=1;
        DelayMs(500U);
        break;
    case KEY9_PRES:
        LED1 =!LED1;
        LED2 =!LED2;
        DelayMs(500U);
        break;
     case KEY10_PRES:
        LED2=1;
        DelayMs(500U);
         LED2=0;
        break;
     
     
     }

    }
 
}
