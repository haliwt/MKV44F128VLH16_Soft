#ifndef __POLLINGUSART_H__
#define __POLLINGUSART_H__ 

#include "board.h"
#include "fsl_uart_edma.h"
#include "pin_mux.h"
#include "clock_config.h"

#include "fsl_dmamux.h"


/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_UART    					UART1
#define DEMO_UART_CLKSRC 				kCLOCK_FastPeriphClk
#define DEMO_UART_CLK_FREQ 				CLOCK_GetFreq(kCLOCK_FastPeriphClk)
#define UART_TX_DMA_CHANNEL 			0U
#define UART_RX_DMA_CHANNEL 			1U
#define EXAMPLE_UART_DMAMUX_BASEADDR 	        DMAMUX0
#define EXAMPLE_UART_DMA_BASEADDR 		DMA0
#define UART_TX_DMA_REQUEST 			kDmaRequestMux0UART1Tx
#define UART_RX_DMA_REQUEST 			kDmaRequestMux0UART1Rx
#define ECHO_BUFFER_LENGTH 			    8




/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/

void USART_EDMA_Init(void);
/* UART user callback */
void UART_UserCallback(UART_Type *base, uart_edma_handle_t *handle, status_t status, void *userData);

void USART_EDMA_RW_Function(void);

#endif 
