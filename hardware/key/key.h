#ifndef __KEY_H__
#define __KEY_H__

#include "fsl_gpio.h"
#include "fsl_port.h"
#include "clock_config.h"
#include "MKV46F16.h"

//首先定义位操作的宏定义
//#define KEY1  PEin(29)  //定义PTE端口的26引脚为输入
//#define KEY2  PEin(30)
//#define LED1  PDout(7)  //定义PTE端口的6引脚输出控制

#define KEY1_GPIO             GPIOA
#define KEY2_GPIO             GPIOE 
#define KEY3_GPIO             GPIOE
#define KEY4_GPIO             GPIOA
#define KEY5_GPIO             GPIOA
#define KEY6_GPIO             GPIOA
#define KEY7_GPIO             GPIOA
#define KEY8_GPIO             GPIOA
#define KEY9_GPIO             GPIOE
#define KEY10_GPIO            GPIOE

#define KEY1_GPIO_PIN              1//29
#define KEY2_GPIO_PIN              25//30
#define KEY3_GPIO_PIN              24//24
#define KEY4_GPIO_PIN              12//25
#define KEY5_GPIO_PIN              13
#define KEY6_GPIO_PIN              2
#define KEY7_GPIO_PIN              4
#define KEY8_GPIO_PIN              5
#define KEY9_GPIO_PIN              30
#define KEY10_GPIO_PIN             29

#define KEY1          PAin(1)   //刹车按键 - 按一次刹车，再按一次解除刹车
#define KEY2          PEin(25)  //起停按键，电机 //PEin(30)  //雨刷器按键，按一次一档，再按一次二档，再按一下关闭
#define KEY3          PEin(24)  //方向按键，电机
#define KEY4          PAin(12)  //数字加速
#define KEY5          PAin(13)  //数字减速
#define KEY6          PAin(2)   //车门按键，
#define KEY7          PAin(4)   //有无霍尔切换，按键
#define KEY8          PAin(5)   //车轮探测按键
#define KEY9          PEin(30)  //雨刷器按键，按一次一档，再按一次二档，再按一下关闭
#define KEY10         PEin(29)  //空调按键，按一次开，再按一次关闭//PAin(13)  





#define KEY1_PRES       1      //PTA1
#define KEY2_PRES       2      //PTE25
#define KEY3_PRES       3      //PTE24,
#define KEY4_PRES       4      //PTA12,
#define KEY5_PRES       5      //PTA13,
#define KEY6_PRES       6      //PTA2,
#define KEY7_PRES       7      //PTA4,
#define KEY8_PRES       8      //PTA5,
#define KEY9_PRES       9      //PTE30,
#define KEY10_PRES     10      //PTE29,


/*初始化按键*/
void KEY_Init(void);
/* 按键扫描程序 */
uint8_t KEY_Scan(uint8_t mode);




#endif 