#include "pollingusart.h"
uint8_t   txbuff[]   = "Uart polling example\r\nBoard will send back received characters\r\n";
uint8_t   rxbuff[20] = {0};


/*************************************************
 *
 *Function Name :
 *Function : 
 *
 *
 *
**************************************************/
void POLLING_USART_Init(void)
{
  
   // uint8_t ch;
    uart_config_t config;

    /*
     * config.baudRate_Bps = 115200U;
     * config.parityMode = kUART_ParityDisabled;
     * config.stopBitCount = kUART_OneStopBit;
     * config.txFifoWatermark = 0;
     * config.rxFifoWatermark = 1;
     * config.enableTx = false;
     * config.enableRx = false;
     */
    UART_GetDefaultConfig(&config);
    config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
    config.enableTx     = true;
    config.enableRx     = true;

    UART_Init(DEMO_UART, &config, DEMO_UART_CLK_FREQ);

    UART_WriteBlocking(DEMO_UART, txbuff, sizeof(txbuff) - 1);

	//UART_ReadBlocking(DEMO_UART, &ch, 1);
   // UART_WriteBlocking(DEMO_UART, &ch, 1);

}



