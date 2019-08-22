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
static void vTaskLED(void *pvParameters);

static void vTaskDIR(void *pvParameters);

static void vTaskBLDC(void *pvParameters);
static void vTaskMsgPro(void *pvParameters);
//static void vTaskStart(void *pvParameters);
 

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
static TaskHandle_t xHandleTaskBLDC = NULL;


static TaskHandle_t xHandleTaskMsgPro = NULL;
//static TaskHandle_t xHandleTaskStart = NULL;
static QueueHandle_t xQueue1 = NULL;
static QueueHandle_t xQueue2 = NULL;
static QueueHandle_t xQueue3 = NULL;
static QueueHandle_t xQueue4 = NULL;



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
  uint8_t ucCount,ucBLDC;
  uint8_t ch[8]={0};
  
  /* ��ʼ���ṹ��ָ�� */
	ptMsg = &g_tMsg;
	
	/* ��ʼ������ */
	ptMsg->ucMessageID = 0;
	ptMsg->ulData[0] = 0;
	ptMsg->usData[0] = 0;
  while(1)
    {
      
                ptMsg->ucMessageID ++;
		ptMsg->ulData[2] ++;
		ptMsg->usData[2] ++;
                ptMsg->ulData[3] ++;
		ptMsg->usData[3] ++;
   
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
          if(ch[1]== 0x32 ) 
            if(ch[2] == 0x33 ) 
         
        {
            ucCount= 50;
            /* ?��???��?����D���騺y?Y��?��?1????��?����D?����?��?�̨���y10??����?��?��?? */
            if( xQueueSend(xQueue1,
                     (void *) &ucCount,
                     (TickType_t)10) != pdPASS )
            {
             /* ����ʧ�ܣ���ʹ�ȴ���10��ʱ�ӽ��� */
	  			 printf("vTaskMsgRro����ʹ�ȴ���10��ʱ�ӽ���\r\n");
            }
            else
            {
             /* ���ͳɹ� */
				printf("TaskMsgRro����xQueue1�������ݳɹ�\r\n");							
            }
            /*************************xQueue2************************************/
            /* ?��???��?����D���騺y?Y��?��?1????��?����D?����?��?�̨���y10??����?��?��?? */
            ucBLDC=0x0a;
            if( xQueueSend(xQueue2,
                     (void *)&ucBLDC,
                     (TickType_t)10) != pdPASS )
            {
             /* ����ʧ�ܣ���ʹ�ȴ���10��ʱ�ӽ��� */
	   	printf("vTaskDIR����ʹ�ȴ���20��ʱ�ӽ���\r\n");
            }
            else
            {
             /* ���ͳɹ� */
		 
                  printf("DIR send ����xQueue2�������ݳɹ�\r\n");	
            }
        
            /******************xQueue3************************/
      
#if 1
            if( xQueueSend(xQueue3,
                     (void *)&ptMsg,
                     (TickType_t)10) != pdPASS )
            {
             /* ����ʧ�ܣ���ʹ�ȴ���10��ʱ�ӽ��� */
	   	printf("vTaskBLDC����ʹ�ȴ���10��ʱ�ӽ���\r\n");
            }
            else
            {
             /* ���ͳɹ� */
	       printf("BLDC send ����xQueue3�������ݳɹ�\r\n");						
            }
#endif 
            ucBLDC++;
            if( xQueueSend(xQueue4,
                     (void *)&ucBLDC,
                     (TickType_t)10) != pdPASS )
            {
             /* ����ʧ�ܣ���ʹ�ȴ���10��ʱ�ӽ��� */
	   	printf("vTaskLED����ʹ�ȴ���10��ʱ�ӽ���\r\n");
            }
            else
            {
             /* ���ͳɹ� */
	       printf("vTaskMsgLED ����xQueue4�������ݳɹ�\r\n");						
            }
         }
        
        
#endif 
	vTaskDelay(300);
    }
}
/*********************************************************************************************************
*	�� �� ��: vTaskLED
*	����˵��: ʹ�ú���xQueueReceive��������vTaskTaskUserIF���͵���Ϣ��������(xQueue2)	
*	��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ��: 2  
*********************************************************************************************************/
#if 1
static void vTaskLED(void *pvParameters)
{
   
  BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100); /* �������ȴ�ʱ��Ϊ5ms */
	uint8_t ucQueueMsgValue;
    while(1)
    {
	 
      
      printf("vTaskLED-2 \r\n");
              xResult = xQueueReceive(xQueue4,                   /* ???��?����D??���� */
		                        (void *)&ucQueueMsgValue,  /* ��?���?����?��?��?��y?Y��?��?��?ucQueueMsgValue?D */
		                        (TickType_t)xMaxBlockTime);/* ����??������?����?? */
		
		if(xResult == pdPASS)
		{
			/* �ɹ����գ���ͨ�����ڽ����ݴ�ӡ���� */
			printf("���յ���Ϣ��������vTaskLED = %d\r\n", ucQueueMsgValue);
			
			LED1 = !LED1;
			LED2 = !LED2;
                        
                
                   }
		else
		{
			 LED1 = !LED1;
                        // DelayMs(500);
                        // LED1=1;
                        // DelayMs(500);
			
		}
		//vTaskDelay(200);//taskYIELD();              //����ʱ��Ƭ����CPU�ø�ͬ���ȼ�����������
    }

  

}
#endif 

