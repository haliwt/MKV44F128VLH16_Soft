#ifndef __INPUT_H_
#define __INPUT_H_

#include "fsl_gpio.h"
#include "fsl_port.h"
#include "clock_config.h"

#define SD315AI_SO12_A_INPUT    PEin(19)
#define SD315AI_SO12_B_INPUT    PEin(18)
#define SD315AI_SO12_C_INPUT    PEin(17)

#define SD315AI_SO12_A_GPIO    GPIOE//GPIOC 
#define SD315AI_SO12_B_GPIO    GPIOE//GPIOC
#define SD315AI_SO12_C_GPIO    GPIOE//GPIOC

#define SD315AI_SO12_A_PIN     19//7U
#define SD315AI_SO12_B_PIN     18//6U
#define SD315AI_SO12_C_PIN     17//4U



void SD315AI_SO12_Init(void);




#endif 