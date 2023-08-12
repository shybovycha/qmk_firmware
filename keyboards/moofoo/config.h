// Copyright 2023 Artem Shubovych (@shybovycha)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// left:
// "rows": ["A7", "A6", "A5", "B3", "B4", "B5"],
// "cols": ["B12", "B14", "B15", "A0", "A1", "A15"]

// right:
// "rows": ["A7", "A6", "A5", "B3", "B4", "B5"],
// "cols": ["B12", "B13", "B14", "B15", "A8", "A15"],
// #define MATRIX_ROW_PINS       { A7,  A6,  A5,  B3, B4, B5  }
// #define MATRIX_COL_PINS       { B12, B14, B15, A0, A1, A15 }

// #define MATRIX_ROW_PINS_RIGHT { A7,  A6,  A5,  B3,  B4, B5  }
// #define MATRIX_COL_PINS_RIGHT { B12, B13, B14, B15, A8, A15 }

#define SPLIT_USB_DETECT
#define SPLIT_LAYER_STATE_ENABLE
#define SPLIT_HAND_PIN B10

#define SERIAL_USART_FULL_DUPLEX

#define HAL_USE_SIO TRUE

// ===

/* connection */
// #define SERIAL_USART_FULL_DUPLEX   // Enable full duplex operation mode.

#define SERIAL_USART_RX_PIN B7     // USART RX pin
#define SERIAL_USART_TX_PIN B6     // USART TX pin

#define SELECT_SOFT_SERIAL_SPEED 1 // or 0, 2, 3, 4, 5
                                   //  0: 460800 baud
                                   //  1: 230400 baud (default)
                                   //  2: 115200 baud
                                   //  3: 57600 baud
                                   //  4: 38400 baud
                                   //  5: 19200 baud
#define SERIAL_USART_DRIVER SD1    // USART driver of TX and RX pin. default: SD1
#define SERIAL_USART_TX_PAL_MODE 7 // Pin "alternate function", see the respective datasheet for the appropriate values for your MCU. default: 7
#define SERIAL_USART_RX_PAL_MODE 7 // Pin "alternate function", see the respective datasheet for the appropriate values for your MCU. default: 7
#define SERIAL_USART_TIMEOUT 20    // USART driver timeout. default 20
