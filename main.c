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
											变量声明
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
	uint8_t  ulData[8];
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
    
    HallSensor_GetPinState();
    OUTPUT_Fucntion_Init();
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
  uint8_t ucCount,i;
  uint8_t ch[8];
  const TickType_t xMaxBlockTime = pdMS_TO_TICKS(300); /* 设置最大等待时间为5ms */
  /* 初始化结构体指针 */
	ptMsg = &g_tMsg;
	
	/* 初始化数组 */
	ptMsg->ucMessageID = 0;
	ptMsg->ulData[0] = 0;
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
          ptMsg->ulData[i]=ch[i];
		  ptMsg->usData[i]=ch[i];
		  printf("ptMsg[i]= %#x \r\n",ptMsg->ulData[i]);
		  printf("ptMsg->usDta[i]= %#x \r\n",ptMsg->usData[i]);

		}
        
        if(ch[0] == 0x31) //'1' = 0x31
         {
            /* 向消息队列发送数据，如果消息队列满，等待10个时钟节拍 */
		    ptMsg->ucMessageID = ch[1];
            if( xQueueSend(xQueue2,
                     (void *) &ptMsg,
                     (TickType_t)10) != pdPASS )
            {
             /* 发送失败，即使等待了10个时钟节拍 */
	       		printf("向xQueue-2发送数据失败??????????\r\n");
            }
            else
            {
             /* 发送成功 */
		      printf("发送数据xQueue-2成功！\r\n");							
            }
        }
		if(ch[0]== 0x30 )
		{
            ucCount= ptMsg->ucMessageID=ch[5];
			if( xQueueSend(xQueue1,
								   (void *) &ucCount,
								   (TickType_t)10) != pdPASS )
			{
				/*发送失败，等待10个时钟节拍*/
				printf("发送数据xQueue-l失败??????????\r\n");
			}
			else
			{
				/*发送数据成功*/
				printf("发送数据xQueue-1成功！\r\n");						
			}
        }

 
		vTaskDelay(xMaxBlockTime);
    }
}

/*********************************************************************************************************
*	函 数 名: vTaskBLDC
*	功能说明: 使用函数xQueueReceive接收任务vTaskTaskUserIF发送的消息队列数据(xQueue2)	
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 2  
*********************************************************************************************************/
static void vTaskBLDC(void *pvParameters)
{

    
	uint16_t sampleMask;
	BaseType_t xResult;
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS(200); /* 设置最大等待时间为300ms */
	uint8_t ucQueueMsgValue;
	while(1)
    {       
      printf("vTaskBLDC-2 \r\n");  
	  xResult = xQueueReceive(xQueue1,                   	/* 消息队列句柄3 */
		                        (void *)&ucQueueMsgValue,  	/* 存储到接收到数据变量ucQueueMsgValue */
		                        (TickType_t)xMaxBlockTime);	/*设置阻塞时间*/
		
		if(xResult == pdPASS)
		{
			/* 接收数据成功 */
          printf("vTaskBLDC ucQueueMsgValue = %#x\r\n", ucQueueMsgValue);
		}
		else
		{
			/* 超时 */
			LED1= 0;
			LED2= 0;
		}

	  if(ucQueueMsgValue==0) //|| (recoder_number.start_number ==0))//刹车
	  {
         PMW_AllClose_ABC_Channel();
         DelayMs(10U);
         PMW_AllClose_ABC_Channel();
		 PWM_StopTimer(BOARD_PWM_BASEADDR,  kPWM_Control_Module_0);
		 PWM_StopTimer(BOARD_PWM_BASEADDR,  kPWM_Control_Module_1);
		 PWM_StopTimer(BOARD_PWM_BASEADDR,  kPWM_Control_Module_2);
		 printf("Break is OK !!!!\r\n");
       }
	 else 
	 {
             printf("Motor Run is OK !!!!\r\n");
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
        	HALLSensor_Detected_BLDC(uwStep); 

        }
        vTaskDelay(xMaxBlockTime);         
     }  
 } 

