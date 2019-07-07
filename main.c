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
/*******************************************************************************
 *
 * Funtion Name: main(void)
 *
 *
 *
 ******************************************************************************/
int main(void)
{
   // uint8_t key; 
   // uint32_t pwmVal = 4;
   // uint8_t i;

    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    PRINTF("uwStep numbers 00\n");  
    LED_Init();
    KEY_Init();
    DelayInit();
    HALL_Init();
    HallSensor_GetPinState();
	
    /* Set the PWM Fault inputs to a low value */
    PWM_BLDC_Init();
   
    PRINTF("uwStep numbers 0\n");  

    while (1)
    {
         PRINTF("uwStep numbers \n");
         HallSensor_GetPinState();
	 HALLSensor_Detected_BLDC();
         LED1 = !LED1;
         DelayMs(100U);
#if 0
     switch(i)
     {
     case 1:
       LED2=1;
       PWMABC_Select_Channel(2);
       //PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_2, kPWM_PwmA, kPWM_SignedCenterAligned, 50); 
       //PWM_SetPwmLdok(BOARD_PWM_BASEADDR,  kPWM_Control_Module_2, true);
       DelayMs(500U);
       break;
     case 2:  
        LED2=0;
        PWMABC_Close_Channel(2);
         PWMABC_Close_Channel(1);
         PWMABC_Close_Channel(0);
       //PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_2, kPWM_PwmA, kPWM_SignedCenterAligned, 0);
       //PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_Control_Module_2, true);
      DelayMs(500U);
      break;
     
     }
     if(i==2)i =0;
#endif 
#if 0 
     key = KEY_Scan(0);
     switch(key)
     {
     
     case KEY1_PRES:
       
      LED1 = !LED1;
      DelayMs(100U);
     PWMABC_Close_Channel(2);
      LED2 = 1;
     //  printf("UART%d OK! Hello Kinetis\r\n", instance);
       break;
     case KEY2_PRES:
       LED1 =1;
       LED2 = 0 ;
        PWMABC_Select_Channel(2);
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
     case KEY0XA_PRES:
        LED2=1;
        DelayMs(500U);
         LED2=0;
        break;
     
     
     }
#endif 
#if 0 
        DelayMs(100U);
        pwmVal = pwmVal + 4;

        /* Reset the duty cycle percentage */
        if (pwmVal > 100)
        {
            pwmVal = 4;
        }

        /* Update duty cycles for all 3 PWM signals */
        PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_0, kPWM_PwmA, kPWM_SignedCenterAligned, pwmVal);
        PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_1, kPWM_PwmA, kPWM_SignedCenterAligned, (pwmVal >> 1));
        PWM_UpdatePwmDutycycle(BOARD_PWM_BASEADDR, kPWM_Module_2, kPWM_PwmA, kPWM_SignedCenterAligned, (pwmVal >> 2));

        /* Set the load okay bit for all submodules to load registers from their buffer */
      PWM_SetPwmLdok(BOARD_PWM_BASEADDR, kPWM_Control_Module_0 | kPWM_Control_Module_1 | kPWM_Control_Module_2, true);
#endif 
    }
	
 
}
