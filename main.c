/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*System includes.*/
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

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
//#include "usart_edma_rb.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MAX_LOG_LENGTH 20


output_t recoder_number;


static void vTaskUSART(void *pvParameters);
static void vTaskBLDC(void *pvParameters);
static void vTaskCOTL(void *pvParameters);


static void AppTaskCreate (void);
static void AppObjCreate (void);

/*
**********************************************************************************************************
											��������
**********************************************************************************************************
*/
static TaskHandle_t xHandleTaskUSART = NULL;
static TaskHandle_t xHandleTaskBLDC = NULL;
static TaskHandle_t xHandleTaskCOTL = NULL;

static QueueHandle_t xQueue1 = NULL;
static QueueHandle_t xQueue2 = NULL;



typedef struct Msg
{
	uint8_t  ucMessageID;
	uint8_t  usData[8];
	
}MSG_T;

MSG_T   g_tMsg; /* ����һ���ṹ��������Ϣ���� */




/*******************************************************************************
 *
 * Code
 *
 * @brief Main function
 *
******************************************************************************/
int main(void)
{
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    
   
    LED_Init();
    KEY_Init();
    DelayInit();
    HALL_Init();
    
    HallSensor_GetPinState();
    OUTPUT_Fucntion_Init();
    ADC_CADC_Init();
    ABC_POWER_OUTPUT_Init();
    /* Set the PWM Fault inputs to a low value */
    PWM_BLDC_Init();
    
     //BOARD_InitDebugConsole();
     USART_POLLING_Init();
     
   //  POLLING_USART_Init();
    /* Initialize logger for 10 logs with maximum lenght of one log 20 B */
    /* �������� */
	AppTaskCreate();
    
    /* ��������ͨ�Ż��� */
	AppObjCreate();// WT.EDIT log_init(10, MAX_LOG_LENGTH);
    
    vTaskStartScheduler();
    for (;;)
        ;
}

