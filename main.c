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


/*******************************************************************************
 * Globals
 ******************************************************************************/
/* Logger queue handle */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Application API */
 
static void vTaskUSART(void *pvParameters);
static void vTaskCOTL(void *pvParameters);

static void vTaskBLDC(void *pvParameters);
#if 0
static void vTaskBLDC(void *pvParameters);
static void vTaskMsgPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
#endif 

static void AppTaskCreate (void);
static void AppObjCreate (void);

/*
**********************************************************************************************************
											��������
**********************************************************************************************************
*/
static TaskHandle_t xHandleTaskUSART = NULL;
static TaskHandle_t xHandleTaskLED = NULL;
static TaskHandle_t xHandleTaskDIR = NULL;

static QueueHandle_t xQueue1 = NULL;
static QueueHandle_t xQueue2 = NULL;
static QueueHandle_t xQueue3 = NULL;



typedef struct Msg
{
	uint8_t  ucMessageID;
	uint8_t usData[8];
	uint8_t ulData[8];
}MSG_T;

MSG_T   g_tMsg; /* ����һ���ṹ��������Ϣ���� */
//uint8_t ch[8];

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
    
    HallSensor_GetPinState();
    ADC_CADC_Init();
	
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
  
  MSG_T *ptMsg;
  uint8_t ucCount;
  uint8_t ch[8];
  const TickType_t xMaxBlockTime = pdMS_TO_TICKS(300); /* �������ȴ�ʱ��Ϊ5ms */
  /* ��ʼ���ṹ��ָ�� */
	ptMsg = &g_tMsg;
	
	/* ��ʼ������ */
	ptMsg->ucMessageID = 0;
	ptMsg->ulData[0] = 0;
	ptMsg->usData[0] = 0;
  while(1)
    {
      
                ptMsg->ucMessageID ++;
		ptMsg->ulData[0]++ ;
		ptMsg->usData[0] ++;
   
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
        
        
#if 1
          
         if(ch[0] == 0x31)
         
         
        {
            
            /* ?��???��?����D���騺y?Y��?��?1????��?����D?����?��?�̨���y10??����?��?��?? */
            if( xQueueSend(xQueue2,
                     (void *) &ptMsg,
                     (TickType_t)10) != pdPASS )
            {
             /* ����ʧ�ܣ���ʹ�ȴ���10��ʱ�ӽ��� */
	   printf("K6�����£���xQueue2��������ʧ�ܣ���ʹ�ȴ���10��ʱ�ӽ���\r\n");
            }
            else
            {
             /* ���ͳɹ� */
		printf("K6�����£���xQueue1�������ݳɹ�\r\n");							
            }
        }
        if(ch[3] == 0x34)
            {

               ucCount++;
			   /*�����3 ��������*/
              if( xQueueSend(xQueue3,
                         (void *)&ucCount,
                         (TickType_t)10) != pdPASS )
                {
                 /* ����ʧ�ܣ���ʹ�ȴ���10��ʱ�ӽ��� */
                 printf("vTaskDIR����ʹ�ȴ���10��ʱ�ӽ���\r\n");
                }
                else
                {
                 /* ���ͳɹ� */
                      printf("DIR send ����xQueue1�������ݳɹ�\r\n");
                                    
                }

              
              
              
        }
        
#endif 
	vTaskDelay(xMaxBlockTime);
    }
}
/*********************************************************************************************************
*	�� �� ��: vTaskCOTL
*	����˵��: ʹ�ú���xQueueReceive��������vTaskTaskUserIF���͵���Ϣ��������(xQueue2)	
*	��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ��: 2  
*********************************************************************************************************/
static void vTaskCOTL(void *pvParameters)
{
     
	uint8_t ucKeyCode;
    MSG_T *ptMsg;
    BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100); /* �������ȴ�ʱ��Ϊ5ms */
	//uint8_t ucQueueMsgValue;
    while(1)
    {
	      printf("vTaskCOTL-2 \r\n");
		  ucKeyCode = KEY_Scan(0);
          xResult = xQueueReceive(xQueue2,                   	/* ���о�� */
		                        (void *)&ptMsg,  				/*���յ������ݵ�ַ */
		                        (TickType_t)xMaxBlockTime);		/* ����??������?����?? */
		
		if(xResult == pdPASS)
		{
			/* �ɹ����գ���ͨ�����ڽ����ݴ�ӡ���� */
			printf("���յ���Ϣ��������vTaskLED = %d\r\n", ptMsg->ucMessageID);
			printf("���յ���Ϣ��������ulData[0] = %d\r\n", ptMsg->ulData[0]);
            printf("���յ���Ϣ��������vsData[0] = %d\r\n", ptMsg->usData[0]);
			LED1 = !LED1;
			LED2 = !LED2;
                        
                   
		}
		else
		{
			 LED1 = 0;
             LED2=0;
                        
        }
		
		if (ucKeyCode != KEY_UP||xResult ==pdPASS)
		{
			switch (ucKeyCode || ptMsg->ucMessageID)
			{
                          
               case BRAKE_PRES:
			   	break;

			   case START_PRES :
			   	   recoder_number.start_number=1;
				   LED1=0;
			       LED2=0;
			   	break;
			   case DIR_PRES:
			   	break;
				case DIGITAL_ADD_PRES :
					break;
				case DIGITAL_REDUCE_PRES :
					break;
				case DOOR_PRES :
					break;
				case HALL_PRES:
					break;
				case WIPERS_PRES:
                     LED1 =0 ;
					 LED2 =!LED2;
				
				break;
				
                case AIR_PRES : //PE29
                    LED1 =!LED1;
                    LED2 =!LED2;
				break;
			}
         }  
		vTaskDelay(xMaxBlockTime);            //����ʱ��Ƭ����CPU�ø�ͬ���ȼ�����������
    }

  

}


