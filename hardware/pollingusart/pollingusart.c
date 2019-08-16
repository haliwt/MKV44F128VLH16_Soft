#include "pollingusart.h"


uart_edma_handle_t g_uartEdmaHandle;
edma_handle_t g_uartTxEdmaHandle;
edma_handle_t g_uartRxEdmaHandle;
uint8_t g_tipString[]    = "UART EDMA example\r\nSend back received data\r\nEcho every 8 characters\r\n";
uint8_t g_txBuffer[ECHO_BUFFER_LENGTH] = {0};
uint8_t g_rxBuffer[ECHO_BUFFER_LENGTH] = {0};
volatile bool rxBufferEmpty            = true;
volatile bool txBufferFull             = false;
volatile bool txOnGoing                = false;
volatile bool rxOnGoing                = false;

  uart_config_t uartConfig;
 edma_config_t config;
uart_transfer_t xfer;
 uart_transfer_t sendXfer;
 uart_transfer_t receiveXfer;


/*******************************************************************************
 *
 *Code: UART user callback
 *Active : 
 *
 *
 *
*******************************************************************************/
void UART_UserCallback(UART_Type *base, uart_edma_handle_t *handle, status_t status, void *userData)
{
    userData = userData;

    if (kStatus_UART_TxIdle == status)
    {
        txBufferFull = false;
        txOnGoing    = false;
    }

    if (kStatus_UART_RxIdle == status)
    {
        rxBufferEmpty = false;
        rxOnGoing     = false;
    }
}

/*!
 * @brief Main function
 */
void USART_EDMA_Init(void)
{


 
   // BOARD_InitPins();
   // BOARD_BootClockRUN();

    /* Initialize the UART. */
    /*
     * uartConfig.baudRate_Bps = 115200U;
     * uartConfig.parityMode = kUART_ParityDisabled;
     * uartConfig.stopBitCount = kUART_OneStopBit;
     * uartConfig.txFifoWatermark = 0;
     * uartConfig.rxFifoWatermark = 1;
     * uartConfig.enableTx = false;
     * uartConfig.enableRx = false;
     */
    UART_GetDefaultConfig(&uartConfig);
    uartConfig.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
    uartConfig.enableTx     = true;
    uartConfig.enableRx     = true;

    UART_Init(DEMO_UART, &uartConfig, DEMO_UART_CLK_FREQ);

    /* Init DMAMUX */
    DMAMUX_Init(EXAMPLE_UART_DMAMUX_BASEADDR);
    /* Set channel for UART */
    DMAMUX_SetSource(EXAMPLE_UART_DMAMUX_BASEADDR, UART_TX_DMA_CHANNEL, UART_TX_DMA_REQUEST);
    DMAMUX_SetSource(EXAMPLE_UART_DMAMUX_BASEADDR, UART_RX_DMA_CHANNEL, UART_RX_DMA_REQUEST);
    DMAMUX_EnableChannel(EXAMPLE_UART_DMAMUX_BASEADDR, UART_TX_DMA_CHANNEL);
    DMAMUX_EnableChannel(EXAMPLE_UART_DMAMUX_BASEADDR, UART_RX_DMA_CHANNEL);

    /* Init the EDMA module */
    EDMA_GetDefaultConfig(&config);
    EDMA_Init(EXAMPLE_UART_DMA_BASEADDR, &config);
    EDMA_CreateHandle(&g_uartTxEdmaHandle, EXAMPLE_UART_DMA_BASEADDR, UART_TX_DMA_CHANNEL);
    EDMA_CreateHandle(&g_uartRxEdmaHandle, EXAMPLE_UART_DMA_BASEADDR, UART_RX_DMA_CHANNEL);

    /* Create UART DMA handle. */
    UART_TransferCreateHandleEDMA(DEMO_UART, &g_uartEdmaHandle, UART_UserCallback, NULL, &g_uartTxEdmaHandle,
                                  &g_uartRxEdmaHandle);
    
    
   

 }
/*******************************************************************************
 * 
 *Function Name : void USART_RW_Function(void)
 *
 *
 *
 *
******************************************************************************/
#if 0
void USART_EDMA_RW_Function(void)
{

      /* If RX is idle and g_rxBuffer is empty, start to read data to g_rxBuffer. */
        if ((!rxOnGoing) && rxBufferEmpty)
        {
            rxOnGoing = true;
            UART_ReceiveEDMA(DEMO_UART, &g_uartEdmaHandle, &receiveXfer);
        }

        /* If TX is idle and g_txBuffer is full, start to send data. */
        if ((!txOnGoing) && txBufferFull)
        {
            txOnGoing = true;
            UART_SendEDMA(DEMO_UART, &g_uartEdmaHandle, &sendXfer);
        }

        /* If g_txBuffer is empty and g_rxBuffer is full, copy g_rxBuffer to g_txBuffer. */
        if ((!rxBufferEmpty) && (!txBufferFull))
        {
            memcpy(g_txBuffer, g_rxBuffer, ECHO_BUFFER_LENGTH);
            rxBufferEmpty = true;
            txBufferFull  = true;
        }
    
 

}

#endif 

