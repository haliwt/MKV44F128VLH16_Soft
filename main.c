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
#include "pwm.h"
#include "adc.h"
#include "pollingusart.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define MAX_LOG_LENGTH 20



/*******************************************************************************
 * Globals
 ******************************************************************************/
/* Logger queue handle */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Application API */
 
static void vTaskTaskUserIF(void *pvParameters);
static void vTaskLED(void *pvParameters);
static void vTaskLED2(void *pvParameters);
static void vTaskBLDC(void *pvParameters);


static void vTaskMsgPro(void *pvParameters);
static void vTaskStart(void *pvParameters);

static void AppTaskCreate (void);
static void AppObjCreate (void);
/*
**********************************************************************************************************
											变量声明
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
	uint8_t usData[3];
	uint8_t ulData[3];
}MSG_T;

MSG_T   g_tMsg; /* 定义一个结构体用于消息队列 */

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
    
     BOARD_InitDebugConsole();
     POLLING_USART_Init();
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
*	函 数 名: vTaskTaskUserIF
*	功能说明: 接口消息处理。
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 1  (数值越小优先级越低，这个跟uCOS相反)
********************************************************************************************************/
static void vTaskTaskUserIF(void *pvParameters)
{
    MSG_T   *ptMsg;
	uint8_t ch;
	uint8_t ucCount = 0;
	uint8_t ucKeyCode;

	
	/* 初始化结构体指针 */
	ptMsg = &g_tMsg;
	
	/* 初始化数组 */
	ptMsg->ucMessageID = 0;
	ptMsg->ulData[0] = 0;
	ptMsg->usData[0] = 0;

    while(1)
    {
		ucKeyCode = KEY_Scan(0);
		
		if (ucKeyCode != KEY_NONE)
		{
			switch (ucKeyCode)
			{
				/* K1键按下 打印任务执行情况 */
				case KEY1_PRES:			 
					printf("=================================================\r\n");
					printf("任务名      任务状态 优先级   剩余栈 任务序号\r\n");
					
				
					printf("\r\n任务名       运行计数         使用率\r\n");
				
				
					break;
				
				/* K2键按下，向xQueue1发送数据 */
				case KEY2_PRES:
					ucCount++;
				
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
				case KEY3_PRES:
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
				case KEY4_PRES :
					UART_ReadBlocking(DEMO_UART, &ch, 1);
                    UART_WriteBlocking(DEMO_UART, &ch, 1);
					break;
				case KEY5_PRES :
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
					break;
				case KEY6_PRES :
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
					
					break;
				case KEY7_PRES :
					break;
				case KEY8_PRES :
					break;
				
				/* 其他的键值不处理 */
				default:                     
					break;
			}
		}
		
		vTaskDelay(20);
	}
}
/*********************************************************************************************************
*	函 数 名: vTaskLED
*	功能说明: 使用函数xQueueReceive接收任务vTaskTaskUserIF发送的消息队列数据(xQueue2)	
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 2  
*********************************************************************************************************/
static void vTaskLED(void *pvParameters)
{
   
    
    MSG_T *ptMsg;
	BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(200); /* 设置最大等待时间为200ms */
	
    while(1)
    {
		xResult = xQueueReceive(xQueue2,                   /* 消息队列句柄 */
		                        (void *)&ptMsg,  		   /* 这里获取的是结构体的地址 */
		                        (TickType_t)xMaxBlockTime);/* 设置阻塞时间 */
		
		
		if(xResult == pdPASS)
		{
			/* 成功接收，并通过串口将数据打印出来 */
			printf("接收到消息队列数据ptMsg->ucMessageID = %d\r\n", ptMsg->ucMessageID);
			printf("接收到消息队列数据ptMsg->ulData[0] = %d\r\n", ptMsg->ulData[0]);
			printf("接收到消息队列数据ptMsg->usData[0] = %d\r\n", ptMsg->usData[0]);
		}
		else
		{
			 LED1 = !LED1;
			
		}
    }

    // TickType_t xLastWakeTime;
    // const TickType_t xFrequency = 10; // 10 x 0.5Khz（Ticks）= 10 * 5ms = 50ms
    // vTaskDelayUntil(&xLastWakeTime,xFrequency);//vTaskDelay(10);  //vTaskSuspend(NULL);

}