/*********************************************************************************************************
*
*	函 数 名: vTaskCOTL
*	功能说明: 接收物理按键和数字按键的命令
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 3  
*
*********************************************************************************************************/
static void vTaskCOTL(void *pvParameters)
{
    MSG_T *ptMsg; 
	uint8_t ucKeyCode;
   
    BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(300); /* 设置最大等待时间为5ms */
	uint8_t ucControl;
	
    while(1)
    {
	      printf("vTaskCOTL-3 \r\n");
		  ucKeyCode = KEY_Scan(0);
          xResult = xQueueReceive(xQueue2,                   	/* 队列句柄 */
		                        (void *)&ptMsg,  				/*接收到的数据地址 */
		                        (TickType_t)xMaxBlockTime);		/* 设置阻塞时间*/
		
				if(xResult == pdPASS)
				{
					/* 成功接收，并通过串口将数据打印出来 */
		          printf("接收到消息队列数据vTaskCOTL = %#x\r\n", ptMsg->ucMessageID);
		          printf("接收到消息队列数据ulData[0] = %#x\r\n", ptMsg->ulData[0]);
		          printf("接收到消息队列数据vsData[0] = %#x\r\n", ptMsg->usData[0]);
				  LED1 = !LED1;
				  LED2 = !LED2;
				  
				}
				else
				{
					 LED1 = 0;
			         LED2 = 0;
		        }
				if(ptMsg->ucMessageID==0x32 || ucKeyCode==START_PRES)
				{
		          recoder_number.dir_change ++;
		          if(recoder_number.dir_change == 1)
		          {
                     ucControl = 1;
				  }
				  else 
				  {
                     ucControl =0;
					 recoder_number.dir_change =0;
				  }

		         		/* 向消息队列发送数据 */
					if( xQueueSend(xQueue1,
								   (void *) &ucControl,
								   (TickType_t)10) != pdPASS )
					{
						/* 发送数据失败，等待10个节拍 */
						printf("START_PRES is fail?????????\r\n");
					}
					else
					{
						/* 发送数据成功 */
						printf("START_PRES is OK \r\n");						
					}

				}
      
				if(ptMsg->ucMessageID==0x33 || ucKeyCode==DIR_PRES) //driection
				{
				     /* 向消息队列发送数据 */
							if( xQueueSend(xQueue1,
										   (void *) &ucControl,
										   (TickType_t)10) != pdPASS )
							{
								/* 发送数据失败，等待10个节拍 */
								printf("START_PRES is fail?????????\r\n");
							}
							else
							{
								/* 发送数据成功 */
								printf("START_PRES is OK \r\n");						
							} 
				}
				
				if(ptMsg->ucMessageID==0x34 || ucKeyCode==DIGITAL_ADD_PRES) //driection
				{
				     /* 向消息队列发送数据 */
							if( xQueueSend(xQueue1,
										   (void *) &ucControl,
										   (TickType_t)10) != pdPASS )
							{
								/* 发送数据失败，等待10个节拍 */
								printf("START_PRES is fail?????????\r\n");
							}
							else
							{
								/* 发送数据成功 */
								printf("START_PRES is OK \r\n");						
							} 
				}
				if(ptMsg->ucMessageID==0x35 || ucKeyCode==DIGITAL_REDUCE_PRES) //数字加减键
				{
				     /* 向消息队列发送数据 */
							if( xQueueSend(xQueue1,
										   (void *) &ucControl,
										   (TickType_t)10) != pdPASS )
							{
								/* 发送数据失败，等待10个节拍 */
								printf("START_PRES is fail?????????\r\n");
							}
							else
							{
								/* 发送数据成功 */
								printf("START_PRES is OK \r\n");					
							} 
				}
				if(ptMsg->ucMessageID==0x36 || ucKeyCode==DOOR_PRES)
				{ 
				/* 向消息队列发送数据 */
						if( xQueueSend(xQueue1,
									   (void *) &ucControl,
									   (TickType_t)10) != pdPASS )
						{
							/* 发送数据失败，等待10个节拍 */
							printf("START_PRES is fail?????????\r\n");
						}
						else
						{
							/* 发送数据成功 */
							printf("START_PRES is OK \r\n");			
						} 

				}
				if(ptMsg->ucMessageID==0x37 || ucKeyCode==HALL_PRES)
				{ 
				/* 向消息队列发送数据 */
						if( xQueueSend(xQueue1,
									   (void *) &ucControl,
									   (TickType_t)10) != pdPASS )
						{
							/* 发送数据失败，等待10个节拍 */
							printf("START_PRES is fail?????????\r\n");
						}
						else
						{
							/* 发送数据成功 */
							printf("START_PRES is OK \r\n");			
						} 

				}

	 			if(ptMsg->ucMessageID==0x38 || ucKeyCode==WHEEL_PRES)
				{ 
				/* 向消息队列发送数据 */
						if( xQueueSend(xQueue1,
									   (void *) &ucControl,
									   (TickType_t)10) != pdPASS )
						{
							/* 发送数据失败，等待10个节拍 */
							printf("START_PRES is fail?????????\r\n");
						}
						else
						{
							/* 发送数据成功 */
							printf("START_PRES is OK \r\n");			
						} 

				}
				if(ptMsg->ucMessageID==0x39 || ucKeyCode==WIPERS_PRES)
				  { 
				   /* 向消息队列发送数据 */
					if( xQueueSend(xQueue1,
								  (void *) &ucControl,
								  (TickType_t)10) != pdPASS )
					{
						/* 发送数据失败，等待10个节拍 */
						printf("START_PRES is fail?????????\r\n");
					}
					else
					{
						/* 发送数据成功 */
						printf("START_PRES is OK \r\n");			
					}
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

				  }
				if(ptMsg->ucMessageID==0x41 ||ptMsg->ucMessageID==0x61 || ucKeyCode==AIR_PRES)
				  { 
				   /* 向消息队列发送数据 */
							if( xQueueSend(xQueue1,
										   (void *) &ucControl,
										   (TickType_t)10) != pdPASS )
							{
								/* 发送数据失败，等待10个节拍 */
								printf("START_PRES is fail?????????\r\n");
							}
							else
							{
								/* 发送数据成功 */
								printf("START_PRES is OK \r\n");			
							} 

				  }
		#if 0
		if (ucKeyCode != KEY_UP ||xResult == pdPASS )
		{
			switch (ucKeyCode || ptMsg->ucMessageID)
			{
                          
              case 2 ://START_PRES : //2
                    PRINTF("START_PRES key \r\n");
                    recoder_number.start_number++;
					if(recoder_number.start_number ==1)
					{
	                   ucControl =1;
					}
				   	else 
				   	{
					   ucControl =0;
					   recoder_number.air_number=0;
					   LED1=0;
				       LED2=0;
			
				    }
					
					/* 向消息队列发送数据 */
					if( xQueueSend(xQueue1,
								   (void *) &ucControl,
								   (TickType_t)10) != pdPASS )
					{
						/* 发送数据失败，等待10个节拍 */
						printf("START_PRES is fail?????????\r\n");
					}
					else
					{
						/* 发送数据成功 */
						printf("START_PRES is OK \r\n");						
					}
				   
			  break;
			  
			  case 3 :  //DIR_PRES: //3

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
				 /* 向消息队列发送数据 */
					if( xQueueSend(xQueue1,
								   (void *) &ucControl,
								   (TickType_t)10) != pdPASS )
					{
						/* 发送数据失败，等待10个节拍 */
						printf("DIR_PRES is fail?????????\r\n");
					}
					else
					{
						/* 发送数据成功 */
						printf("DIR_PRES is OK \r\n");						
					}
                  #endif 
           		
			 break;
				
			 case DIGITAL_ADD_PRES ://4
				PRINTF("DIGITAL_ADD_PRES key \r\n");
				 /* 向消息队列发送数据 */
					if( xQueueSend(xQueue1,
								   (void *) &ucControl,
								   (TickType_t)10) != pdPASS )
					{
						/* 发送数据失败，等待10个节拍 */
						printf("DIGITAL_ADD_PRES is fail?????????\r\n");
					}
					else
					{
						/* 发送数据成功 */
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
				
			 case WIPERS_PRES: //9雨刮器
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
			 break;
			}
        }
		#endif 
       
		//vTaskDelay(xMaxBlockTime);           vTaskYILED() //放弃时间片，把CPU让给同优先级的其它任务
    }

  

}
/********************************************************************************************************
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************/
static void AppTaskCreate (void)
{
    xTaskCreate( vTaskUSART,   									/* 任务函数  */
                 "vTaskUserIF",     							/* 任务名    */
                 configMINIMAL_STACK_SIZE + 166,               	/* 任务栈大小，单位word，也就是4字节 */
                 NULL,              							/* 任务参数  */
                 tskIDLE_PRIORITY+1,                 			/* 任务优先级 最低*/
                 &xHandleTaskUSART );  							/* 任务句柄  */
	
	xTaskCreate( vTaskBLDC,    									/* 任务函数  */
                 "vTaskBLDC",  									/* 任务名    */
                 configMINIMAL_STACK_SIZE + 934,         		/* stack大小，单位word，也就是4字节 */
                 NULL,        									/* 任务参数  */
                 tskIDLE_PRIORITY+2,           					/* 任务优先级*/
                 &xHandleTaskBLDC); 							/* 任务句柄  */

	xTaskCreate( vTaskCOTL,    									/* 任务函数  */
                 "vTaskCOTL",  									/* 任务名    */
                 configMINIMAL_STACK_SIZE + 422,         		/* stack大小，单位word，也就是4字节 */
                 NULL,        									/* 任务参数  */
                 tskIDLE_PRIORITY+3,           					/* 任务优先级*/
                 &xHandleTaskCOTL); 							/* 任务句柄  */

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

