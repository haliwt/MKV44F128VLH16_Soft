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
#if 0
static void vTaskBLDC(void *pvParameters);
static void vTaskMsgPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
#endif 

static void AppTaskCreate (void);
static void AppObjCreate (void);

/*
**********************************************************************************************************
											变量声明
**********************************************************************************************************
*/
static TaskHandle_t xHandleTaskUSART = NULL;

static TaskHandle_t xHandleTaskLED = NULL;
static TaskHandle_t xHandleTaskDIR = NULL;
//static TaskHandle_t xHandleTaskBLDC = NULL;


//static TaskHandle_t xHandleTaskMsgPro = NULL;
//static TaskHandle_t xHandleTaskStart = NULL;
static QueueHandle_t xQueue1 = NULL;
static QueueHandle_t xQueue2 = NULL;
static QueueHandle_t xQueue3 = NULL;



typedef struct Msg
{
	uint8_t  ucMessageID;
	uint8_t usData[8];
	uint8_t ulData[8];
}MSG_T;

MSG_T   g_tMsg; /* 定义一个结构体用于消息队列 */
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
    /* 创建任务 */
	AppTaskCreate();
    
    /* 创建任务通信机制 */
	AppObjCreate();// WT.EDIT log_init(10, MAX_LOG_LENGTH);
    
    vTaskStartScheduler();
    for (;;)
        ;
}

