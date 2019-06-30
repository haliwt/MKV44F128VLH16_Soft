#ifndef __HALL_H__
#define __HALL_H__

#include "fsl_gpio.h"
#include "fsl_port.h"

#define HALL_A   PEin(16)
#define HALL_B   PCin(6)
#define HALL_C   PBin(3)

void HALL_Init(void);






#endif 
