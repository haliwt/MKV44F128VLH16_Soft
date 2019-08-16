#include "usart_edma_rb.h"



/*******************************************************************************
 *
 * Prototypes:Initialize the UART module. 
 *
 *
 ******************************************************************************/
static void EXAMPLE_InitUART(void);

/* Initialize the EDMA configuration. */
static void EXAMPLE_InitEDMA(void);

/* Initialize a hardware timer. */
static void EXAMPLE_InitTimer(void);

/* Start ring buffer configuration. */
static void EXAMPLE_StartRingBufferEDMA(void);

/* Get how many bytes in ring buffer. */
static uint32_t EXAMPLE_GetRingBufferLengthEDMA(void);

/* Read the characters from ring buffer. */
static void EXAMPLE_ReadRingBufferEDMA(uint8_t *ringBuffer, uint8_t *receiveBuffer, uint32_t length);



/*******************************************************************************
 * Variables
 ******************************************************************************/

uart_edma_handle_t      g_uartEdmaHandle;
edma_handle_t           g_uartTxEdmaHandle;
edma_handle_t           g_uartRxEdmaHandle;


AT_NONCACHEABLE_SECTION_ALIGN(uint8_t g_ringBuffer[EXAMPLE_RING_BUFFER_SIZE], 16);
/* Allocate TCD memory poll with ring buffer used. */
AT_NONCACHEABLE_SECTION_ALIGN(edma_tcd_t tcdMemoryPoolPtr[1], sizeof(edma_tcd_t));

hal_timer_handle_struct_t g_timerHandle;

/*******************************************************************************
 * Code
 ******************************************************************************/
/* Initialize the UART module. */
static void EXAMPLE_InitUART(void)
{
    uart_config_t uartConfig;

    /*
     * uartConfig.baudRate_Bps = 115200U;
     * uartConfig.parityMode = kUART_ParityDisabled;
     * uartConfig.stopBitCount = kUART_OneStopBit;
     * uartConfig.txFifoWatermark = 0;
     * uartConfig.rxFifoWatermark = 1;
     * uartConfig.enableTx = false;
     * uartConfig.enableRx = false;
     */
    UART_GetDefaultConfig(&uartConfig); //freescale default define UART reference
    uartConfig.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;//重新定义波特率
    uartConfig.enableTx     = true;
    uartConfig.enableRx     = true;

    UART_Init(EXAMPLE_UART, &uartConfig, EXAMPLE_UART_CLK_FREQ); 
}

/* Initialize the EDMA configuration. */
static void EXAMPLE_InitEDMA(void)
{
    edma_config_t config;

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
    UART_TransferCreateHandleEDMA(EXAMPLE_UART, &g_uartEdmaHandle, UART_UserCallback, NULL, &g_uartTxEdmaHandle, NULL);
}

/* Initialize a hardware timer. */
static void EXAMPLE_InitTimer(void)
{
    hal_timer_config_t timerConfig;

    timeoutFlag             = false;
    timerConfig.timeout     = EXAMPLE_TIMEOUT_PERIOD_COUNT;
    timerConfig.srcClock_Hz = EXAMPLE_TIMER_CLK_FREQ;
    timerConfig.instance    = EXAMPLE_TIMER_INSTANCE;

    (void)memset(&g_timerHandle, 0, sizeof(g_timerHandle));

    /* Initialize the timer. */
    HAL_TimerInit(&g_timerHandle, &timerConfig);
    /* Install call back function. */
    HAL_TimerInstallCallback(&g_timerHandle, TIMER_UserCallback, NULL);
}

