#ifndef __HALL_H__
#define __HALL_H__

#include "fsl_gpio.h"
#include "fsl_port.h"
#include "bldc.h"
#include "adc.h"

#define NO_HALL_A_GPIO      GPIOB  //PTB18
#define NO_HALL_B_GPIO      GPIOC  //PTC8
#define NO_HALL_C_GPIO      GPIOE  //PTE16

#define NO_HALL_A_GPIO_PIN      18U
#define NO_HALL_B_GPIO_PIN       8U
#define NO_HALL_C_GPIO_PIN      16U

#define NO_HALL_A_PORT        PORTB
#define NO_HALL_B_PORT        PORTC
#define NO_HALL_C_PORT        PORTE

#define NO_HALL_A_IRQ        PORTB_IRQn
#define NO_HALL_B_IRQ        PORTC_IRQn
#define NO_HALL_C_IRQ        PORTE_IRQn


#define  NO_HALL_A_IRQ_HANDLER      PORTB_IRQHandler
#define  NO_HALL_B_IRQ_HANDLER      PORTC_IRQHandler
#define  NO_HALL_C_IRQ_HANDLER      PORTE_IRQHandler


extern __IO uint8_t step;

extern __IO uint8_t SENSE_H;




extern __IO uint8_t Phase  ;

#define NOPRINT    0


void NO_HALL_GPIO_Init(void);

uint8_t NO_HallSensor_GetPinState(void);

uint8_t NO_HallSensor_Hex(void);

void Dector_Phase(uint8_t duty);



#endif 
