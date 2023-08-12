#pragma once

#include_next <mcuconf.h>

// #include "config.h"

// #undef STM32_SIO_USE_USART0

// #undef STM32_SIO_USE_USART1

#undef STM32_SERIAL_USE_USART1
#define STM32_SERIAL_USE_USART1 TRUE

/*
#ifdef LEFT
    // LEFT: UART0 on 21/22 GP16/GP17
    #define SERIAL_USART_TX_PIN		GP16
    #define SERIAL_USART_RX_PIN		GP17
    #define PLATFORM_SIO_USE_SIO0     TRUE
    #define RP_SIO_USE_UART0          TRUE
    #define SERIAL_USART_DRIVER       SIOD0
#else
    // RIGHT: UART1 on 11/12 GP8/GP9
    #define SERIAL_USART_TX_PIN		GP8
    #define SERIAL_USART_RX_PIN		GP9
    #define PLATFORM_SIO_USE_SIO1     TRUE
    #define RP_SIO_USE_UART1          TRUE
    #define SERIAL_USART_DRIVER       SIOD1
#endif
*/
