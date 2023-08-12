// Copyright 2023 Artem Shubovych (@shybovycha)
// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

#define SPLIT_USB_DETECT
#define SPLIT_LAYER_STATE_ENABLE
// #define SPLIT_HAND_PIN B10

/* connection */
// #define SERIAL_USART_FULL_DUPLEX   // Enable full duplex operation mode.
#undef SERIAL_USART_FULL_DUPLEX

// #define SERIAL_USART_RX_PIN B7     // USART RX pin
// #define SERIAL_USART_TX_PIN A9     // USART TX pin - otherwise read from info.json, "soft_serial_pin"

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
