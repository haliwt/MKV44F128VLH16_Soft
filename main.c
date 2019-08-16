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
 
static void vTaskTaskUSART(void *pvParameters);
static void vTaskLED(void *pvParameters);
static void vTaskDIR(void *pvParameters);
static void vTaskBLDC(void *pvParameters);


static void vTaskMsgPro(void *pvParameters);
static void vTaskStart(void *pvParameters);

static void AppTaskCreate (void);
static void AppObjCreate (void);
/*
**********************************************************************************************************
											��������
**********************************************************************************************************
*/
static TaskHandle_t xHandleTaskUserIF = NULL;
static TaskHandle_t xHandleTaskLED = NULL;
static TaskHandle_t xHandleTaskLED2 = NULL;
static TaskHandle_t xHandleTaskBLDC = NULL;


static TaskHandle_t xHandleTaskMsgPro = NULL;
static TaskHandle_t xHandleTaskStart = NULL;
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
    ADC_CADC_Init();
	
    /* Set the PWM Fault inputs to a low value */
    PWM_BLDC_Init();
    
     //BOARD_InitDebugConsole();
     USART_EDMA_Init();
     
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
static void vTaskTaskUSART(void *pvParameters)
{

  
  
  uint8_t g_txBuffer[ECHO_BUFFER_LENGTH] ;
 uint8_t g_rxBuffer[ECHO_BUFFER_LENGTH] ;
  
  uart_edma_handle_t g_uartEdmaHandle;
  edma_handle_t g_uartTxEdmaHandle;
  edma_handle_t g_uartRxEdmaHandle;
  
  
  volatile bool rxBufferEmpty            = true;
  volatile bool txBufferFull             = false;
  volatile bool txOnGoing                = false;
  volatile bool rxOnGoing                = false; 

    uart_transfer_t xfer;
    uart_transfer_t sendXfer;
    uart_transfer_t receiveXfer;
    uint8_t g_tipString[]="vTaskUsart\r\n";

	
	/* Send g_tipString out. */
	  xfer.data 	= g_tipString;
	  xfer.dataSize = sizeof(g_tipString) - 1;
	  txOnGoing 	= true;
	  UART_SendEDMA(DEMO_UART, &g_uartEdmaHandle, &xfer);

	  /* Wait send finished */
	  while (txOnGoing)
	  {
	  }
	
	  /* Start to echo. */
	  sendXfer.data 	= g_txBuffer;
	  sendXfer.dataSize    = ECHO_BUFFER_LENGTH;
	  receiveXfer.data	   = g_rxBuffer;
	  receiveXfer.dataSize = ECHO_BUFFER_LENGTH;
  
  while(1)
    {
       g_txBuffer[0] = 2;
       g_txBuffer[1] = 0x0a;
       g_txBuffer[2] = 0x0b;
       g_txBuffer[3] = 0x04;
      
      /* If RX is idle and g_rxBuffer is empty, start to read data to g_rxBuffer. */
        if ((!rxOnGoing) && rxBufferEmpty)
        {
            rxOnGoing = true;
            UART_ReceiveEDMA(DEMO_UART, &g_uartEdmaHandle, &receiveXfer);
            
        }

        /* If TX is idle and g_txBuffer is full, start to send data. */
        if ((!txOnGoing) && txBufferFull)
        {
            txOnGoing = true;
            UART_SendEDMA(DEMO_UART, &g_uartEdmaHandle, &sendXfer);
        }

        /* If g_txBuffer is empty and g_rxBuffer is full, copy g_rxBuffer to g_txBuffer. */
        if ((!rxBufferEmpty) && (!txBufferFull))
        {
             memcpy(g_txBuffer, g_rxBuffer, ECHO_BUFFER_LENGTH);
            rxBufferEmpty = true;
            txBufferFull  = true;
        }
		vTaskDelay(100);
    }
}
/*********************************************************************************************************
*	�� �� ��: vTaskLED
*	����˵��: ʹ�ú���xQueueReceive��������vTaskTaskUserIF���͵���Ϣ��������(xQueue2)	
*	��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ��: 2  
*********************************************************************************************************/
static void vTaskLED(void *pvParameters)
{
   
    
        MSG_T *ptMsg;
	BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(5); /* �������ȴ�ʱ��Ϊ5ms */
	
    while(1)
    {
		xResult = xQueueReceive(xQueue2,                   /* ��Ϣ���о�� */
		                        (void *)&ptMsg,  		   /* �����ȡ���ǽṹ��ĵ�ַ */
		                        (TickType_t)xMaxBlockTime);/* ��������ʱ�� */
		
		
		if(xResult == pdPASS)
		{
			/* �ɹ����գ���ͨ�����ڽ����ݴ�ӡ���� */
			printf("���յ���Ϣ��������ptMsg->ucMessageID = %d\r\n", ptMsg->ucMessageID);
			printf("���յ���Ϣ��������ptMsg->ulData[0] = %d\r\n", ptMsg->ulData[0]);
			printf("���յ���Ϣ��������ptMsg->usData[0] = %d\r\n", ptMsg->usData[0]);
			LED1 = !LED1;
			LED2 = !LED2;
		}
		else
		{
			 LED1 = !LED1;
			
		}
		taskYIELD();              //����ʱ��Ƭ����CPU�ø�ͬ���ȼ�����������
    }

    // TickType_t xLastWakeTime;
    // const TickType_t xFrequency = 10; // 10 x 0.5Khz��Ticks��= 10 * 5ms = 50ms
    // vTaskDelayUntil(&xLastWakeTime,xFrequency);//vTaskDelay(10);  //vTaskSuspend(NULL);

}

