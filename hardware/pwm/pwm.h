#ifndef __PWM_H__
#define __PWM_H__

#include "fsl_pwm.h"
#include "fsl_xbara.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"




#define PWM_SRC_CLK_FREQ CLOCK_GetFreq(kCLOCK_FastPeriphClk) //84Mhz 
/* The PWM base address */
#define BOARD_PWM_BASEADDR PWMA

#define HALL_A_GPIO          GPIOB
#define HALL_B_GPIO          GPIOB
#define HALL_C_GPIO          GPIOB

#define HALL_A_GPIO_PIN       1
#define HALL_B_GPIO_PIN       2
#define HALL_C_GPIO_PIN       3

#define HALL_A          PBin(1)
#define HALL_B          PBin(2)
#define HALL_C          PBin(3)




#define CCW                              (int32_t)(1)
#define CW                               (int32_t)(-1)

#define  PWMA_INIT_ON                    1

/* 无刷马达定义 ------------------------------------------------------------------*/
typedef struct {
  __IO int32_t  uwStep ;  	    // 马达霍尔步数
  __IO int32_t  Dir ;        	    // 方向
  __IO int32_t  Position;           // 位置
  __IO int32_t  Speed;		    // 速度
  __IO uint16_t PWM_Duty; 	    // PWM 占空比
  __IO int32_t  Hall_Period;        // 霍尔周期
  __IO int32_t  Hall_PulNum;       // 霍尔记数
  __IO int32_t  Lock_Time;         // 堵转电流
}BLDC_Typedef;

extern BLDC_Typedef BLDCMotor ;

extern __IO int32_t  uwStep ;

extern __IO uint32_t Lock_Time; // 堵转时间
extern __IO float   PWM_Duty;
extern __IO int32_t Dir;
extern uint16_t SPEED_VALUE ;   //速度值



typedef enum
{
  GPIO_PIN_RESET = 0,
  GPIO_PIN_SET
}GPIO_PinState;

void HALL_Init(void);

uint32_t HallSensor_GetPinState(void);

void HALLSensor_Detected_BLDC(uint32_t uvw);


void PWM_BLDC_Init(void);   // initial pwm function

void PWMA_Select_A_Channel(uint8_t s_pwmA);  //select one PWM A channel,H-bridge first 
void PWMA_Select_B_Channel(uint8_t s_pwmB);  //select one PWM B  channel,H-bridge second
void PWMA_Select_C_Channel(uint8_t s_pwmC); 



void PWMA_Close_ABC_Channel(uint8_t f_pwma);






#endif 