/******************************************************************************************************
*	函 数 名: vTaskTaskUSART
*	功能说明: 接口消息处理。
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 1  (数值越小优先级越低，最低优先级)
********************************************************************************************************/
static void vTaskUSART(void *pvParameters)
{
  
  MSG_T *ptMsg;
  uint8_t ucCount;
  uint8_t ch[8];
  
  /* 初始化结构体指针 */
	ptMsg = &g_tMsg;
	
	/* 初始化数组 */
	ptMsg->ucMessageID = 0;
	ptMsg->ulData[0] = 0;
	ptMsg->usData[0] = 0;
  while(1)
    {
      
                ptMsg->ucMessageID ++;
		ptMsg->ulData[2] = 0x0a;
		ptMsg->usData[2] = 0x0b;
   
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
            ucCount= 10;
            /* ?ò???￠?óáD·￠êy?Y￡?è?1????￠?óáD?úá?￡?μè′y10??ê±?ó?ú?? */
            if( xQueueSend(xQueue1,
                     (void *) &ucCount,
                     (TickType_t)10) != pdPASS )
            {
             /* 发送失败，即使等待了10个时钟节拍 */
	   printf("K6键按下，向xQueue2发送数据失败，即使等待了10个时钟节拍\r\n");
            }
            else
            {
             /* 发送成功 */
		printf("K6键按下，向xQueue1发送数据成功\r\n");							
            }
            
            /* ?ò???￠?óáD·￠êy?Y￡?è?1????￠?óáD?úá?￡?μè′y10??ê±?ó?ú?? */
            if( xQueueSend(xQueue2,
                     (void *)&ptMsg,
                     (TickType_t)10) != pdPASS )
            {
             /* 发送失败，即使等待了10个时钟节拍 */
	   printf("vTaskDIR，即使等待了10个时钟节拍\r\n");
            }
            else
            {
             /* 发送成功 */
		printf("DIR send ，向xQueue1发送数据成功\r\n");							
            }
        }
        
#endif 
	vTaskDelay(200);
    }
}
/*********************************************************************************************************
*	函 数 名: vTaskLED
*	功能说明: 使用函数xQueueReceive接收任务vTaskTaskUserIF发送的消息队列数据(xQueue2)	
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 2  
*********************************************************************************************************/
#if 1
static void vTaskLED(void *pvParameters)
{
   
    
    BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(200); /* 设置最大等待时间为5ms */
	uint8_t ucQueueMsgValue;
    while(1)
    {
	      printf("vTaskLED-2 \r\n");
              xResult = xQueueReceive(xQueue1,                   /* ???￠?óáD??±ú */
		                        (void *)&ucQueueMsgValue,  /* ′?′￠?óê?μ?μ?êy?Yμ?±?á?ucQueueMsgValue?D */
		                        (TickType_t)xMaxBlockTime);/* éè??×èè?ê±?? */
		
		if(xResult == pdPASS)
		{
			/* 成功接收，并通过串口将数据打印出来 */
			printf("接收到消息队列数据YucQueueMsgValue = %d\r\n", ucQueueMsgValue);
			
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
		//vTaskDelay(100);//taskYIELD();              //放弃时间片，把CPU让给同优先级的其它任务
    }

  

}
#endif 

/*********************************************************************************************************
*	函 数 名: vTaskDIR
*	功能说明: 使用函数xQueueReceive接收任务vTaskTaskUserIF发送的消息队列数据(xQueue2)	
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 3  
*********************************************************************************************************/
static void vTaskDIR(void *pvParameters)
{
    MSG_T *ptMsg;
    BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(200); /* 设置最大等待时间为300ms */
	//uint8_t ucQueueMsgValue;
	
    while(1)
    {           printf("vTaskDIR-3 \r\n");
		xResult = xQueueReceive(xQueue2,                   /* 消息队列句柄 */
		                       (void *)&ptMsg,  /* 存储接收到的数据到变量ucQueueMsgValue中 */
		                       (TickType_t)xMaxBlockTime);/* 设置阻塞时间 */
		
		if(xResult == pdPASS)
		{
			/* 成功接收，并通过串口将数据打印出来 */
			printf("接收到消息队列数据ptMsg->ucMessageID = %d\r\n", ptMsg->ucMessageID);
			printf("接收到消息队列数据ptMsg->ulData[2] = %d\r\n", ptMsg->ulData[2]);
			printf("接收到消息队列数据ptMsg->usData[2] = %d\r\n", ptMsg->usData[2]);
			Dir = Dir;
		}
		else
		{
			/* 超时 */
			LED2 =!LED2;
		}
		taskYIELD();              //放弃时间片，把CPU让给同优先级的其它任务
    } 
   
}
#if 0
/*********************************************************************************************************
 *
 *	函 数 名: vTaskBLDC
 *	功能说明: 使用函数xQueueReceive接收任务vTaskTaskUserIF发送的消息队列数据(xQueue2)	
 *	形    参: pvParameters 是在创建该任务时传递的形参
 *	返 回 值: 无
 *   优 先 级: 4 
 *
*********************************************************************************************************/
static void vTaskBLDC(void *pvParameters)
{
         MSG_T *ptMsg;
	BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(200); /* 设置最大等待时间为200ms */
	
    while(1)
    {
	  printf("vTaskBLDC-4 \r\n");	
          xResult = xQueueReceive(xQueue3,                   /* 消息队列句柄 */
		                        (void *)&ptMsg,  		   /* 这里获取的是结构体的地址 */
		                        (TickType_t)xMaxBlockTime);/* 设置阻塞时间 */
		
		
		if(xResult == pdPASS)
		{
			/* 成功接收，并通过串口将数据打印出来 */
			printf("接收到消息队列数据ptMsg->ucMessageID = %d\r\n", ptMsg->ucMessageID);
			printf("接收到消息队列数据ptMsg->ulData[2] = %d\r\n", ptMsg->ulData[2]);
			printf("接收到消息队列数据ptMsg->usData[2] = %d\r\n", ptMsg->usData[2]);
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
		taskYIELD();              //放弃时间片，把CPU让给同优先级的其它任务
    }

}


/**********************************************************************************************************
*	函 数 名: vTaskMsgPro
*	功能说明: 使用函数xQueueReceive接收任务vTaskTaskUserIF发送的消息队列数据(xQueue1)
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 5  
**********************************************************************************************************/

static void vTaskMsgPro(void *pvParameters)
{
    BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(200); /* 设置最大等待时间为300ms */
	uint8_t ucQueueMsgValue;
	
    while(1)
    {
	       printf("vTaskMSG-5 \r\n");
               xResult = xQueueReceive(xQueue1,                   /* 消息队列句柄 */
		                        (void *)&ucQueueMsgValue,  /* 存储接收到的数据到变量ucQueueMsgValue中 */
		                        (TickType_t)xMaxBlockTime);/* 设置阻塞时间 */
		
		if(xResult == pdPASS)
		{
			/* 成功接收，并通过串口将数据打印出来 */
			printf("接收到消息队列数据ucQueueMsgValue = %d\r\n", ucQueueMsgValue);
		}
		else
		{
			/* 超时 */
			LED2 =!LED2;
		}
		//taskYIELD();              //放弃时间片，把CPU让给同优先级的其它任务
    }

}

/*********************************************************************************************************
*
*	函 数 名: vTaskStart
*	功能说明: 启动任务，也就是最高优先级任务，这里用作按键扫描。
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 6  (优先级最高)
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
	
	/* 初始化结构体指针 */
	ptMsg = &g_tMsg;
	
	/* 初始化数组 */
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
				/* K1键按下 打印任务执行情况 */
				case START_PRES:			 
					ptMsg->ucMessageID ++;
					ptMsg->ulData[2] = 0x0a;
					ptMsg->usData[2] = 0x0b;
					
					/* 使用消息队列实现指针变量的传递 */
					if(xQueueSend(xQueue3,                  /* 消息队列句柄 */
						     (void *) &ptMsg,           /* 发送结构体指针变量ptMsg的地址 */
						     (TickType_t)10) != pdPASS )
					{
						/* 发送失败，即使等待了10个时钟节拍 */
						printf("K6键按下，向xQueue2发送数据失败，即使等待了10个时钟节拍\r\n");
					}
					else
					{
						/* 发送成功 */
						printf("K6键按下，向xQueue2发送数据成功\r\n");						
					}
				
					break;
				
				/* K2键按下，向xQueue1发送数据 */
				case DIR_PRES:
					ucCount ++ ;
				 
					/* 向消息队列发数据，如果消息队列满了，等待10个时钟节拍 */
					if( xQueueSend(xQueue1,
								   (void *) &ucCount,
								   (TickType_t)10) != pdPASS )
					{
						/* 发送失败，即使等待了10个时钟节拍 */
						printf("K2键按下，向xQueue1发送数据失败，即使等待了10个时钟节拍\r\n");
					}
					else
					{
						/* 发送成功 */
						printf("K2键按下，向xQueue1发送数据成功\r\n");						
					}
					break;
				
				/* K3键按下，向xQueue2发送数据 */
				case DIGITAL_ADD_PRES:
					ptMsg->ucMessageID++;
					ptMsg->ulData[0]++;;
					ptMsg->usData[0]++;
					
					/* 使用消息队列实现指针变量的传递 */
					if(xQueueSend(xQueue2,                  /* 消息队列句柄 */
								 (void *) &ptMsg,           /* 发送结构体指针变量ptMsg的地址 */
								 (TickType_t)10) != pdPASS )
					{
						/* 发送失败，即使等待了10个时钟节拍 */
						printf("K3键按下，向xQueue2发送数据失败，即使等待了10个时钟节拍\r\n");
					}
					else
					{
						/* 发送成功 */
						printf("K3键按下，向xQueue2发送数据成功\r\n");						
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
					
					/* 使用消息队列实现指针变量的传递 */
					if(xQueueSend(xQueue2,                  /* 消息队列句柄 */
						     (void *) &ptMsg,           /* 发送结构体指针变量ptMsg的地址 */
						     (TickType_t)10) != pdPASS )
					{
						/* 发送失败，即使等待了10个时钟节拍 */
						printf("K3键按下，向xQueue2发送数据失败，即使等待了10个时钟节拍\r\n");
					}
					else
					{
						/* 发送成功 */
						printf("K3键按下，向xQueue2发送数据成功\r\n");						
					}
					//taskYIELD();              //放弃时间片，把CPU让给同优先级的其它任务
					break;
				case WIPERS_PRES :
					ptMsg->ucMessageID++;
					ptMsg->ulData[2]++;;
					ptMsg->usData[2]++;
					
					/* 使用消息队列实现指针变量的传递 */
					if(xQueueSend(xQueue3,                  /* 消息队列句柄 */
						     (void *) &ptMsg,           /* 发送结构体指针变量ptMsg的地址 */
						     (TickType_t)10) != pdPASS )
					{
						/* 发送失败，即使等待了10个时钟节拍 */
						printf("K6键按下，向xQueue2发送数据失败，即使等待了10个时钟节拍\r\n");
					}
					else
					{
						/* 发送成功 */
						printf("K6键按下，向xQueue2发送数据成功\r\n");						
					}
					//taskYIELD();              //放弃时间片，把CPU让给同优先级的其它任务
					break;
				case AIR_PRES :
					break;
				
				/* 其他的键值不处理 */
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
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************/
static void AppTaskCreate (void)
{
    xTaskCreate( vTaskUSART,   	/* 任务函数  */
                 "vTaskUserIF",     	/* 任务名    */
                 512,               	/* 任务栈大小，单位word，也就是4字节 */
                 NULL,              	/* 任务参数  */
                 1,                 	/* 任务优先级 最低*/
                 &xHandleTaskUSART );  /* 任务句柄  */
#if 1	
	xTaskCreate( vTaskLED,    		/* 任务函数  */
                 "vTaskLED",  		/* 任务名    */
                 512,         		/* stack大小，单位word，也就是4字节 */
                 NULL,        		/* 任务参数  */
                 2,           		/* 任务优先级*/
                 &xHandleTaskLED ); /* 任务句柄  */
#endif 

	xTaskCreate( vTaskDIR,    		/* 任务函数  */
                 "vTaskDIR",  		/* 任务名    */
                 512,         		/* stack大小，单位word，也就是4字节 */
                 NULL,        		/* 任务参数  */
                 3,           		/* 任务优先级*/
                 &xHandleTaskDIR ); /* 任务句柄  */
#if 0
	xTaskCreate( vTaskBLDC,    		/* 任务函数  */
                 "vTaskBLDC",  		/* 任务名    */
                 512,         		/* stack大小，单位word，也就是4字节 */
                 NULL,        		/* 任务参数  */
                 4,           		/* 任务优先级*/
                 &xHandleTaskBLDC ); /* 任务句柄  */
	
	xTaskCreate( vTaskMsgPro,     		/* 任务函数  */
                 "vTaskMsgPro",   		/* 任务名    */
                 512,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 5,               		/* 任务优先级*/
                 &xHandleTaskMsgPro );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务名    */
                 512,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 6,              		/* 任务优先级 最高*/
                 &xHandleTaskStart );   /* 任务句柄  */
#endif 
}
/********************************************************************
*	函 数 名: AppObjCreate
*	功能说明: 创建任务通信机制
*	形    参: 无
*	返 回 值: 无
********************************************************************/
//void log_init(uint32_t queue_length, uint32_t max_log_lenght)
static void AppObjCreate (void)
{
  
    /* 创建10个uint8_t型消息队列 */
	xQueue1 = xQueueCreate(10, sizeof(uint8_t));
    if( xQueue1 == 0 )
    {
       printf("xQueuel set up fail!!!!"); 
       /* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
    }
     /* 创建10个存储指针变量的消息队列，由于CM3/CM4内核是32位机，一个指针变量占用4个字节 */
	xQueue2 = xQueueCreate(10, sizeof(struct Msg *));
    if( xQueue2 == 0 )
    {
         printf("xQueue2 set up fail!!!!"); /* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
    }
	/* 创建10个存储指针变量的消息队列，由于CM3/CM4内核是32位机，一个指针变量占用4个字节 */
	xQueue3 = xQueueCreate(10, sizeof(struct Msg *));
    if( xQueue3 == 0 )
    {
         printf("xQueue3 set up fail!!!!"); /* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
    }
	//taskYIELD();              //放弃时间片，把CPU让给同优先级的其它任务
	
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

