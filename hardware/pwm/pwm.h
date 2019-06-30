#ifndef __PWM_H__
#define __PWM_H__

#include "fsl_pwm.h"
#include "fsl_xbara.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"

/* The PWM base address */
#define BOARD_PWM_BASEADDR PWMA

#define PWM_SRC_CLK_FREQ CLOCK_GetFreq(kCLOCK_FastPeriphClk) //84Mhz 

void PWM_BLDC_Init(void);






#endif 