/*********************************************************************************************************
*	�� �� ��: vTaskDIR
*	����˵��: ʹ�ú���xQueueReceive��������vTaskTaskUserIF���͵���Ϣ��������(xQueue2)	
*	��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ��: 3  
*********************************************************************************************************/
static void vTaskDIR(void *pvParameters)
{
   
    BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(5); /* �������ȴ�ʱ��Ϊ300ms */
	uint8_t ucQueueMsgValue;
	
    while(1)
    {
		xResult = xQueueReceive(xQueue1,                   /* ��Ϣ���о�� */
		                       (void *)&ucQueueMsgValue,  /* �洢���յ������ݵ�����ucQueueMsgValue�� */
		                       (TickType_t)xMaxBlockTime);/* ��������ʱ�� */
		
		if(xResult == pdPASS)
		{
			/* �ɹ����գ���ͨ�����ڽ����ݴ�ӡ���� */
			printf("���յ���Ϣ��������ucQueueMsgValue = %d\r\n", ucQueueMsgValue);
			Dir = Dir;
		}
		else
		{
			/* ��ʱ */
			LED2 =!LED2;
		}
		taskYIELD();              //����ʱ��Ƭ����CPU�ø�ͬ���ȼ�����������
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
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(200); /* �������ȴ�ʱ��Ϊ200ms */
	
    while(1)
    {
		xResult = xQueueReceive(xQueue3,                   /* ��Ϣ���о�� */
		                        (void *)&ptMsg,  		   /* �����ȡ���ǽṹ��ĵ�ַ */
		                        (TickType_t)xMaxBlockTime);/* ��������ʱ�� */
		
		
		if(xResult == pdPASS)
		{
			/* �ɹ����գ���ͨ�����ڽ����ݴ�ӡ���� */
			printf("���յ���Ϣ��������ptMsg->ucMessageID = %d\r\n", ptMsg->ucMessageID);
			printf("���յ���Ϣ��������ptMsg->ulData[2] = %d\r\n", ptMsg->ulData[2]);
			printf("���յ���Ϣ��������ptMsg->usData[2] = %d\r\n", ptMsg->usData[2]);
			PMW_AllClose_ABC_Channel();
            uwStep = HallSensor_GetPinState();
            PRINTF("ouread = %d \r\n",uwStep);
            HALLSensor_Detected_BLDC(uwStep);
			
		}
		else
		{
			 LED2 = !LED2;
			 LED1 = !LED1;
			
		}
		taskYIELD();              //����ʱ��Ƭ����CPU�ø�ͬ���ȼ�����������
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
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(5); /* �������ȴ�ʱ��Ϊ300ms */
	uint8_t ucQueueMsgValue;
	
    while(1)
    {
		xResult = xQueueReceive(xQueue1,                   /* ��Ϣ���о�� */
		                        (void *)&ucQueueMsgValue,  /* �洢���յ������ݵ�����ucQueueMsgValue�� */
		                        (TickType_t)xMaxBlockTime);/* ��������ʱ�� */
		
		if(xResult == pdPASS)
		{
			/* �ɹ����գ���ͨ�����ڽ����ݴ�ӡ���� */
			printf("���յ���Ϣ��������ucQueueMsgValue = %d\r\n", ucQueueMsgValue);
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
		ucKeyCode = KEY_Scan(0);
		if (ucKeyCode != KEY_UP)
		{
			switch (ucKeyCode)
			{
				/* K1������ ��ӡ����ִ����� */
				case START_PRES:			 
					ptMsg->ucMessageID++;
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
					ucCount++;
				 
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
					taskYIELD();              //����ʱ��Ƭ����CPU�ø�ͬ���ȼ�����������
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
/*
*********************************************************************************************************
*	�� �� ��: AppTaskCreate
*	����˵��: ����Ӧ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{
    xTaskCreate( vTaskTaskUSART,   	/* ������  */
                 "vTaskUserIF",     	/* ������    */
                 512,               	/* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,              	/* �������  */
                 1,                 	/* �������ȼ� ���*/
                 &xHandleTaskUserIF );  /* ������  */
	
	
	xTaskCreate( vTaskLED,    		/* ������  */
                 "vTaskLED",  		/* ������    */
                 512,         		/* stack��С����λword��Ҳ����4�ֽ� */
                 NULL,        		/* �������  */
                 2,           		/* �������ȼ�*/
                 &xHandleTaskLED ); /* ������  */

	xTaskCreate( vTaskDIR,    		/* ������  */
                 "vTaskDIR",  		/* ������    */
                 512,         		/* stack��С����λword��Ҳ����4�ֽ� */
                 NULL,        		/* �������  */
                 3,           		/* �������ȼ�*/
                 &xHandleTaskLED2 ); /* ������  */

	xTaskCreate( vTaskBLDC,    		/* ������  */
                 "vTaskBLDC",  		/* ������    */
                 512,         		/* stack��С����λword��Ҳ����4�ֽ� */
                 NULL,        		/* �������  */
                 4,           		/* �������ȼ�*/
                 &xHandleTaskBLDC ); /* ������  */
	
	xTaskCreate( vTaskMsgPro,     		/* ������  */
                 "vTaskMsgPro",   		/* ������    */
                 512,             		/* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,           		/* �������  */
                 5,               		/* �������ȼ�*/
                 &xHandleTaskMsgPro );  /* ������  */
	
	
	xTaskCreate( vTaskStart,     		/* ������  */
                 "vTaskStart",   		/* ������    */
                 512,            		/* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,           		/* �������  */
                 6,              		/* �������ȼ� ���*/
                 &xHandleTaskStart );   /* ������  */
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
	xQueue3 = xQueueCreate(10, sizeof(struct Msg *));
    if( xQueue3 == 0 )
    {
         printf("xQueue3 set up fail!!!!"); /* û�д����ɹ����û�������������봴��ʧ�ܵĴ������ */
    }
	//taskYIELD();              //����ʱ��Ƭ����CPU�ø�ͬ���ȼ�����������
	
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


