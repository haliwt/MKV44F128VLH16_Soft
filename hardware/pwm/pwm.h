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

#define HALL_A          PEin(16)
#define HALL_B          PCin(6)
#define HALL_C          PBin(3)


#define CCW                              (int32_t)(1)
#define CW                               (int32_t)(-1)

#define  PWMA_INIT_ON                    1

/* 电机参数定义 ------------------------------------------------------------------*/
typedef struct {
  __IO int32_t  uwStep ;  	    // 电机旋转状态
  __IO int32_t  Dir ;        	    // 电机旋转方向
  __IO int32_t  Position;           // 电机位置
  __IO int32_t  Speed;		    // 电机速度
  __IO uint16_t PWM_Duty; 	    // 占空比
  __IO int32_t  Hall_Period;        // 霍尔传感器触发周期
  __IO int32_t  Hall_PulNum;       // 霍尔传感器脉冲个数
  __IO int32_t  Lock_Time;         // 电机堵转时间
}BLDC_Typedef;

extern BLDC_Typedef BLDCMotor ;

extern __IO int32_t  uwStep ;

extern __IO uint32_t Lock_Time; // 电机堵转时间
extern __IO float   PWM_Duty;
extern __IO int32_t Dir;
extern uint16_t SPEED_VALUE ;   //电机的速度值



typedef enum
{
  GPIO_PIN_RESET = 0,
  GPIO_PIN_SET
}GPIO_PinState;

void HALL_Init(void);

uint32_t HallSensor_GetPinState(void);

void HALLSensor_Detected_BLDC(void);


void PWM_BLDC_Init(void);   // initial pwm function
void PWMABC_Select_Channel(uint8_t s_pwma);  //select one PWM channel
void PWMABC_Close_Channel(uint8_t f_pwma);   //Close one PWM channel 







#endif 
