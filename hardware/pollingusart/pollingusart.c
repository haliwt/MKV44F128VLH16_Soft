#include "pollingusart.h"


uint8_t txbuff[]   = "Uart polling example\r\nBoard will send back received characters\r\n";
uint8_t rxbuff[20] = {0};


void USART_POLLING_Init(void)
{


    uart_config_t config;
   
    UART_GetDefaultConfig(&config);
    config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
    config.enableTx     = true;
    config.enableRx     = true;

    UART_Init(DEMO_UART, &config, DEMO_UART_CLK_FREQ);

  //  UART_WriteBlocking(DEMO_UART, txbuff, sizeof(txbuff) - 1);
    
   

 }