/*********************************************************************************************************
*	函 数 名: vTaskLED2
*	功能说明: 使用函数xQueueReceive接收任务vTaskTaskUserIF发送的消息队列数据(xQueue2)	
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 3  
*********************************************************************************************************/
static void vTaskLED2(void *pvParameters)
{
   
    
    MSG_T *ptMsg;
	BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(200); /* 设置最大等待时间为200ms */
	
    while(1)
    {
		xResult = xQueueReceive(xQueue2,                   /* 消息队列句柄 */
		                        (void *)&ptMsg,  		   /* 这里获取的是结构体的地址 */
		                        (TickType_t)xMaxBlockTime);/* 设置阻塞时间 */
		
		
		if(xResult == pdPASS)
		{
			/* 成功接收，并通过串口将数据打印出来 */
			printf("接收到消息队列数据ptMsg->ucMessageID = %d\r\n", ptMsg->ucMessageID);
			printf("接收到消息队列数据ptMsg->ulData[1] = %d\r\n", ptMsg->ulData[0]);
			printf("接收到消息队列数据ptMsg->usData[1] = %d\r\n", ptMsg->usData[0]);
		}
		else
		{
			 LED2 = !LED2;
			
		}
    }

    // TickType_t xLastWakeTime;
    // const TickType_t xFrequency = 10; // 10 x 0.5Khz（Ticks）= 10 * 5ms = 50ms
    // vTaskDelayUntil(&xLastWakeTime,xFrequency);//vTaskDelay(10);  //vTaskSuspend(NULL);

}
/*********************************************************************************************************
 *
 *	函 数 名: vTaskLED2
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
		xResult = xQueueReceive(xQueue3,                   /* 消息队列句柄 */
		                        (void *)&ptMsg,  		   /* 这里获取的是结构体的地址 */
		                        (TickType_t)xMaxBlockTime);/* 设置阻塞时间 */
		
		
		if(xResult == pdPASS)
		{
			/* 成功接收，并通过串口将数据打印出来 */
			printf("接收到消息队列数据ptMsg->ucMessageID = %d\r\n", ptMsg->ucMessageID);
			printf("接收到消息队列数据ptMsg->ulData[2] = %d\r\n", ptMsg->ulData[0]);
			printf("接收到消息队列数据ptMsg->usData[2] = %d\r\n", ptMsg->usData[0]);
			uwStep =HallSensor_GetPinState();
			printf("uwStep = %d \n",uwStep);
			HALLSensor_Detected_BLDC(uwStep);
			
		}
		else
		{
			 LED2 = !LED2;
			 LED1 = !LED1;
			
		}
    }

    // TickType_t xLastWakeTime;
    // const TickType_t xFrequency = 10; // 10 x 0.5Khz（Ticks）= 10 * 5ms = 50ms
    // vTaskDelayUntil(&xLastWakeTime,xFrequency);//vTaskDelay(10);  //vTaskSuspend(NULL);

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
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(300); /* 设置最大等待时间为300ms */
	uint8_t ucQueueMsgValue;
	
    while(1)
    {
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
    }

}

/*
*********************************************************************************************************
*	函 数 名: vTaskStart
*	功能说明: 启动任务，也就是最高优先级任务，这里用作按键扫描。
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 6  
*********************************************************************************************************
*/
static void vTaskStart(void *pvParameters)
{
    while(1)
    {
		/* 按键扫描 */
		KEY_Scan(0);//bsp_KeyScan();
        vTaskDelay(10);
    }
}
/*
*********************************************************************************************************
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{
    xTaskCreate( vTaskTaskUserIF,   	/* 任务函数  */
                 "vTaskUserIF",     	/* 任务名    */
                 512,               	/* 任务栈大小，单位word，也就是4字节 */
                 NULL,              	/* 任务参数  */
                 1,                 	/* 任务优先级 最低*/
                 &xHandleTaskUserIF );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskLED,    		/* 任务函数  */
                 "vTaskLED",  		/* 任务名    */
                 512,         		/* stack大小，单位word，也就是4字节 */
                 NULL,        		/* 任务参数  */
                 2,           		/* 任务优先级*/
                 &xHandleTaskLED ); /* 任务句柄  */

	xTaskCreate( vTaskLED2,    		/* 任务函数  */
                 "vTaskLED2",  		/* 任务名    */
                 512,         		/* stack大小，单位word，也就是4字节 */
                 NULL,        		/* 任务参数  */
                 3,           		/* 任务优先级*/
                 &xHandleTaskLED2 ); /* 任务句柄  */

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
	xQueue1 = xQueueCreate(5, sizeof(uint8_t));
    if( xQueue1 == 0 )
    {
       printf("xQueuel set up fail!!!!"); 
       /* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
    }
     /* 创建10个存储指针变量的消息队列，由于CM3/CM4内核是32位机，一个指针变量占用4个字节 */
	xQueue2 = xQueueCreate(5, sizeof(struct Msg *));
    if( xQueue2 == 0 )
    {
         printf("xQueue2 set up fail!!!!"); /* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
    }
	/* 创建10个存储指针变量的消息队列，由于CM3/CM4内核是32位机，一个指针变量占用4个字节 */
	xQueue3 = xQueueCreate(5, sizeof(struct Msg *));
    if( xQueue3 == 0 )
    {
         printf("xQueue3 set up fail!!!!"); /* 没有创建成功，用户可以在这里加入创建失败的处理机制 */
    }
	
}


