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
static void vTaskSUBJ(void *pvParameters);
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
static TaskHandle_t xHandleTaskSUBJ =  NULL;
static TaskHandle_t xHandleTaskBLDC = NULL;
static TaskHandle_t xHandleTaskCOTL = NULL;

static QueueHandle_t xQueue1 = NULL;
static QueueHandle_t xQueue2 = NULL;



typedef struct Msg
{
	uint8_t  ucMessageID;
	uint8_t  usData[8];
	
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
    OUTPUT_Fucntion_Init();
    ADC_CADC_Init();
    ABC_POWER_OUTPUT_Init();
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
  //TickType_t xLastWakeTime;
  //const TickType_t xFrequency = 300;
  MSG_T *ptMsg;
  uint8_t i;
  uint8_t ch[8];
  const TickType_t xMaxBlockTime = pdMS_TO_TICKS(300); /* 设置最大等待时间为5ms */
  /* 初始化结构体指针 */
	ptMsg = &g_tMsg;
	
	/* 初始化数组 */
	ptMsg->ucMessageID = 0;
	ptMsg->usData[0] = 0;
  while(1)
    {
        printf("vTaskUSART-1 \r\n");
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
           
		      xTaskNotify(xHandleTaskCOTL,      /* 目标任务 */
								ptMsg->usData[1],              /* 发送数据 */
								eSetValueWithOverwrite);/* 上次目标任务没有执行，会被覆盖 */

        #if 0
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
		#endif 
        }
		
		vTaskDelay(xMaxBlockTime);
	 //vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}
/*********************************************************************************************************
*
*	函 数 名: vTaskSUBJ
*	功能说明: 接收物理按键和数字按键的命令
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 2  
*
*********************************************************************************************************/
static void vTaskSUBJ(void *pvParameters)
{
     uint32_t vlSubj;
     uint8_t ucConValue;
	 BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100); /* 设置最大等待时间为5ms */

	while(1)
    {
          printf("vTaskSUBJ-2 \r\n");
		
		  xResult = xTaskNotifyWait(0x00000000,      
						          0xFFFFFFFF,      
						          &vlSubj,        /* 存储ulNotifiedValue在ulvalue中 */
						          xMaxBlockTime);  /* 最大延迟时间 */
          
		  ucConValue = (uint8_t)vlSubj;
		if( xResult == pdPASS )
		{
			printf("vTaskSUBJ vlSubj = %#x\r\n", vlSubj);
			 if(ucConValue==0x03)
			 {
                DOOR_OUTPUT =1;//door open
		        printf("DOOR_OUTPUT = 1 @@@@@@@\r\n");  
		     }
             if(ucConValue==0x04)
			 {
                DOOR_OUTPUT =0;//door open
		        printf("DOOR_OUTPUT = 0 ~~~~~~~\r\n");     
		     }  
		         	
             
			 
		}
		else
		{
			/* 3?ê± */
			LED1=0;
			LED2 =0 ;
		}


	}
}
/*********************************************************************************************************
*	函 数 名: vTaskBLDC
*	功能说明: 使用函数xQueueReceive接收任务vTaskTaskUserIF发送的消息队列数据(xQueue2)	
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 3  
*********************************************************************************************************/
static void vTaskBLDC(void *pvParameters)
{
    
    uint8_t ucValue;
	TickType_t xLastWakeTime;
	
	const TickType_t xFrequency = 100;
    xLastWakeTime = xTaskGetTickCount();
    volatile uint16_t pwm_f=0;
	uint16_t sampleMask;
	BaseType_t xResult;
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS(5); /* 设置最大等待时间为300ms */
	uint32_t ucQueueMsgValue;
	while(1)
    {       
      printf("vTaskBLDC-2 \r\n");  

        xResult = xTaskNotifyWait(0x00000000,      
						          0xFFFFFFFF,      
						          &ucQueueMsgValue,        /* 存储ulNotifiedValue在ulvalue中 */
						          xMaxBlockTime);  /* 最大延迟时间 */
	  if(xResult == pdPASS)
		{
			/* 接收数据成功 */
          printf("vTaskBLDC ConmessageID = %#x\r\n",ucQueueMsgValue);
		  ucValue = ucQueueMsgValue;
		}
		else
		{
			/* 超时 */
			LED1= !LED1;
			
			
		}
	  
	  #if 0
	  xResult = xQueueReceive(xQueue1,                   	/* 消息队列句柄3 */
		                        (void *)&ucQueueMsgValue,  	/* 存储到接收到数据变量ucQueueMsgValue */
		                        (TickType_t)xMaxBlockTime);	/*设置阻塞时间*/
		
		if(xResult == pdPASS)
		{
			/* 接收数据成功 */
          printf("vTaskBLDC ConmessageID = %#x\r\n",ucQueueMsgValue);
		}
		else
		{
			/* 超时 */
			LED1= !LED1;
			
			
		}
	
 #endif     
	  if((ucValue==0x0a)|| (recoder_number.break_f ==1))//刹车
	  {
         taskENTER_CRITICAL(); //进入临界状态
		 PMW_AllClose_ABC_Channel();
         DelayMs(10U);
         PMW_AllClose_ABC_Channel();
		 PWM_StopTimer(BOARD_PWM_BASEADDR,  kPWM_Control_Module_0);
		 PWM_StopTimer(BOARD_PWM_BASEADDR,  kPWM_Control_Module_1);
		 PWM_StopTimer(BOARD_PWM_BASEADDR,  kPWM_Control_Module_2);
		 taskEXIT_CRITICAL(); //退出临界状态
		 printf("Break is OK $$$$$$$$$$$$$\r\n");
       }
	 else if(ucValue==0x0b) 
	 { 
             	/* 接收数据成功 */
              printf("Motor run = %#x\r\n",ucQueueMsgValue);
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
			if(recoder_number.dir_change == 1) 
            {
                  Dir = Dir ;
				  printf("Dir = Dir is OK !!!!\r\n");
			}
			else 
			{
			    Dir = -Dir;
				printf("Dir = - Dir is OK #######\r\n");
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
*	函 数 名: vTaskCOTL
*	功能说明: 接收物理按键和数字按键的命令
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 3  
*
*********************************************************************************************************/
static void vTaskCOTL(void *pvParameters)
{
   
	//TickType_t xLastWakeTime;
	//const TickType_t xFrequency = 200;
   // MSG_T  *ptMsg; 
	uint8_t ucKeyCode=0,abc_s=0;
    uint8_t start_s =0,door_s = 0,wiper_s=0;
    BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(100); /* 设置最大等待时间为5ms */
	uint8_t ucControl=0;
	uint32_t ulValue;

	while(1)
    {
	      printf("vTaskCOTL-3 \r\n");
		  ucKeyCode = KEY_Scan(0);
		  xResult = xTaskNotifyWait(0x00000000,      
						          0xFFFFFFFF,      
						          &ulValue,        /* 存储ulNotifiedValue在ulvalue中 */
						          xMaxBlockTime);  /* 最大延迟时间 */
		
		if( xResult == pdPASS )
		{
			printf("xTaskNotfifyWait = %#x\r\n", ulValue);
			 if(ulValue==0x32)
				  {
                       recoder_number.dir_change ++;
		          if(recoder_number.dir_change == 1)
		          {
                     ucControl =0x0a;
					 recoder_number.break_f =0;
					 xTaskNotify(xHandleTaskBLDC,      /* 目标任务 */
					ucControl,              /* 发送数据 */
					eSetValueWithOverwrite);/* 上次目标任务没有执行，会被覆盖 */
				  }
				  else 
				  {
                     ucControl = 0x0b;
					 recoder_number.dir_change =0;
					 xTaskNotify(xHandleTaskBLDC,      /* 目标任务 */
					ucControl,              /* 发送数据 */
					eSetValueWithOverwrite);/* 上次目标任务没有执行，会被覆盖 */
				  }
                
		         	
                   

				  }
		}
		else
		{
			/* 3?ê± */
			LED1=0;
			LED2 =0 ;
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
				    start_s ++;
		          if((start_s == 1)||(recoder_number.break_f ==1))
		          {
                     ucControl =0x0b;
					 recoder_number.break_f =0;
					 xTaskNotify(xHandleTaskBLDC,      /* 目标任务 */
									ucControl,              /* 发送数据 */
									eSetValueWithOverwrite);/* 上次目标任务没有执行，会被覆盖 */
					
				  }
				  else 
				  {
                     ucControl = 0x0a;
					 start_s =0;
					  xTaskNotify(xHandleTaskBLDC,      /* 目标任务 */
									ucControl,              /* 发送数据 */
									eSetValueWithOverwrite);/* 上次目标任务没有执行，会被覆盖 */
					  
				  }
                 
				  break;
				  case DIR_PRES: //3

			      recoder_number.dir_change++;
	  			 PRINTF(" DIR_change = %d  \r\n", recoder_number.dir_change);
	  			 if(recoder_number.dir_change == 1)
	   				{
                       LED1 =0;
						LED2 =0;
				    }
				 else 
				   {
                      recoder_number.dir_change =0;
					  
				   }
			
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
			 	   door_s ++ ;
				   if(door_s ==1)
				   {
                      
                       ucControl = 0x03;
					
					  xTaskNotify(xHandleTaskSUBJ,      /* 目标任务 */
								   ucControl,              /* 发送数据 */
								   eSetValueWithOverwrite);/* 上次目标任务没有执行，会被覆盖 */
				   }
				   else
				   {
                     
                      ucControl = 0x04;
					 
					  xTaskNotify(xHandleTaskSUBJ,      /* 目标任务 */
								   ucControl,              /* 发送数据 */
								   eSetValueWithOverwrite);/* 上次目标任务没有执行，会被覆盖 */
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
				wiper_s ++;
				
				if(wiper_s ==1)
				{
                   WIPER_OUTPUT_2 = 0;
				   WIPER_OUTPUT_1 = 1;
				   ucControl = 0x05;
					
				   xTaskNotify(xHandleTaskSUBJ,      /* 目标任务 */
								   ucControl,              /* 发送数据 */
								   eSetValueWithOverwrite);/* 上次目标任务没有执行，会被覆盖 */
				   }
				
				else if(wiper_s ==2)
				{
                   wiper_s = 0;
				   WIPER_OUTPUT_1 = 0;   
			       WIPER_OUTPUT_2  = 1;
				    ucControl = 0x06;
					
					xTaskNotify(xHandleTaskSUBJ,      /* 目标任务 */
								   ucControl,              /* 发送数据 */
								   eSetValueWithOverwrite);/* 上次目标任务没有执行，会被覆盖 */
				 }
				else if(wiper_s == 3)
			    {
					WIPER_OUTPUT_1 = 1;
					WIPER_OUTPUT_2 = 1;
                    ucControl = 0x06;
					
					xTaskNotify(xHandleTaskSUBJ,      /* 目标任务 */
								   ucControl,              /* 发送数据 */
								   eSetValueWithOverwrite);/* 上次目标任务没有执行，会被覆盖 */
					
				}
				else
				{
                    wiper_s == 0;
					ucControl = 0x07;
					
					xTaskNotify(xHandleTaskSUBJ,      /* 目标任务 */
								ucControl,              /* 发送数据 */
								eSetValueWithOverwrite);/* 上次目标任务没有执行，会被覆盖 */
					
				}
                 
			
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
        }
        taskYIELD();//   vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
    taskYIELD();//   vTaskDelayUntil(&xLastWakeTime, xFrequency);
   }//end whilt(1)
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

	xTaskCreate( vTaskSUBJ,    									/* 任务函数  */
                 "vTaskBLDC",  									/* 任务名    */
                 configMINIMAL_STACK_SIZE + 166,         		/* stack大小，单位word，也就是4字节 */
                 NULL,        									/* 任务参数  */
                 tskIDLE_PRIORITY+2,           					/* 任务优先级*/
                 &xHandleTaskSUBJ); 							/* 任务句柄  */
	
	xTaskCreate( vTaskBLDC,    									/* 任务函数  */
                 "vTaskBLDC",  									/* 任务名    */
                 configMINIMAL_STACK_SIZE + 934,         		/* stack大小，单位word，也就是4字节 */
                 NULL,        									/* 任务参数  */
                 tskIDLE_PRIORITY+3,           					/* 任务优先级*/
                 &xHandleTaskBLDC); 							/* 任务句柄  */

	xTaskCreate( vTaskCOTL,    									/* 任务函数  */
                 "vTaskCOTL",  									/* 任务名    */
                 configMINIMAL_STACK_SIZE + 166,         		/* stack大小，单位word，也就是4字节 */
                 NULL,        									/* 任务参数  */
                 tskIDLE_PRIORITY+4,           					/* 任务优先级*/
                 &xHandleTaskCOTL); 							/* 任务句柄  */

}
/********************************************************************
 *
 *	函 数 名: AppObjCreate
 *	功能说明: 创建任务通信机制
 *	形    参: 无
 *	返 回 值: 无
 *
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

	recoder_number.break_f =1;
	                  
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}
#endif 


