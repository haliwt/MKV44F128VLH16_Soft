/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause  MKV46F256VLH16
 */

/*System includes.*/
#include <stdio.h>
#include "amclib_FP.h"
#include "mlib_FP.h"
#include "gflib_FP.h"
#include "gdflib_FP.h"
#include "gmclib_FP.h"

/* Kernel includes. */
//#include "FreeRTOS.h"
//#include "task.h"
//#include "queue.h"
//#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "fsl_debug_console.h"

#include "fsl_pwm.h"
#include "pin_mux.h"
#include "fsl_xbara.h"
#include "led.h"
#include "key.h"
#include "bldc.h"
#include "adc.h"
#include "pollingusart.h"
#include "output.h"
#include "input.h"
//#include "usart_edma_rb.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

output_t motor_ref;

/*******************************************************************************
 *
 * Code
 *
 * @brief Main function
 *
******************************************************************************/
int main(void)
{

 
     
     uint8_t printx1[]="Dir = 1 is OK !!!! CW \r\n";
     uint8_t printx2[]="Dir = 0 is OK #### CCW \r\n";
   uint8_t printx3[]="motor run is OK @@@@@@@@@ \r\n";
   uint8_t printx4[]="motor stop !!!!!!!! \r\n";
     uint8_t ucKeyCode=0,abc_s=0;
     uint8_t dir_s =0,start_s = 0;

 

    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    
   
    LED_Init();
    KEY_Init();
    DelayInit();
    HALL_Init();
    
    SD315AI_SO12_Input_Init();
    
    
    OUTPUT_Fucntion_Init();
    ADC_CADC_Init();
    ABC_POWER_OUTPUT_Init();
    /* Set the PWM Fault inputs to a low value */
    PWM_BLDC_Init();
    USART_POLLING_Init();
     
   while(1)
   {

          ucKeyCode = KEY_Scan(0);
          
          if(motor_ref.motor_run == 1 )
           {
               if(motor_ref.power_on == 1)
               {
                   printf("************************************************************\r \n");
                   motor_ref.power_on ++ ;
                   SD315AI_Check_Fault();
                   if(Dir==0)  
                   {
                        
#if 1
                         
                        uwStep = HallSensor_GetPinState();
                        switch(uwStep)
                        {
                        case 5 :
                         BLDC_CCW_SIX_5_Run();
                          break;
                        case 4:
                          
                          BLDC_CCW_SIX_4_Run();
                         break;
                        
                        case 6:
                          BLDC_CCW_SIX_6_Run();
                        break;
                          
                         case 2:
                           BLDC_CCW_SIX_2_Run();
                         break;
                         
                         case 3:
                           BLDC_CCW_SIX_3_Run();
                         break;
                         
                         case 1:
                            BLDC_CCW_SIX_1_Run();
                         break;
                        }
                          
                     printf("Dir = %d\r \n",Dir); 
                     printf("PWM_Duty= %d\r \n",PWM_Duty);
#endif                         
                 }
                 else //CW 
                 {

#if 1
                    uwStep = HallSensor_GetPinState();
                        switch(uwStep)
                        {
                        case 5 :
                         BLDC_CW_Six_5_Run();
                          break;
                        case 4:
                          
                          BLDC_CW_Six_4_Run();
                         break;
                        
                        case 6:
                          BLDC_CW_Six_6_Run();
                        break;
                          
                         case 2:
                           BLDC_CW_Six_2_Run();
                         break;
                         
                         case 3:
                           BLDC_CW_Six_3_Run();
                         break;
                         
                         case 1:
                            BLDC_CW_Six_1_Run();
                         break;
                        }

                    printf("Dir = %d\r \n",Dir); 
#endif                 
                 }
               }
              else 
              {
                  
                  PWM_Duty=40;
                  uwStep = HallSensor_GetPinState();
                                 
                  HALLSensor_Detected_BLDC();
                 // PWM_Duty = CADC_Read_ADC_Value();
                  printf("PWM_Duty= %d\r \n",PWM_Duty);
                  SD315AI_Check_Fault();
                 
               }
             }
          
          else
          {
              if(motor_ref.stop_numbers !=1)
              {
                 PMW_AllClose_ABC_Channel();
                 printf("PWMA_ALLClose_0 \r \n");
              }
              else if(motor_ref.stop_numbers==1)
              {
                  PMW_AllClose_ABC_Duty_20_Channel();
                  PMW_AllClose_ABC_Duty_10_Channel();
                  PMW_AllClose_ABC_Channel();
                  motor_ref.stop_numbers++;
                  printf("PWMA_ALLClose_20 \r \n");
              }
            
           }
      
       /*处理接收到的按键功能*/  
		if(ucKeyCode !=KEY_UP) 
		{
           switch(ucKeyCode)
            { 
                 
                  case ABC_POWER_PRES :
                     PRINTF("ABC_PRES key  \r\n");
                     abc_s ++;
                    if((abc_s == 1)||(motor_ref.abc_numbers ==1))
				    {
                       A_POWER_OUTPUT =1;
                       motor_ref.abc_numbers =0;
                 
                    }
				     else 
				     {
                         A_POWER_OUTPUT =0; //shut down
                         
                         abc_s =0;
                        
				     }
                      
				  	break;
                #if 1		
                  case START_PRES:
                   PRINTF("START_PRES key \r\n");
				    start_s ++;
		          if( start_s== 1)
		          {
                      motor_ref.motor_run =1 ;
                      motor_ref.power_on = 1;
                    UART_WriteBlocking(DEMO_UART, printx3, sizeof(printx3) - 1);
                    // printf("START KEY is motor run \r\n");
                    
    			   }
				  else 
				  {
                     
					 start_s =0;
                      motor_ref.motor_run = 0;
                     motor_ref.power_on =0;
                       motor_ref.stop_numbers=1;
					 
					 // printf("START KEY IS STOP\r\n");
					 UART_WriteBlocking(DEMO_UART, printx4, sizeof(printx4) - 1);
				  }
                 
				  break;
			#endif 
				 case DIR_PRES: //3

			       dir_s ++ ;
	  			 if(dir_s == 1)
	   			 {
                         Dir=1;
                         motor_ref.power_on = 1;
                       //  printf(" dir is  =1 \r\n");
                         UART_WriteBlocking(DEMO_UART, printx1, sizeof(printx1) - 1);
				  }
				 else 
				   {
                       dir_s = 0;
                       motor_ref.power_on = 1;
                      //dirvalue =0; //逆时针旋转
                        Dir = 0;
					 // printf(" dir is  =0 \r\n");
                       UART_WriteBlocking(DEMO_UART, printx2, sizeof(printx2) - 1);
				   }
			
           		break;
            default :
              
      
              break;
			
        }
        
	}

   }

}
/******************************************************************************
 *
 * Function Name:BARKE_KEY_IRQ_HANDLER(void)
 * Function Active: Interrpt brake input key 
 * @brief Interrupt service fuction of switch.
 *
 * This function toggles the LED
 *
******************************************************************************/
#if 1
void BARKE_KEY_IRQ_HANDLER(void )//void BOARD_BRAKE_IRQ_HANDLER(void)
{
  
    /* Clear external interrupt flag. */
    GPIO_PortClearInterruptFlags(BRAKE_KEY_GPIO, 1U << BRAKE_KEY_GPIO_PIN );
    /* Change state of button. */
    A_POWER_OUTPUT =0;
	motor_ref.abc_numbers = 1;
    motor_ref.motor_run = 0;
	PRINTF("interrupte has happed  \r\n");
	                  
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

#endif 
#if 0
void START_KEY_IRQ_HANDLER(void )
{
    uint8_t i;
    /* Clear external interrupt flag. */
    GPIO_PortClearInterruptFlags(START_KEY_GPIO, 1U << START_KEY_GPIO_PIN );
    /* Change state of button. */
    if(GPIO_PinRead(START_KEY_GPIO,25)== 1)
    {
        DelayMs(5);
        if(GPIO_PinRead(START_KEY_GPIO,25)== 1)
        {
            i++;
            if(i == 1)
            {
              motor_ref.motor_run =1 ;
              motor_ref.power_on = 1;
            }
        	else
        	{
                
                motor_ref.motor_run = 0;
                motor_ref.power_on =0;
                motor_ref.stop_numbers=1;
                i=0;
        	}
        }
    }
	PRINTF("motor_run or \r\n");
	                  
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}


#endif 



/*******************************************************************************************/


