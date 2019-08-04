#ifndef __OUTPUT_H__
#define __OUTPUT_H__


#include "fsl_gpio.h"
#include "fsl_port.h"

#define AIR               PEout(19)   //空调输出 
#define WIPER_2           PEout(18)   //雨刮器输出二档
#define WIPER_1           PEout(17)   //雨刮器输出一档
#define DOOR              PAout(19)   //车门输出
#define WHEEL             PBout(0)    //车轮输出


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

typedef struct _output_t
{
  uint8_t air_number;
  uint8_t wiper_number;
  uint8_t door_number;
  uint8_t wheel_number;

}output_t;




void OUTPUT_Fucntion_Init(void);
void AIR_OUTPUT_F(void);
void WIPER_OUTPUT_F(void);
void DOOR_OUTPUT_F(void);
void WHEEL_OUTPUT_F(void);


#endif 