/*********************************************************************************************************
*	�� �� ��: vTaskBLDC
*	����˵��: ʹ�ú���xQueueReceive��������vTaskTaskUserIF���͵���Ϣ��������(xQueue2)	
*	��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ��: 3  
*********************************************************************************************************/
static void vTaskBLDC(void *pvParameters)
{
   
     uint16_t sampleMask;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(200); /* �������ȴ�ʱ��Ϊ300ms */
	
	
    while(1)
    {       
      printf("vTaskBLDC-3 \r\n");   

	  if(recoder_number.start_number==0)//ɲ��
	  {
         PMW_AllClose_ABC_Channel();
         DelayMs(10U);
         PMW_AllClose_ABC_Channel();
		 PWM_StopTimer(BOARD_PWM_BASEADDR,  kPWM_Control_Module_0);
		 PWM_StopTimer(BOARD_PWM_BASEADDR,  kPWM_Control_Module_1);
		 PWM_StopTimer(BOARD_PWM_BASEADDR,  kPWM_Control_Module_2);
       }
	  else 
	 {
            /**********************adjust frequency ****************************/
			CADC_DoSoftwareTriggerConverter(CADC_BASEADDR, kCADC_ConverterA);
	             /* Wait the conversion to be done. */
	         while (kCADC_ConverterAEndOfScanFlag !=
	               (kCADC_ConverterAEndOfScanFlag & CADC_GetStatusFlags(CADC_BASEADDR)))
	        		{
	        		}

	        /* Read the result value. */
	        if (sampleMask == (sampleMask & CADC_GetSampleReadyStatusFlags(CADC_BASEADDR)))
	        {
                 PWM_Duty =(int16_t)CADC_GetSampleResultValue(CADC_BASEADDR, 1U);
		       PRINTF("%d\t\t",PWM_Duty );
	           //PRINTF("%d\t\t", (int16_t)CADC_GetSampleResultValue(CADC_BASEADDR, 1U));
			   DelayMs(50U);
               PWM_Duty = (uint16_t)((CADC_GetSampleResultValue(CADC_BASEADDR, 1U))/ 330);
	           PRINTF("PWM_Duty = %d\r\n", PWM_Duty);
			   DelayMs(200U);
	            //PRINTF("%d", (int16_t)CADC_GetSampleResultValue(CADC_BASEADDR, 3U));
            }
        	CADC_ClearStatusFlags(CADC_BASEADDR, kCADC_ConverterAEndOfScanFlag);


			/***********Motor Run**************/
            PMW_AllClose_ABC_Channel();
			uwStep = HallSensor_GetPinState();
        	PRINTF("ouread = %d \r\n",uwStep);
        	HALLSensor_Detected_BLDC(uwStep); 

        }
        vTaskDelay(xMaxBlockTime);         
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
	
	xTaskCreate( vTaskCOTL,    									/* ������  */
                 "vTaskCOTL",  									/* ������    */
                 configMINIMAL_STACK_SIZE + 422,         		/* stack��С����λword��Ҳ����4�ֽ� */
                 NULL,        									/* �������  */
                 tskIDLE_PRIORITY+2,           					/* �������ȼ�*/
                 &xHandleTaskLED ); 							/* ������  */
 

	xTaskCreate( vTaskBLDC,    									/* ������  */
                 "vTaskBLDC",  									/* ������    */
                 configMINIMAL_STACK_SIZE + 934,         		/* stack��С����λword��Ҳ����4�ֽ� */
                 NULL,        									/* �������  */
                 tskIDLE_PRIORITY+3,           					/* �������ȼ�*/
                 &xHandleTaskDIR ); 							/* ������  */

}
/********************************************************************
*	�� �� ��: AppObjCreate
*	����˵��: ��������ͨ�Ż���
*	��    ��: ��
*	�� �� ֵ: ��
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
    
     /* ����10���洢ָ���������Ϣ���У�����CM3/CM4�ں���32λ����һ��ָ�����ռ��4���ֽ� */
      xQueue3 = xQueueCreate(10, sizeof(uint8_t));
    if( xQueue3 == 0 )
    {
         printf("xQueue3 set up fail!!!!"); /* û�д����ɹ����û�������������봴��ʧ�ܵĴ������ */
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
     recoder_number.start_number=0;
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}
#endif 

