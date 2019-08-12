#ifndef __POLLINGUSART_H__
#define __POLLINGUSART_H__ 

#include "board.h"
#include "fsl_uart.h"

#include "pin_mux.h"
#include "clock_config.h"


#define DEMO_UART UART1
#define DEMO_UART_CLKSRC kCLOCK_FastPeriphClk
#define DEMO_UART_CLK_FREQ CLOCK_GetFreq(kCLOCK_FastPeriphClk)

/*******************************************************************************
 * Variables
 ******************************************************************************/

//uint8_t pollingtxbuff[]   = "Uart polling example\r\nBoard will send back received characters\r\n";
//uint8_t pollingrxbuff[20] = {0};

void POLLING_USART_Init(void);


#endif 