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
											��������
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
        printf("vTaskUSART-1 \r\n");
        DelayMs(1);
		//UART_ReadBlocking(DEMO_UART, ch, 1);
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
        
        if(ch[0] == 0x01) //'1' = 0x31
         {
           
		      xTaskNotify(xHandleTaskCOTL,      /* Ŀ������ */
								ptMsg->usData[2],              /* �������� */
								eSetValueWithOverwrite);/* �ϴ�Ŀ������û��ִ�У��ᱻ���� */

        #if 0
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
		#endif 
        }
		
	//	vTaskDelay(xMaxBlockTime);
	 //vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}
/*********************************************************************************************************
*
*	�� �� ��: vTaskSUBJ
*	����˵��: ���������������ְ���������
*	��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
*   �� �� ��: 2  
*
*********************************************************************************************************/
static void vTaskSUBJ(void *pvParameters)
{
     uint32_t vlSubj;
     volatile uint8_t ucConKeyValue;
	 BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(200); /* �������ȴ�ʱ��Ϊ5ms */

	while(1)
    {
          printf("vTaskSUBJ-2 \r\n");
		
		  xResult = xTaskNotifyWait(0x00000000,      
						          0xFFFFFFFF,      
						          &vlSubj,        /* �洢ulNotifiedValue��ulvalue�� */
						          xMaxBlockTime);  /* ����ӳ�ʱ�� */
          
		  
		if( xResult == pdPASS )
		{
			
             ucConKeyValue = (uint8_t)vlSubj;
             printf("vTaskSUBJ vlSubj = %#x\r\n", ucConKeyValue);
             /*******************ABC_POWER_F*************************/
			 if(ucConKeyValue==0x01)
			 {
                 A_POWER_OUTPUT =1;
				 B_POWER_OUTPUT =1;
				 C_POWER_OUTPUT =1;
		        printf("ABC_= 1 @@@@~~~@@@\r\n");  
		     }
			 if(ucConKeyValue==0x00)
			 {
                 A_POWER_OUTPUT =0;
				 B_POWER_OUTPUT =0;
				 C_POWER_OUTPUT =0;
		        printf("ABC_ = 0 @@@@@@@~~~\r\n");  
		     }
			 /*******************Door_F*************************/
			 if(ucConKeyValue==0x09)
			 {
                DOOR_OUTPUT =1;//door open
		        printf("DOOR_OUTPUT = 1 @@@@@@@\r\n");  
		     }
             if(ucConKeyValue==0x08)
			 {
                DOOR_OUTPUT =0;//door open
		        printf("DOOR_OUTPUT = 0 ~~~~~~~\r\n");     
		     }
			 /*******************WIPERS_F***************************/
             if(ucConKeyValue == 0x0e)
	         {
	  			 WIPER_OUTPUT_2 = 0;
	  			 WIPER_OUTPUT_1 = 1;
	  			 printf("WIPERS  = 1 @@@@~~~~\r\n");   
	         }
			 if(ucConKeyValue == 0x0f)
			 {
                 WIPER_OUTPUT_1 = 1;   
			     WIPER_OUTPUT_2  = 0;
				printf("WIPERS  = 2 ~~~~@@@@\r\n");
			 }
			 if(ucConKeyValue == 0x10)
			 {
                 WIPER_OUTPUT_1 = 1;   
			     WIPER_OUTPUT_2  = 1;
				printf("WIPERS  = 3 @@@~~~~@@@\r\n");
			 }
			 if(ucConKeyValue == 0x11)
			 {
                 WIPER_OUTPUT_1 = 0;   
			     WIPER_OUTPUT_2  = 0;
				printf("WIPERS  = 0 @@@~~~~@@@~~~~\r\n");
			 }
		     /*******************AIR_F***************************/   
              if(ucConKeyValue == 0x12)
			 {
                 AIR_OUTPUT = 1;
				printf("AIR  = 1 ~~@@@~~\r\n");
			 }
              if(ucConKeyValue == 0x13)
			 {
                 AIR_OUTPUT = 0;
				printf("AIR  = 0 ~~@@@~~@@@~~~~@@@\r\n");
			 }
        }
		else
		{
			/* 3?���� */
			LED1=0;
			LED2 =0 ;
		}


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
    
    volatile uint8_t ucValue;
	TickType_t xLastWakeTime;
	
	const TickType_t xFrequency = 100;
    xLastWakeTime = xTaskGetTickCount();
    volatile uint16_t pwm_f=0;
	uint16_t sampleMask;
	BaseType_t xResult;
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS(200); /* �������ȴ�ʱ��Ϊ300ms */
	uint32_t ucConValue;
	while(1)
    {       
      printf("vTaskBLDC-2 \r\n");  

        xResult = xTaskNotifyWait(0x00000000,      
						          0xFFFFFFFF,      
						          &ucConValue,        /* �洢ulNotifiedValue��ulvalue�� */
						          xMaxBlockTime);  /* ����ӳ�ʱ�� */
	  if(xResult == pdPASS)
		{
			/* �������ݳɹ� */
          printf("vTaskBLDC ConmessageID = %#x\r\n",ucConValue );
		  ucValue = (uint8_t)ucConValue ;
		}
		else
		{
			/* ��ʱ */
          LED1= !LED1;
		}
	  if((ucValue==0x02)|| (recoder_number.break_f ==1))//ɲ��
	  {
         taskENTER_CRITICAL(); //�����ٽ�״̬
		 PMW_AllClose_ABC_Channel();
         PMW_AllClose_ABC_Channel();
		 PWM_StopTimer(BOARD_PWM_BASEADDR,  kPWM_Control_Module_0);
		 PWM_StopTimer(BOARD_PWM_BASEADDR,  kPWM_Control_Module_1);
		 PWM_StopTimer(BOARD_PWM_BASEADDR,  kPWM_Control_Module_2);
		 if(recoder_number.break_f ==1)
		 {
		     A_POWER_OUTPUT =0;
			 B_POWER_OUTPUT =0;
			 C_POWER_OUTPUT =0;	 
		  }
		 taskEXIT_CRITICAL(); //�˳��ٽ�״̬
		 
		 printf("Break is OK $$$$$$$$$$$$$\r\n");
       }
	 else if(ucValue==0x03) 
	 { 
             	/* �������ݳɹ� */
             printf("Motor run = %#x\r\n",ucConValue);
			 printf("Motor Run is OK !!!!\r\n");

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
*   �� �� ��: 4  
*
*********************************************************************************************************/
static void vTaskCOTL(void *pvParameters)
{
   
    uint8_t ucKeyCode=0,abc_s=0,digital_s=0;
    uint8_t start_s =0,door_s = 0,wiper_s=0,air_s=0;

    BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(200); /* �������ȴ�ʱ��Ϊ5ms */
	uint8_t ucControl=0,ulValue;
	uint32_t rlValue;
	

	while(1)
    {
	      printf("vTaskCOTL-3 \r\n");
		  ucKeyCode = KEY_Scan(0);
		  xResult = xTaskNotifyWait(0x00000000,      
						          0xFFFFFFFF,      
						          &rlValue,        /* �洢ulNotifiedValue��ulvalue�� */
						          xMaxBlockTime);  /* ����ӳ�ʱ�� */
		
		if( xResult == pdPASS )
		{
            ulValue=(uint8_t)rlValue;
			printf("xTaskNotfifyWait = %#x\r\n", ulValue);
			/****************digitalkey coder******************/
			 if(ulValue == 0x01)
			 {
                ucKeyCode =ABC_POWER_PRES  ; 
			 }
			 if(ulValue == 0x2)
			 {
                ucKeyCode =START_PRES  ; 
			 }
			 if(ulValue == 0x4)
			 {
                ucKeyCode =DIR_PRES  ; 
			 }
			 if(ulValue == 0x07)
			 {
                ucKeyCode =DIGITAL_ADD_PRES  ; 
			 }
			 if(ulValue == 0x06)
			 {
                ucKeyCode =DIGITAL_REDUCE_PRES  ; 
			 }
			 if(ulValue == 0x09)
			 {
                ucKeyCode =DOOR_PRES ; 
			 }
			 if(ulValue == 0x0a)
			 {
                ucKeyCode =HALL_PRES ; 
			 }
			 if(ulValue == 0x0c)
			 {
                ucKeyCode =WHEEL_PRES ; 
			 }
			  if(ulValue == 0x0e)
			 {
                ucKeyCode =WIPERS_PRES ; 
			 }
			  if(ulValue == 0x12)
			 {
                ucKeyCode =AIR_PRES ; 
			 }
			 
		}
		else
		{
			/* 3?���� */
			LED1=0;
			LED2 =0 ;
		}

		  
		if(ucKeyCode !=KEY_UP) 
				
			{
               switch(ucKeyCode)//if(ptMsg->ucMessageID == 0x32)
                { 
                
                  case ABC_POWER_PRES :

				  PRINTF("ABC_PRES key \r\n");
				     abc_s++;
				  if(abc_s ==1)
				  	{
                       ucControl = 0x01;
                       xTaskNotify(xHandleTaskSUBJ,      /* Ŀ������ */
                                   ucControl,              /* �������� */
                                   eSetValueWithOverwrite);/* �ϴ�Ŀ������û��ִ�У��ᱻ���� */
                     }
					else 
					{
                     ucControl = 0x00;
                     abc_s =0 ;
                     xTaskNotify(xHandleTaskSUBJ,      /* Ŀ������ */
                           ucControl,              /* �������� */
                           eSetValueWithOverwrite);/* �ϴ�Ŀ������û��ִ�У��ᱻ���� */

					}
				  	break;



				 case START_PRES:
                   PRINTF("START_PRES key \r\n");
				    start_s ++;
		          if((start_s == 1)||(recoder_number.break_f ==1))
		          {
                     ucControl =0x03;
					 recoder_number.break_f =0;
					 xTaskNotify(xHandleTaskBLDC,      /* Ŀ������ */
									ucControl,              /* �������� */
									eSetValueWithOverwrite);/* �ϴ�Ŀ������û��ִ�У��ᱻ���� */
					
				  }
				  else 
				  {
                     ucControl = 0x02;
					 start_s =0;
					  xTaskNotify(xHandleTaskBLDC,      /* Ŀ������ */
									ucControl,              /* �������� */
									eSetValueWithOverwrite);/* �ϴ�Ŀ������û��ִ�У��ᱻ���� */
					  
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
                  digital_s ++;
				 if(digital_s ==1)
				  	{
                       ucControl = 0x01;
                       xTaskNotify(xHandleTaskSUBJ,      /* Ŀ������ */
                                   ucControl,              /* �������� */
                                   eSetValueWithOverwrite);/* �ϴ�Ŀ������û��ִ�У��ᱻ���� */
                     }
					else 
					{
                     ucControl = 0x00;
                     digital_s =0 ;
                     xTaskNotify(xHandleTaskSUBJ,      /* Ŀ������ */
                           ucControl,              /* �������� */
                           eSetValueWithOverwrite);/* �ϴ�Ŀ������û��ִ�У��ᱻ���� */

					}
				break;
				
			 case DIGITAL_REDUCE_PRES ://5
			 	PRINTF("DIGITAL_REDUCE_PRES key \r\n");
                digital_s ++;
				if(digital_s ==1)
				  	{
                       ucControl = 0x01;
                       xTaskNotify(xHandleTaskSUBJ,      /* Ŀ������ */
                                   ucControl,              /* �������� */
                                   eSetValueWithOverwrite);/* �ϴ�Ŀ������û��ִ�У��ᱻ���� */
                     }
					else 
					{
                     ucControl = 0x00;
                     digital_s =0 ;
                     xTaskNotify(xHandleTaskSUBJ,      /* Ŀ������ */
                           ucControl,              /* �������� */
                           eSetValueWithOverwrite);/* �ϴ�Ŀ������û��ִ�У��ᱻ���� */

					}
				break;
				
			 case DOOR_PRES ://6
			 	   PRINTF("DOOR_PRES key \r\n");
			 	   door_s ++ ;
				   if(door_s ==1)
				   {
                      
                       ucControl = 0x09;
					
					  xTaskNotify(xHandleTaskSUBJ,      /* Ŀ������ */
								   ucControl,              /* �������� */
								   eSetValueWithOverwrite);/* �ϴ�Ŀ������û��ִ�У��ᱻ���� */
				   }
				   else
				   {
                      door_s =0; 
                     ucControl = 0x08;
					  xTaskNotify(xHandleTaskSUBJ,      /* Ŀ������ */
								   ucControl,              /* �������� */
								   eSetValueWithOverwrite);/* �ϴ�Ŀ������û��ִ�У��ᱻ���� */
                    }
			 	   
				break;
				   
			 case HALL_PRES://7
			 	PRINTF("HALL_PRES key \r\n");
				  door_s ++ ;
				   if(door_s ==1)
				   {
                      
                       ucControl = 0x0b;
					
					  xTaskNotify(xHandleTaskSUBJ,      /* Ŀ������ */
								   ucControl,              /* �������� */
								   eSetValueWithOverwrite);/* �ϴ�Ŀ������û��ִ�У��ᱻ���� */
				   }
				   else
				   {
                      ucControl = 0x0a;
					  xTaskNotify(xHandleTaskSUBJ,      /* Ŀ������ */
								   ucControl,              /* �������� */
								   eSetValueWithOverwrite);/* �ϴ�Ŀ������û��ִ�У��ᱻ���� */
                   }
				break;
			 
			 case WHEEL_PRES : //8
			    PRINTF("WHEEL_PRES key \r\n");
				  door_s ++ ;
				   if(door_s ==1)
				   {
                      
                       ucControl = 0x0d;
					
					  xTaskNotify(xHandleTaskSUBJ,      /* Ŀ������ */
								   ucControl,              /* �������� */
								   eSetValueWithOverwrite);/* �ϴ�Ŀ������û��ִ�У��ᱻ���� */
				   }
				   else
				   {
                      ucControl = 0x0c;
					  xTaskNotify(xHandleTaskSUBJ,      /* Ŀ������ */
								   ucControl,              /* �������� */
								   eSetValueWithOverwrite);/* �ϴ�Ŀ������û��ִ�У��ᱻ���� */
                   }
			    break;
				
			 case WIPERS_PRES: //9�����
			  	PRINTF("WIPERS_PRES key \r\n");
				wiper_s ++;
				
				if(wiper_s ==1)
				{
                   ucControl = 0x0e;
				   xTaskNotify(xHandleTaskSUBJ,      /* Ŀ������ */
							   ucControl,              /* �������� */
							   eSetValueWithOverwrite);/* �ϴ�Ŀ������û��ִ�У��ᱻ���� */
				}
				else if(wiper_s ==2)
				{
                    ucControl = 0x0f;
					xTaskNotify(xHandleTaskSUBJ,      /* Ŀ������ */
								ucControl,              /* �������� */
								eSetValueWithOverwrite);/* �ϴ�Ŀ������û��ִ�У��ᱻ���� */
				 }
				else if(wiper_s == 3)
			    {
					ucControl = 0x10;
					xTaskNotify(xHandleTaskSUBJ,      /* Ŀ������ */
								ucControl,              /* �������� */
								eSetValueWithOverwrite);/* �ϴ�Ŀ������û��ִ�У��ᱻ���� */
				}
				else
				{
                    wiper_s == 0;
					ucControl = 0x11;
					xTaskNotify(xHandleTaskSUBJ,      /* Ŀ������ */
								ucControl,              /* �������� */
								eSetValueWithOverwrite);/* �ϴ�Ŀ������û��ִ�У��ᱻ���� */
				}
             break;
				
	         case AIR_PRES : //10 PE29
	         	PRINTF("AIR_PRES key \r\n");
	            air_s ++;
				        
				if(air_s ==1)
				{
			        ucControl = 0x12;
					xTaskNotify(xHandleTaskSUBJ,      /* Ŀ������ */
								ucControl,              /* �������� */
								eSetValueWithOverwrite);/* �ϴ�Ŀ������û��ִ�У��ᱻ���� */
				}	  
			    else 
			    {
		            air_s =0;
				    ucControl = 0x13;
					xTaskNotify(xHandleTaskSUBJ,      /* Ŀ������ */
								ucControl,              /* �������� */
								eSetValueWithOverwrite);/* �ϴ�Ŀ������û��ִ�У��ᱻ���� */
				  
				   
			    }
                break;
        }
        
	}
    taskYIELD();//   vTaskDelayUntil(&xLastWakeTime, xFrequency);
   }//end whilt(1)
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

	xTaskCreate( vTaskSUBJ,    									/* ������  */
                 "vTaskBLDC",  									/* ������    */
                 configMINIMAL_STACK_SIZE + 166,         		/* stack��С����λword��Ҳ����4�ֽ� */
                 NULL,        									/* �������  */
                 tskIDLE_PRIORITY+2,           					/* �������ȼ�*/
                 &xHandleTaskSUBJ); 							/* ������  */
	
	xTaskCreate( vTaskBLDC,    									/* ������  */
                 "vTaskBLDC",  									/* ������    */
                 configMINIMAL_STACK_SIZE + 934,         		/* stack��С����λword��Ҳ����4�ֽ� */
                 NULL,        									/* �������  */
                 tskIDLE_PRIORITY+3,           					/* �������ȼ�*/
                 &xHandleTaskBLDC); 							/* ������  */

	xTaskCreate( vTaskCOTL,    									/* ������  */
                 "vTaskCOTL",  									/* ������    */
                 configMINIMAL_STACK_SIZE + 166,         		/* stack��С����λword��Ҳ����4�ֽ� */
                 NULL,        									/* �������  */
                 tskIDLE_PRIORITY+4,           					/* �������ȼ�*/
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


