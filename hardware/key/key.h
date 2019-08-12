#ifndef __KEY_H__
#define __KEY_H__

#include "fsl_gpio.h"
#include "fsl_port.h"
#include "clock_config.h"
#include "MKV46F16.h"

typedef enum
{
  KEY_NONE =0,
  KEY_PRESS,
}KEY_ENUM;


#define KEY1_GPIO             GPIOE
#define KEY2_GPIO             GPIOE 
#define KEY3_GPIO             GPIOE
#define KEY4_GPIO             GPIOE
#define KEY5_GPIO             GPIOA
#define KEY6_GPIO             GPIOA
#define KEY7_GPIO             GPIOA
#define KEY8_GPIO             GPIOA
#define KEY9_GPIO             GPIOA
#define KEY10_GPIO            GPIOA

#define KEY1_GPIO_PIN              29
#define KEY2_GPIO_PIN              30
#define KEY3_GPIO_PIN              24
#define KEY4_GPIO_PIN              25
#define KEY5_GPIO_PIN              1
#define KEY6_GPIO_PIN              2
#define KEY7_GPIO_PIN              4
#define KEY8_GPIO_PIN              5
#define KEY9_GPIO_PIN              12
#define KEY10_GPIO_PIN             13

#define KEY1          PEin(29)  //定义PTE端口的26引脚为输入
#define KEY2          PEin(30)
#define KEY3          PEin(24)
#define KEY4          PEin(25)
#define KEY5          PAin(1)
#define KEY6          PAin(2)
#define KEY7          PAin(4)
#define KEY8          PAin(5)
#define KEY9          PAin(12)
#define KEY10         PAin(13)





#define KEY1_PRES       1      //PTE29
#define KEY2_PRES       2      //PTE30
#define KEY3_PRES       3      //PTE24,
#define KEY4_PRES       4      //PTE25,
#define KEY5_PRES       5      //PTA1,
#define KEY6_PRES       6      //PTA2,
#define KEY7_PRES       7      //PTA4,
#define KEY8_PRES       8      //PTA5,
#define KEY9_PRES       9      //PTA12,
#define KEY10_PRES     10    //PTA13,


/*初始化按键*/
void KEY_Init(void);
/* 按键扫描程序 */
uint8_t KEY_Scan(uint8_t mode);




#endif 