/*********************************************************************************************************
*	�� �� ��: vTaskDIR
*	����˵��: ʹ�ú���xQueueReceive��������vTaskTaskUserIF���͵���Ϣ��������(xQueue2)	
*	��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ��: 3  
*********************************************************************************************************/
static void vTaskDIR(void *pvParameters)
{
    //MSG_T *ptMsg;
    BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100); /* �������ȴ�ʱ��Ϊ300ms */
	uint8_t ucQueueMsgValue;
	
    while(1)
    {           printf("vTaskDIR-3 \r\n");
		xResult = xQueueReceive(xQueue2,                   /* ��Ϣ���о�� */
		                       (void *)&ucQueueMsgValue,  /* �洢���յ������ݵ�����ucQueueMsgValue�� */
		                       (TickType_t)xMaxBlockTime);/* ��������ʱ�� */
		
		if(xResult == pdPASS)
		{
			/* �ɹ����գ���ͨ�����ڽ����ݴ�ӡ���� */
                      printf("���յ���Ϣ��������vTaskDIR = %d\r\n",ucQueueMsgValue );
			//printf("���յ���Ϣ��������ptMsg->ucMessageID = %d\r\n", ptMsg->ucMessageID);
			//printf("���յ���Ϣ��������ptMsg->ulData[3] = %d\r\n", ptMsg->ulData[3]);
			//printf("���յ���Ϣ��������ptMsg->usData[3] = %d\r\n", ptMsg->usData[3]);
			Dir = Dir;
		}
		else
		{
			/* ��ʱ */
			LED2 =!LED2;
		}
		//taskYIELD();              //����ʱ��Ƭ����CPU�ø�ͬ���ȼ�����������
    } 
   
}

/*********************************************************************************************************
 *
 *	�� �� ��: vTaskBLDC
 *	����˵��: ʹ�ú���xQueueReceive��������vTaskTaskUserIF���͵���Ϣ��������(xQueue2)	
 *	��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
 *	�� �� ֵ: ��
 *   �� �� ��: 4 
 *
*********************************************************************************************************/
static void vTaskBLDC(void *pvParameters)
{
    MSG_T *ptMsg;
    BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100); /* �������ȴ�ʱ��Ϊ300ms */
	//uint8_t ucQueueMsgValue;
	
    while(1)
    {           printf("vTaskDIR-4 \r\n");
		xResult = xQueueReceive(xQueue3,                   /* ��Ϣ���о�� */
		                       (void *)&ptMsg,  /* �洢���յ������ݵ�����ucQueueMsgValue�� */
		                       (TickType_t)xMaxBlockTime);/* ��������ʱ�� */
		
		if(xResult == pdPASS)
		{
			/* �ɹ����գ���ͨ�����ڽ����ݴ�ӡ���� */
			printf("���յ���Ϣ��������vTaskBLDC = %d\r\n",  ptMsg->ucMessageID);
			
			
		}
		else
		{
			/* ��ʱ */
			LED2 =!LED2;
		}
		vTaskDelay(300);//taskYIELD();              //����ʱ��Ƭ����CPU�ø�ͬ���ȼ�����������
    } 
    

}


