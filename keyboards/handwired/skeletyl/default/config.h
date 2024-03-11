/*
Copyright 2023 Artem Shubovych
Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

// in col2row col is input, and row is output
// #define SPLIT_HAND_MATRIX_GRID A10, A4
// 68kohm
// High -> left, Low -> right.
#define SPLIT_HAND_PIN              GP29

/* connection */
#undef SERIAL_USART_HALF_DUPLEX   // Enable half-duplex operation mode.

#define SERIAL_USART_TX_PIN GP0     // USART TX pin

#define SELECT_SOFT_SERIAL_SPEED 1 // or 0, 2, 3, 4, 5
                                   //  0: 460800 baud
                                   //  1: 230400 baud (default)
                                   //  2: 115200 baud
                                   //  3: 57600 baud
                                   //  4: 38400 baud
                                   //  5: 19200 baud
#define SERIAL_USART_DRIVER PIO    // USART driver of TX and RX pin. default: SD1

#define PERMISSIVE_HOLD            // enable tap-and-hold keys
#define TAPPING_TOGGLE 2           // number of taps needed to toggle (layers, with TT(x))
