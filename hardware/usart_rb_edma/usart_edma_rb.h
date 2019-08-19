#ifndef __USART_EDMA_RB_H__
#define __USART_EDMA_RB_H__



#include "board.h"
#include "fsl_uart_edma.h"
#include "fsl_dmamux.h"
#include "timer.h"

#include "pin_mux.h"
#include "clock_config.h"

#define EXAMPLE_UART UART1
#define EXAMPLE_UART_CLKSRC kCLOCK_FastPeriphClk
#define EXAMPLE_UART_CLK_FREQ CLOCK_GetFreq(kCLOCK_FastPeriphClk)
#define UART_TX_DMA_CHANNEL 0U
#define UART_RX_DMA_CHANNEL 1U
#define EXAMPLE_UART_DMAMUX_BASEADDR DMAMUX0
#define EXAMPLE_UART_DMA_BASEADDR DMA0
#define UART_TX_DMA_REQUEST kDmaRequestMux0UART1Tx
#define UART_RX_DMA_REQUEST kDmaRequestMux0UART1Rx

#define EXAMPLE_TIMEOUT_PERIOD_MS                         (10U)
#define EXAMPLE_TIMEOUT_PERIOD_COUNT                      (EXAMPLE_TIMEOUT_PERIOD_MS * 1000U)
#define EXAMPLE_TIMER_CLK_FREQ                             CLOCK_GetFreq(kCLOCK_LpoClk)
#define EXAMPLE_TIMER_INSTANCE                             (0U)
#define EXAMPLE_RING_BUFFER_SIZE                            32

uart_transfer_t xfer;
uart_transfer_t sendXfer;


typedef struct _hal_timer_handle_struct_t
{
    uint32_t timeout;
    uint32_t timerClock_Hz;
    hal_timer_callback_t callback;
    void *callbackParam;
    uint8_t instance;
} hal_timer_handle_struct_t;


uint8_t                 g_tipString[] = "UART EDMA example\r\nSend back received data\r\nEcho every 8 characters\r\n";
uint8_t                 g_rxBuffer[EXAMPLE_RING_BUFFER_SIZE] = {0};
volatile bool           txOnGoing                      = false;
volatile bool           rxIdleLineDetected             = false;
volatile bool           timeoutFlag                    = false;
volatile                uint32_t ringBufferIndex            = 0U;



void UART_UserCallback(UART_Type *base, uart_edma_handle_t *handle, status_t status, void *userData);

void TIMER_UserCallback(void *param);

void UART_EDMA_RB_Init(void);


void UART_EDMA_RW_Function(void);



#endif 