/**********************************************************************************************************
*	�� �� ��: vTaskMsgPro
*	����˵��: ʹ�ú���xQueueReceive��������vTaskTaskUserIF���͵���Ϣ��������(xQueue1)
*	��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ��: 5  
**********************************************************************************************************/

static void vTaskMsgPro(void *pvParameters)
{
    BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(200); /* �������ȴ�ʱ��Ϊ300ms */
	uint8_t ucQueueMsgValue;
	
    while(1)
    {
	       printf("vTaskMSG-5 \r\n");
               xResult = xQueueReceive(xQueue1,                   /* ��Ϣ���о�� */
		                        (void *)&ucQueueMsgValue,  /* �洢���յ������ݵ�����ucQueueMsgValue�� */
		                        (TickType_t)xMaxBlockTime);/* ��������ʱ�� */
		
		if(xResult == pdPASS)
		{
			/* �ɹ����գ���ͨ�����ڽ����ݴ�ӡ���� */
			printf("vucQueueMsgValue = %d\r\n", ucQueueMsgValue);
		}
		else
		{
			/* ��ʱ */
			LED2 =0;
                        LED1=0;
		}
		//taskYIELD();              //����ʱ��Ƭ����CPU�ø�ͬ���ȼ�����������
    }

}
#if 0
/*********************************************************************************************************
*
*	�� �� ��: vTaskStart
*	����˵��: ��������Ҳ����������ȼ�����������������ɨ�衣
*	��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ��: 6  (���ȼ����)
*
*********************************************************************************************************
*/
static void vTaskStart(void *pvParameters)
{

     MSG_T   *ptMsg;
	//uint8_t ch;
	uint8_t ucCount = 0;
	uint8_t ucKeyCode;
    uint16_t sampleMask;
	
	/* ��ʼ���ṹ��ָ�� */
	ptMsg = &g_tMsg;
	
	/* ��ʼ������ */
	ptMsg->ucMessageID = 0;
	ptMsg->ulData[0] = 0;
	ptMsg->usData[0] = 0;
	while(1)
    {
		printf("vTaskStart-6 \r\n");
                ucKeyCode = KEY_Scan(0);
		if (ucKeyCode != KEY_UP)
		{
			switch (ucKeyCode)
			{
				/* K1������ ��ӡ����ִ����� */
				case START_PRES:			 
					ptMsg->ucMessageID ++;
					ptMsg->ulData[2] = 0x0a;
					ptMsg->usData[2] = 0x0b;
					
					/* ʹ����Ϣ����ʵ��ָ������Ĵ��� */
					if(xQueueSend(xQueue3,                  /* ��Ϣ���о�� */
						     (void *) &ptMsg,           /* ���ͽṹ��ָ�����ptMsg�ĵ�ַ */
						     (TickType_t)10) != pdPASS )
					{
						/* ����ʧ�ܣ���ʹ�ȴ���10��ʱ�ӽ��� */
						printf("K6�����£���xQueue2��������ʧ�ܣ���ʹ�ȴ���10��ʱ�ӽ���\r\n");
					}
					else
					{
						/* ���ͳɹ� */
						printf("K6�����£���xQueue2�������ݳɹ�\r\n");						
					}
				
					break;
				
				/* K2�����£���xQueue1�������� */
				case DIR_PRES:
					ucCount ++ ;
				 
					/* ����Ϣ���з����ݣ������Ϣ�������ˣ��ȴ�10��ʱ�ӽ��� */
					if( xQueueSend(xQueue1,
								   (void *) &ucCount,
								   (TickType_t)10) != pdPASS )
					{
						/* ����ʧ�ܣ���ʹ�ȴ���10��ʱ�ӽ��� */
						printf("K2�����£���xQueue1��������ʧ�ܣ���ʹ�ȴ���10��ʱ�ӽ���\r\n");
					}
					else
					{
						/* ���ͳɹ� */
						printf("K2�����£���xQueue1�������ݳɹ�\r\n");						
					}
					break;
				
				/* K3�����£���xQueue2�������� */
				case DIGITAL_ADD_PRES:
					ptMsg->ucMessageID++;
					ptMsg->ulData[0]++;;
					ptMsg->usData[0]++;
					
					/* ʹ����Ϣ����ʵ��ָ������Ĵ��� */
					if(xQueueSend(xQueue2,                  /* ��Ϣ���о�� */
								 (void *) &ptMsg,           /* ���ͽṹ��ָ�����ptMsg�ĵ�ַ */
								 (TickType_t)10) != pdPASS )
					{
						/* ����ʧ�ܣ���ʹ�ȴ���10��ʱ�ӽ��� */
						printf("K3�����£���xQueue2��������ʧ�ܣ���ʹ�ȴ���10��ʱ�ӽ���\r\n");
					}
					else
					{
						/* ���ͳɹ� */
						printf("K3�����£���xQueue2�������ݳɹ�\r\n");						
					}
					break;
				case HALL_PRES :
                     
				    //UART_ReadBlocking(DEMO_UART, &ch, 1);
                                       //UART_WriteBlocking(DEMO_UART, &ch, 1);
					break;
				case WHEEL_PRES :
					ptMsg->ucMessageID++;
					ptMsg->ulData[1]++;;
					ptMsg->usData[1]++;
					
					/* ʹ����Ϣ����ʵ��ָ������Ĵ��� */
					if(xQueueSend(xQueue2,                  /* ��Ϣ���о�� */
						     (void *) &ptMsg,           /* ���ͽṹ��ָ�����ptMsg�ĵ�ַ */
						     (TickType_t)10) != pdPASS )
					{
						/* ����ʧ�ܣ���ʹ�ȴ���10��ʱ�ӽ��� */
						printf("K3�����£���xQueue2��������ʧ�ܣ���ʹ�ȴ���10��ʱ�ӽ���\r\n");
					}
					else
					{
						/* ���ͳɹ� */
						printf("K3�����£���xQueue2�������ݳɹ�\r\n");						
					}
					//taskYIELD();              //����ʱ��Ƭ����CPU�ø�ͬ���ȼ�����������
					break;
				case WIPERS_PRES :
					ptMsg->ucMessageID++;
					ptMsg->ulData[2]++;;
					ptMsg->usData[2]++;
					
					/* ʹ����Ϣ����ʵ��ָ������Ĵ��� */
					if(xQueueSend(xQueue3,                  /* ��Ϣ���о�� */
						     (void *) &ptMsg,           /* ���ͽṹ��ָ�����ptMsg�ĵ�ַ */
						     (TickType_t)10) != pdPASS )
					{
						/* ����ʧ�ܣ���ʹ�ȴ���10��ʱ�ӽ��� */
						printf("K6�����£���xQueue2��������ʧ�ܣ���ʹ�ȴ���10��ʱ�ӽ���\r\n");
					}
					else
					{
						/* ���ͳɹ� */
						printf("K6�����£���xQueue2�������ݳɹ�\r\n");						
					}
					//taskYIELD();              //����ʱ��Ƭ����CPU�ø�ͬ���ȼ�����������
					break;
				case AIR_PRES :
					break;
				
				/* �����ļ�ֵ������ */
				default:
				           
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

			     	}
       
					break;
			}
		}
	  vTaskDelay(20);
    }
}
#endif
/********************************************************************************************************
*	�� �� ��: AppTaskCreate
*	����˵��: ����Ӧ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************/
static void AppTaskCreate (void)
{
    xTaskCreate( vTaskUSART,   	/* ������  */
                 "vTaskUserIF",     	/* ������    */
                 512,               	/* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,              	/* �������  */
                 1,                 	/* �������ȼ� ���*/
                 &xHandleTaskUSART );  /* ������  */
#if 1	
	xTaskCreate( vTaskLED,    		/* ������  */
                 "vTaskLED",  		/* ������    */
                 512,         		/* stack��С����λword��Ҳ����4�ֽ� */
                 NULL,        		/* �������  */
                 2,           		/* �������ȼ�*/
                 &xHandleTaskLED ); /* ������  */
#endif 

	xTaskCreate( vTaskDIR,    		/* ������  */
                 "vTaskDIR",  		/* ������    */
                 1024,         		/* stack��С����λword��Ҳ����4�ֽ� */
                 NULL,        		/* �������  */
                 3,           		/* �������ȼ�*/
                 &xHandleTaskDIR ); /* ������  */

	xTaskCreate( vTaskBLDC,    		/* ������  */
                 "vTaskBLDC",  		/* ������    */
                 1024,         		/* stack��С����λword��Ҳ����4�ֽ� */
                 NULL,        		/* �������  */
                 4,           		/* �������ȼ�*/
                 &xHandleTaskBLDC ); /* ������  */
	
	xTaskCreate( vTaskMsgPro,     		/* ������  */
                 "vTaskMsgPro",   		/* ������    */
                 1024,             		/* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,           		/* �������  */
                 6,               		/* �������ȼ�*/
                 &xHandleTaskMsgPro );  /* ������  */
	
#if 0	
	xTaskCreate( vTaskStart,     		/* ������  */
                 "vTaskStart",   		/* ������    */
                 1024,            		/* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,           		/* �������  */
                 6,              		/* �������ȼ� ���*/
                 &xHandleTaskStart );   /* ������  */
#endif 
}
/********************************************************************
*	�� �� ��: AppObjCreate
*	����˵��: ��������ͨ�Ż���
*	��    ��: ��
*	�� �� ֵ: ��
********************************************************************/
static void AppObjCreate (void)
{
  
    /* ����10��uint8_t����Ϣ���� */
	xQueue1 = xQueueCreate(8, sizeof(uint8_t));
    if( xQueue1 == 0 )
    {
       printf("xQueuel set up fail!!!!"); 
       /* û�д����ɹ����û�������������봴��ʧ�ܵĴ������ */
    }
     /* ����10���洢ָ���������Ϣ���У�����CM3/CM4�ں���32λ����һ��ָ�����ռ��4���ֽ� */
	xQueue2 = xQueueCreate(8, sizeof(uint8_t));
    if( xQueue2 == 0 )
    {
         printf("xQueue2 set up fail!!!!"); /* û�д����ɹ����û�������������봴��ʧ�ܵĴ������ */
    }
	/* ����10���洢ָ���������Ϣ���У�����CM3/CM4�ں���32λ����һ��ָ�����ռ��4���ֽ� */
	xQueue3 = xQueueCreate(8, sizeof(struct Msg *));
    if( xQueue3 == 0 )
    {
         printf("xQueue3 set up fail!!!!"); /* û�д����ɹ����û�������������봴��ʧ�ܵĴ������ */
    }
    
    xQueue4 = xQueueCreate(8, sizeof(struct Msg *));
    if( xQueue4 == 0 )
    {
         printf("xQueue3 set up fail!!!!"); /* û�д����ɹ����û�������������봴��ʧ�ܵĴ������ */
    }
	//taskYIELD();              //����ʱ��Ƭ����CPU�ø�ͬ���ȼ�����������
	
}
void vApplicationStackOverflowHook(TaskHandle_t xTask, char * pcTaskName)
{
   printf("BLDC overflow is fail = %s \r\n",pcTaskName);

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
#if 0
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