/* Start ring buffer configuration. */
static void EXAMPLE_StartRingBufferEDMA(void)
{
    edma_transfer_config_t xferConfig;

    /* Install TCD memory. */
    EDMA_InstallTCDMemory(&g_uartRxEdmaHandle, tcdMemoryPoolPtr, 1);

    /* Prepare transfer to receive data to ring buffer. */
    EDMA_PrepareTransfer(&xferConfig, (void *)UART_GetDataRegisterAddress(EXAMPLE_UART), sizeof(uint8_t), g_ringBuffer,
                         sizeof(uint8_t), sizeof(uint8_t), EXAMPLE_RING_BUFFER_SIZE, kEDMA_PeripheralToMemory);

    /* Submit transfer. */
    g_uartRxEdmaHandle.tcdUsed = 1U;
    g_uartRxEdmaHandle.tail    = 0U;
    EDMA_TcdReset(&g_uartRxEdmaHandle.tcdPool[0U]);
    EDMA_TcdSetTransferConfig(&g_uartRxEdmaHandle.tcdPool[0U], &xferConfig, tcdMemoryPoolPtr);

    /* Enable major interrupt for calculating the received bytes. */
    g_uartRxEdmaHandle.tcdPool[0U].CSR |= DMA_CSR_INTMAJOR_MASK;

    /* There is no live chain, TCD block need to be installed in TCD registers. */
    EDMA_InstallTCD(EXAMPLE_UART_DMA_BASEADDR, UART_RX_DMA_CHANNEL, &g_uartRxEdmaHandle.tcdPool[0U]);

    /* Start EDMA transfer. */
    EDMA_StartTransfer(&g_uartRxEdmaHandle);

    /* Enable UART RX EDMA. */
    UART_EnableRxDMA(EXAMPLE_UART, true);
}
/**************************************************************************
 *
 *Function Name:
 *Get how many bytes in ring buffer. 
 *
 *
***************************************************************************/
static uint32_t EXAMPLE_GetRingBufferLengthEDMA(void)
{
    uint32_t regPrimask    = 0U;
    uint32_t receivedBytes = 0U;

    /* Disable IRQ, protect ring buffer. */
    regPrimask = DisableGlobalIRQ();

    receivedBytes =
        EXAMPLE_RING_BUFFER_SIZE - EDMA_GetRemainingMajorLoopCount(EXAMPLE_UART_DMA_BASEADDR, UART_RX_DMA_CHANNEL);

    /* If the received bytes is less than index value, it means the ring buffer has reached it boundary. */
    if (receivedBytes < ringBufferIndex)
    {
        receivedBytes += EXAMPLE_RING_BUFFER_SIZE;
    }

    receivedBytes -= ringBufferIndex;

    /* Enable IRQ if previously enabled. */
    EnableGlobalIRQ(regPrimask);

    return receivedBytes;
}
/**************************************************************************
 *
 *Function Name:
 *Active: Read the characters from ring buffer.
 *
 *
***************************************************************************/
static void EXAMPLE_ReadRingBufferEDMA(uint8_t *ringBuffer, uint8_t *receiveBuffer, uint32_t length)
{
    assert(ringBuffer);
    assert(receiveBuffer);
    assert(length);

    uint32_t index = length;

    /* If length if larger than ring buffer size, it means overflow occurred, need to reset the ringBufferIndex. */
    if (length > EXAMPLE_RING_BUFFER_SIZE)
    {
        ringBufferIndex = ((ringBufferIndex + length) % EXAMPLE_RING_BUFFER_SIZE);
        index           = EXAMPLE_RING_BUFFER_SIZE;
    }

    while (index)
    {
        *(receiveBuffer++) = ringBuffer[ringBufferIndex++];
        if (ringBufferIndex == EXAMPLE_RING_BUFFER_SIZE)
        {
            ringBufferIndex = 0U;
        }
        index--;
    }
}
/**************************************************
 *
 *UART user callback 
 *
 *
***************************************************/
void UART_UserCallback(UART_Type *base, uart_edma_handle_t *handle, status_t status, void *userData)
{
    userData = userData;

    if (kStatus_UART_TxIdle == status)
    {
        txOnGoing = false;
    }
}
/*****************************************************
 *
 * Function Name:Timer call back. 
 *
 *
 *
******************************************************/
void TIMER_UserCallback(void *param)
{
    timeoutFlag = true;
}
/**********************************************
 *
 *Function Name: USART_EDMA_Init()
 *
 *
 *
***********************************************/
void UART_EDMA_RB_Init(void)
{
   

    /* Initialzie the hardware. */
 //   BOARD_InitPins();
 //   BOARD_BootClockRUN();

    /* Initialize the UART configurations. */
    EXAMPLE_InitUART();

    /* Initialize the EDMA configuration for UART trasnfer. */
    EXAMPLE_InitEDMA();

    /* Initialize a timer for use. */
    EXAMPLE_InitTimer();

    /* Send g_tipString out. */
    xfer.data     = g_tipString;
    xfer.dataSize = sizeof(g_tipString) - 1;
    txOnGoing     = true;
    UART_SendEDMA(EXAMPLE_UART, &g_uartEdmaHandle, &xfer);
}

/************************************************
 *
 * @brief Main function
 *
 *
 *
**************************************************/
void UART_EDMA_RW_Function(void)
{
    uint32_t byteCount = 0U;
   

    /* Wait send finished */
    while (txOnGoing)
    {
    }

    /* Start ring buffer. */
    EXAMPLE_StartRingBufferEDMA();

    /* Start timer. */
    HAL_TimerEnable(&g_timerHandle);

   // while (1)
    {
        

        /* Wait for timer timeout occurred. Timeout period is defined by EXAMPLE_TIMEOUT_PERIOD_MS*/
        while (!timeoutFlag)
        {
        }

        timeoutFlag = false;
        /* Get the received bytes number stored in DMA ring buffer. */
        byteCount = EXAMPLE_GetRingBufferLengthEDMA();

        if (0U != byteCount)
        {
            /* Move the data from ring buffer to given buffer section. */
            EXAMPLE_ReadRingBufferEDMA(g_ringBuffer, g_rxBuffer, byteCount);

            /* Wait for sending finished */
            while (txOnGoing)
            {
            }

            /* Start to echo. */
            txOnGoing         = true;
            sendXfer.data     = g_rxBuffer;
            sendXfer.dataSize = byteCount;
            UART_SendEDMA(EXAMPLE_UART, &g_uartEdmaHandle, &sendXfer);
        }
    }
}
