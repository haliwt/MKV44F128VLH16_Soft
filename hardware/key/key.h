#ifndef __KEY_H__
#define __KEY_H__

#include "fsl_gpio.h"
#include "fsl_port.h"
#include "clock_config.h"
#include "led.h"


/* ����һ������----*/
typedef enum
{
  KEY_UP   = 0,
  KEY_DOWN = 1,
}KEYState_TypeDef;
  

#define BRAKE_KEY_GPIO            	GPIOA
#define BRAKE_KEY_PORT            	PORTA

#define START_KEY_GPIO            	GPIOE 
#define START_KEY_PORT              PORTE


#define DIR_GPIO             		GPIOE
#define KEY4_GPIO             		GPIOA
#define KEY5_GPIO             		GPIOA
#define KEY6_GPIO             		GPIOA
#define KEY7_GPIO             		GPIOA
#define KEY8_GPIO             		GPIOA
#define KEY9_GPIO             		GPIOE
//#define AIR_GPIO            		GPIOE
#define ABC_POWER_GPIO              GPIOE//GPIOC

#define BRAKE_KEY_GPIO_PIN              1//29
#define START_KEY_GPIO_PIN              25//30
#define DIR_GPIO_PIN              		24//24
#define KEY4_GPIO_PIN              		12//25
#define KEY5_GPIO_PIN              		13
#define KEY6_GPIO_PIN              		2
#define KEY7_GPIO_PIN              		4
#define KEY8_GPIO_PIN              		5
#define KEY9_GPIO_PIN              		30
//#define AIR_GPIO_PIN             		29
#define ABC_POWER_GPIO_PIN       		29//3


#define BRAKE_KEY               PAin(1)   //ɲ������ - ��һ��ɲ�����ٰ�һ�ν��ɲ��
#define START_KEY               PEin(25)  //��ͣ��������� //PEin(30)  //��ˢ����������һ��һ�����ٰ�һ�ζ������ٰ�һ�¹ر�
#define DIR_KEY                 PEin(24)  //���򰴼������ DIR_KEY3
#define DIGITAL_ADD_KEY         PAin(12)  //���ּ���     DIGITAL_ADD_KEY
#define DIGITAL_REDUCE_KEY      PAin(13)  //���ּ���
#define DOOR_KEY          		PAin(2)   //���Ű�����
#define HALL_SWITCH_KEY         PAin(4)   //���޻����л�������
#define WHEEL_KEY          		PAin(5)   //����̽�ⰴ��
#define WIPERS_KEY          	PEin(30)  //��ˢ����������һ��һ�����ٰ�һ�ζ������ٰ�һ�¹ر�
//#define AIR_KEY         		PEin(29)  //�յ���������һ�ο����ٰ�һ�ιر�//PAin(13)  
#define ABC_POWER_KEY       	PEin(29)




//#define BRAKE_PRES              1      //PTA1
#define ABC_POWER_PRES          1      //
#define START_PRES              2      //PTE25
#define DIR_PRES                3      //PTE24, DIR KEY
#define DIGITAL_ADD_PRES        4      //PTA12,
#define DIGITAL_REDUCE_PRES     5      //PTA13,
#define DOOR_PRES               6      //PTA2,
#define HALL_PRES               7      //PTA4,
#define WHEEL_PRES              8      //PTA5,
#define WIPERS_PRES             9      //PTE30,
#define AIR_PRES                10      //PTE29,


#define BRAKE_KEY_IRQ           PORTA_IRQn  //interrupt number
//#define START_KEY_IRQ           PORTE_IRQn

#define BARKE_KEY_IRQ_HANDLER      PORTA_IRQHandler   //BCD;Pin detect (Port E)--'startup_MKV46F16.s'  
//#define START_KEY_IRQ_HANDLER   PORTE_IRQHandler   //DCD -PORTE_IRQHandler ;Pin detect (Port E)-'startup_MKV46F16.s' 

/*��ʼ������*/
void KEY_Init(void);
/* ����ɨ����� */
uint8_t KEY_Scan(uint8_t mode);


KEYState_TypeDef Start_Key_StateRead(void);

#endif 