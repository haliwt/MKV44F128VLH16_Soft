#ifndef __OUTPUT_H__
#define __OUTPUT_H__


#include "fsl_gpio.h"
#include "fsl_port.h"

#define A_POWER_OUTPUT         PCout(10)
#define B_POWER_OUTPUT         PCout(11)
#define C_POWER_OUTPUT         PCout(0)



#define AIR_OUTPUT               PEout(19)   //空调输出 
#define WIPER_OUTPUT_2           PEout(18)   //雨刮器输出二档
#define WIPER_OUTPUT_1           PEout(17)   //雨刮器输出一档
#define DOOR_OUTPUT              PAout(19)   //车门输出
#define WHEEL_OUTPUT             PBout(0)    //车轮输出


#define OUTPUT_AIR_GPIO          GPIOE
#define OUTPUT_WIPER_2_GPIO      GPIOE
#define OUTPUT_WIPER_1_GPIO      GPIOE 
#define OUTPUT_DOOR_GPIO         GPIOA 
#define OUTPUT_WHEEL_GPIO        GPIOB



#define OUTPUT_AIR_GPIO_PIN           19
#define OUTPUT_WIPER_2_GPIO_PIN       18
#define OUTPUT_WIPER_1_GPIO_PIN       17
#define OUTPUT_DOOR_GPIO_PIN          19
#define OUTPUT_WHEEL_GPIO_PIN         0

typedef struct _output_t_
{
  uint8_t start_number;
  uint8_t air_number;
  uint8_t wiper_number;
  uint8_t door_number;
  uint8_t wheel_number;
  uint8_t dir_change;
  uint8_t break_f;

}output_t;




void OUTPUT_Fucntion_Init(void);
void AIR_OUTPUT_F(void);
void WIPER_OUTPUT_F(void);
void DOOR_OUTPUT_F(void);
void WHEEL_OUTPUT_F(void);
void ABC_POWER_OUTPUT_Init(void);


#endif 