/******************************************************************************************************
*	�� �� ��: vTaskTaskUSART
*	����˵��: �ӿ���Ϣ����
*	��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ��: 1  (��ֵԽС���ȼ�Խ�ͣ�������ȼ�)
********************************************************************************************************/
static void vTaskUSART(void *pvParameters)
{
  //TickType_t xLastWakeTime;
  //const TickType_t xFrequency = 300;
  MSG_T *ptMsg;
  uint8_t i;
  uint8_t ch[8];
  const TickType_t xMaxBlockTime = pdMS_TO_TICKS(300); /* �������ȴ�ʱ��Ϊ5ms */
  /* ��ʼ���ṹ��ָ�� */
	ptMsg = &g_tMsg;
	
	/* ��ʼ������ */
	ptMsg->ucMessageID = 0;
	ptMsg->usData[0] = 0;
  while(1)
    {
        UART_ReadBlocking(DEMO_UART, ch, 1);
        UART_ReadBlocking(DEMO_UART, ch, 8);
       
        UART_WriteBlocking(DEMO_UART, ch, 8);
        printf("ch output .\r\n");
        printf("ch=[7] %#x \r\n",ch[7]);
        printf("ch=[6] %#x \r\n",ch[6]);
        printf("ch=[5] %#x \r\n",ch[5]);
        printf("ch=[4] %#x \r\n",ch[4]);
        printf("ch=[3] %#x \r\n",ch[3]);
        printf("ch=[2] %#x \r\n",ch[2]);
        printf("ch=[1] %#x \r\n",ch[1]);
        printf("ch=[0] %#x \r\n",ch[0]);

		for(i=0;i<8;i++)
		{
          
		  ptMsg->usData[i]=ch[i];
		  printf("ptMsg->usDta[i]= %#x \r\n",ptMsg->usData[i]);

		}
        
        if(ch[0] == 0x31) //'1' = 0x31
         {
            /* ����Ϣ���з������ݣ������Ϣ���������ȴ�10��ʱ�ӽ��� */
		    ptMsg->ucMessageID = ch[1];
            if( xQueueSend(xQueue2,
                     (void *) &ptMsg,
                     (TickType_t)10) != pdPASS )
            {
             /* ����ʧ�ܣ���ʹ�ȴ���10��ʱ�ӽ��� */
	       		printf("��xQueue-2��������ʧ��??????????\r\n");
            }
            else
            {
             /* ���ͳɹ� */
		      printf("��������xQueue-2�ɹ���\r\n");							
            }
        }
		
		vTaskDelay(xMaxBlockTime);
	 //vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

/*********************************************************************************************************
*	�� �� ��: vTaskBLDC
*	����˵��: ʹ�ú���xQueueReceive��������vTaskTaskUserIF���͵���Ϣ��������(xQueue2)	
*	��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ��: 2  
*********************************************************************************************************/
static void vTaskBLDC(void *pvParameters)
{
    
    TickType_t xLastWakeTime;
	
	const TickType_t xFrequency = 100;
    xLastWakeTime = xTaskGetTickCount();
    volatile uint16_t pwm_f=0;
	uint16_t sampleMask;
	BaseType_t xResult;
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100); /* �������ȴ�ʱ��Ϊ300ms */
	uint8_t ucQueueMsgValue;
	while(1)
    {       
      printf("vTaskBLDC-2 \r\n");  
	  #if 1
	  xResult = xQueueReceive(xQueue1,                   	/* ��Ϣ���о��3 */
		                        (void *)&ucQueueMsgValue,  	/* �洢�����յ����ݱ���ucQueueMsgValue */
		                        (TickType_t)xMaxBlockTime);	/*��������ʱ��*/
		
		if(xResult == pdPASS)
		{
			/* �������ݳɹ� */
          printf("vTaskBLDC ConmessageID = %#x\r\n",ucQueueMsgValue);
		}
		else
		{
			/* ��ʱ */
			LED1= !LED1;
			
			
		}
	
 #endif     
	  if((ucQueueMsgValue==0)|| (recoder_number.break_f ==1))//ɲ��
	  {
         taskENTER_CRITICAL(); //�����ٽ�״̬
		 PMW_AllClose_ABC_Channel();
         DelayMs(10U);
         PMW_AllClose_ABC_Channel();
		 PWM_StopTimer(BOARD_PWM_BASEADDR,  kPWM_Control_Module_0);
		 PWM_StopTimer(BOARD_PWM_BASEADDR,  kPWM_Control_Module_1);
		 PWM_StopTimer(BOARD_PWM_BASEADDR,  kPWM_Control_Module_2);
		 taskEXIT_CRITICAL(); //�˳��ٽ�״̬
		 printf("Break is OK $$$$$$$$$$$$$\r\n");
       }
	 else 
	 {
             printf("Motor Run is OK !!!!\r\n");
#if 1
	       /**********************adjust frequency ****************************/
			{
            
		    CADC_DoSoftwareTriggerConverter(CADC_BASEADDR, kCADC_ConverterA);
	             /* Wait the conversion to be done. */
	         while (kCADC_ConverterAEndOfScanFlag !=
	               (kCADC_ConverterAEndOfScanFlag & CADC_GetStatusFlags(CADC_BASEADDR)))
	        {
	        }

	        /* Read the result value. */
	        if (sampleMask == (sampleMask & CADC_GetSampleReadyStatusFlags(CADC_BASEADDR)))
	        {
              //  pwm_f =(int16_t)CADC_GetSampleResultValue(CADC_BASEADDR, 1U);
		      //   PRINTF("%d\t\t",pwm_f );
	           
			 // DelayMs(100U);
              pwm_f = (uint16_t)((CADC_GetSampleResultValue(CADC_BASEADDR, 1U))/ 330);
	        //  PRINTF("PWM_Duty = %d\r\n",pwm_f);
			 //  DelayMs(200U);
	           
            }
        CADC_ClearStatusFlags(CADC_BASEADDR, kCADC_ConverterAEndOfScanFlag);

     	}

            pwm_f = (uint16_t)((CADC_GetSampleResultValue(CADC_BASEADDR, 1U))/ 330);
			if(ucQueueMsgValue== 0x11) 
            {
                  Dir = Dir ;
				  printf("Dir = Dir is OK !!!!\r\n");
			}
			else if(ucQueueMsgValue== 0x10) 
			{
			    Dir = -Dir;
				printf("Dir = - Dir is OK !!!!\r\n");
			}
			/***********Motor Run**************/
            PMW_AllClose_ABC_Channel();
			uwStep = HallSensor_GetPinState();
        	PRINTF("ouread = %d \r\n",uwStep);
        	HALLSensor_Detected_BLDC(uwStep,pwm_f); 
           
#endif
        }
       vTaskDelayUntil(&xLastWakeTime, xFrequency); // vTaskDelay(xMaxBlockTime);         
     }  
 } 

/*********************************************************************************************************
*
*	�� �� ��: vTaskCOTL
*	����˵��: ���������������ְ���������
*	��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ��: 3  
*
*********************************************************************************************************/
static void vTaskCOTL(void *pvParameters)
{
   
	//TickType_t xLastWakeTime;
	//const TickType_t xFrequency = 200;
    MSG_T  *ptMsg; 
	uint8_t ucKeyCode=0,abc_s=0;
   
    BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100); /* �������ȴ�ʱ��Ϊ5ms */
	uint8_t ucControl=0;

	while(1)
    {
	      printf("vTaskCOTL-3 \r\n");
		 
		  ucKeyCode = KEY_Scan(0);
          xResult = xQueueReceive(xQueue2,                   	/* ���о�� */
		                        (void *)&ptMsg,  				/*���յ������ݵ�ַ */
		                        (TickType_t)xMaxBlockTime);		/* ��������ʱ��*/
		
				if(xResult == pdPASS)
				{
					/* �ɹ����գ���ͨ�����ڽ����ݴ�ӡ���� */
		          printf("���յ���Ϣ��������vTaskCOTL = %#x\r\n", ptMsg->ucMessageID);
		          printf("���յ���Ϣ��������vsData[0] = %#x\r\n", ptMsg->usData[0]);
				  LED1 = !LED1;
				  LED2 = !LED2;
				 
				  if(ptMsg->ucMessageID==0x32)
				  {
                       recoder_number.dir_change ++;
		          if(recoder_number.dir_change == 1)
		          {
                     ucControl =1;
					 recoder_number.break_f =0;
				  }
				  else 
				  {
                     ucControl = 0;
					 recoder_number.dir_change =0;
				  }
                
		         	/* ����Ϣ���з������� */
					if( xQueueSend(xQueue1,
								   (void *) &ucControl,
								   (TickType_t)10) != pdPASS )
					{
						/* ��������ʧ�ܣ��ȴ�10������ */
						printf("xQueue1 is fail?????????\r\n");
					}
					else
					{
						/* �������ݳɹ� */
						printf("xQueue1 is OK \r\n");						
					}

				  }
				  
				}
				else
				{
					 LED1 = 0;
			         LED2 = 0;
		        }
				
			if(ucKeyCode !=KEY_UP)
				
			{
               switch(ucKeyCode )//if(ptMsg->ucMessageID == 0x32)
                 { 
                
                  case ABC_POWER_PRES :

				  PRINTF("ABC_PRES key \r\n");
				     abc_s++;
				  if(abc_s ==1)
				  	{
					     A_POWER_OUTPUT =1;
					     B_POWER_OUTPUT =1;
						 C_POWER_OUTPUT =1;
					  	 LED1= !LED1;
						 
				  	}
					else 
					{
                         A_POWER_OUTPUT =0;
					     B_POWER_OUTPUT =0;
						 C_POWER_OUTPUT =0;
						 abc_s =0 ;
						 LED2= !LED2;

					}
				  	break;



				 case START_PRES:
                   PRINTF("START_PRES key \r\n");
				  recoder_number.dir_change ++;
		          if((recoder_number.dir_change == 1)||(recoder_number.break_f ==1))
		          {
                     ucControl =1;
					 recoder_number.break_f =0;
					 A_POWER_OUTPUT =1;
					     B_POWER_OUTPUT =1;
						 C_POWER_OUTPUT =1;
				  }
				  else 
				  {
                     ucControl = 0;
					 recoder_number.dir_change =0;
					  A_POWER_OUTPUT =0;
					     B_POWER_OUTPUT =0;
						 C_POWER_OUTPUT =0;
					
				  }
                   #if 1
		         	/* ����Ϣ���з������� */
					if( xQueueSend(xQueue1,
								   (void *) &ucControl,
								   (TickType_t)10) != pdPASS )
					{
						/* ��������ʧ�ܣ��ȴ�10������ */
						printf("START_PRES is fail?????????\r\n");
					}
					else
					{
						/* �������ݳɹ� */
						printf("START_PRES is OK \r\n");						
					}
                  #endif 
				  break;
				  case DIR_PRES: //3

			    recoder_number.dir_change++;
	  			PRINTF(" DIR_change = %d  \r\n", recoder_number.dir_change);
	  			 if(recoder_number.dir_change == 1)
	   				{
                        ucControl = 0x11;
						Dir = Dir;
						LED1 =1;
				    }
				 else 
				   {
                       ucControl = 0x10;
					   Dir = -Dir;
				       recoder_number.dir_change =0;
					   LED1=0;
				   }
				 #if 0
				 /* ����Ϣ���з������� */
					if( xQueueSend(xQueue1,
								   (void *) &ucControl,
								   (TickType_t)10) != pdPASS )
					{
						/* ��������ʧ�ܣ��ȴ�10������ */
						printf("DIR_PRES is fail?????????\r\n");
					}
					else
					{
						/* �������ݳɹ� */
						printf("DIR_PRES is OK \r\n");						
					}
                  #endif 
           		
			 break;
				
			 case DIGITAL_ADD_PRES ://4
				PRINTF("DIGITAL_ADD_PRES key \r\n");
				 /* ����Ϣ���з������� */
					if( xQueueSend(xQueue1,
								   (void *) &ucControl,
								   (TickType_t)10) != pdPASS )
					{
						/* ��������ʧ�ܣ��ȴ�10������ */
						printf("DIGITAL_ADD_PRES is fail?????????\r\n");
					}
					else
					{
						/* �������ݳɹ� */
						printf("DIGITAL_ADD_PRES is OK \r\n");						
					}
				break;
				
			 case DIGITAL_REDUCE_PRES ://5
			 	PRINTF("DIGITAL_REDUCE_PRES key \r\n");
				break;
				
			 case DOOR_PRES ://6
			 	   PRINTF("DOOR_PRES key \r\n");
			 	   recoder_number.door_number ++ ;
				   if(recoder_number.door_number ==1)
				   {
                       DOOR_OUTPUT =1;//door open
				   }
				   else
				   {
                      DOOR_OUTPUT = 0; //door close
                       recoder_number.door_number=0;
				   }
			 	   
				break;
				   
			 case HALL_PRES://7
			 	PRINTF("HALL_PRES key \r\n");
				break;
			 
			 case WHEEL_PRES : //8
			    PRINTF("WHEEL_PRES key \r\n");
			    break;
				
			 case WIPERS_PRES: //9�����
			  	PRINTF("WIPERS_PRES key \r\n");
				recoder_number.wiper_number ++;
				if(recoder_number.wiper_number ==1)
				{
                   WIPER_OUTPUT_2 = 0;
				   WIPER_OUTPUT_1 = 1;
				}
				else if(recoder_number.wiper_number ==2)
				{
			       WIPER_OUTPUT_1 = 0;   
			       WIPER_OUTPUT_2  = 1;
				   
			    }
				else
			    {
					WIPER_OUTPUT_1 = 0;
					WIPER_OUTPUT_2 = 0;
               		recoder_number.wiper_number =0;    
				}
                 LED1 =0 ;
				 LED2 =!LED2;
			
			 break;
				
	         case AIR_PRES : //10 PE29
	         	PRINTF("AIR_PRES key \r\n");
	            recoder_number.air_number++;
				        
				if(recoder_number.air_number==1)
				{
			           AIR_OUTPUT = 1;
					   LED2=1;
			           DelayMs(500U);
			           LED2=0;
				}
			    else 
			    {
		           AIR_OUTPUT = 0;
				   LED1=1;
		           DelayMs(500U);
		           LED1=0;
		           recoder_number.air_number =0;
				   
			    }
	            LED1 =!LED1;
	            LED2 =!LED2;
               }
			}
               taskYIELD();//   vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
                
}
/********************************************************************************************************
*	�� �� ��: AppTaskCreate
*	����˵��: ����Ӧ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************/
static void AppTaskCreate (void)
{
    xTaskCreate( vTaskUSART,   									/* ������  */
                 "vTaskUserIF",     							/* ������    */
                 configMINIMAL_STACK_SIZE + 166,               	/* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,              							/* �������  */
                 tskIDLE_PRIORITY+1,                 			/* �������ȼ� ���*/
                 &xHandleTaskUSART );  							/* ������  */
	
	xTaskCreate( vTaskBLDC,    									/* ������  */
                 "vTaskBLDC",  									/* ������    */
                 configMINIMAL_STACK_SIZE + 934,         		/* stack��С����λword��Ҳ����4�ֽ� */
                 NULL,        									/* �������  */
                 tskIDLE_PRIORITY+2,           					/* �������ȼ�*/
                 &xHandleTaskBLDC); 							/* ������  */

	xTaskCreate( vTaskCOTL,    									/* ������  */
                 "vTaskCOTL",  									/* ������    */
                 configMINIMAL_STACK_SIZE + 422,         		/* stack��С����λword��Ҳ����4�ֽ� */
                 NULL,        									/* �������  */
                 tskIDLE_PRIORITY+3,           					/* �������ȼ�*/
                 &xHandleTaskCOTL); 							/* ������  */

}
/********************************************************************
 *
 *	�� �� ��: AppObjCreate
 *	����˵��: ��������ͨ�Ż���
 *	��    ��: ��
 *	�� �� ֵ: ��
 *
********************************************************************/
//void log_init(uint32_t queue_length, uint32_t max_log_lenght)
static void AppObjCreate (void)
{
  
    /* ����10��uint8_t����Ϣ���� */
	xQueue1 = xQueueCreate(10, sizeof(uint8_t));
    if( xQueue1 == 0 )
    {
       printf("xQueuel set up fail!!!!"); 
       /* û�д����ɹ����û�������������봴��ʧ�ܵĴ������ */
    }
     /* ����10���洢ָ���������Ϣ���У�����CM3/CM4�ں���32λ����һ��ָ�����ռ��4���ֽ� */
	xQueue2 = xQueueCreate(10, sizeof(struct Msg *));
    if( xQueue2 == 0 )
    {
         printf("xQueue2 set up fail!!!!"); /* û�д����ɹ����û�������������봴��ʧ�ܵĴ������ */
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

	recoder_number.break_f =1;
	                  
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}
#endif 

