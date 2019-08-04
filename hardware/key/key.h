#ifndef __KEY_H__
#define __KEY_H__

#include "fsl_gpio.h"
#include "fsl_port.h"
#include "clock_config.h"
#include "MKV46F16.h"

//���ȶ���λ�����ĺ궨��
//#define BRAKE  PEin(29)  //����PTE�˿ڵ�26����Ϊ����
//#define START  PEin(30)
//#define LED1  PDout(7)  //����PTE�˿ڵ�6�����������

#define BRAKE_GPIO            GPIOA
#define START_GPIO             GPIOE 
#define KEY3_GPIO             GPIOE
#define KEY4_GPIO             GPIOA
#define KEY5_GPIO             GPIOA
#define KEY6_GPIO             GPIOA
#define KEY7_GPIO             GPIOA
#define KEY8_GPIO             GPIOA
#define KEY9_GPIO             GPIOE
#define KEY10_GPIO            GPIOE

#define BRAKE_GPIO_PIN              1//29
#define START_GPIO_PIN              25//30
#define KEY3_GPIO_PIN              24//24
#define KEY4_GPIO_PIN              12//25
#define KEY5_GPIO_PIN              13
#define KEY6_GPIO_PIN              2
#define KEY7_GPIO_PIN              4
#define KEY8_GPIO_PIN              5
#define KEY9_GPIO_PIN              30
#define KEY10_GPIO_PIN             29

#define BRAKE         PAin(1)   //ɲ������ - ��һ��ɲ�����ٰ�һ�ν��ɲ��
#define START         PEin(25)  //��ͣ��������� //PEin(30)  //��ˢ����������һ��һ�����ٰ�һ�ζ������ٰ�һ�¹ر�
#define KEY3          PEin(24)  //���򰴼������
#define KEY4          PAin(12)  //���ּ���
#define KEY5          PAin(13)  //���ּ���
#define KEY6          PAin(2)   //���Ű�����
#define KEY7          PAin(4)   //���޻����л�������
#define KEY8          PAin(5)   //����̽�ⰴ��
#define KEY9          PEin(30)  //��ˢ����������һ��һ�����ٰ�һ�ζ������ٰ�һ�¹ر�
#define KEY10         PEin(29)  //�յ���������һ�ο����ٰ�һ�ιر�//PAin(13)  





#define BRAKE_PRES       1      //PTA1
#define START_PRES       2      //PTE25
#define KEY3_PRES       3      //PTE24,
#define KEY4_PRES       4      //PTA12,
#define KEY5_PRES       5      //PTA13,
#define KEY6_PRES       6      //PTA2,
#define KEY7_PRES       7      //PTA4,
#define KEY8_PRES       8      //PTA5,
#define KEY9_PRES       9      //PTE30,
#define KEY10_PRES     10      //PTE29,


/*��ʼ������*/
void KEY_Init(void);
/* ����ɨ����� */
uint8_t KEY_Scan(uint8_t mode);




#endif 