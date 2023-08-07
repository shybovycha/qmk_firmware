#include "matrix.h"
#include <string.h>
#include "timer.h"
#include "wait.h"
#include "debug.h"
#include <stdint.h>
#include <stdbool.h>
#include <hal.h>
// #include "i2c_master.h"
// #include "ergodox_stm32.h"

#ifndef DEBOUNCE
#define DEBOUNCE 10
#endif

// static uint8_t mcp23017_reset_loop = 0;

volatile matrix_row_t matrix[MATRIX_ROWS];
volatile matrix_row_t raw_matrix[MATRIX_ROWS];
volatile uint8_t debounce_matrix[MATRIX_ROWS * MATRIX_COLS];

static matrix_row_t read_cols(uint8_t row);

static void init_cols(void);

static void unselect_rows(void);

static void select_row(uint8_t row);

static void init_rows(void);

__attribute__((weak)) void matrix_init_user(void) {}

__attribute__((weak)) void matrix_scan_user(void) {}

__attribute__((weak)) void matrix_init_kb(void) {
  matrix_init_user();
}

__attribute__((weak)) void matrix_scan_kb(void) {
  matrix_scan_user();
}

void matrix_init(void) {
  // mcp23017_status = init_mcp23017();
  // (void) mcp23017_reset_loop;
  init_rows();
  unselect_rows();
  init_cols();


  for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
    matrix[i] = 0;
    raw_matrix[i] = 0;
    for (uint8_t j = 0; j < MATRIX_COLS; ++j) {
      debounce_matrix[i * MATRIX_COLS + j] = 0;
    }
  }
  matrix_init_kb();
}

void matrix_power_up(void) {
  // mcp23017_status = init_mcp23017();

  init_rows();
  unselect_rows();
  init_cols();

  for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
    matrix[i] = 0;
  }
}

matrix_row_t debounce_mask(matrix_row_t rawcols, uint8_t row) {
  matrix_row_t result = 0;
  matrix_row_t change = rawcols ^raw_matrix[row];
  raw_matrix[row] = rawcols;
  for (uint8_t i = 0; i < MATRIX_COLS; ++i) {
    if (debounce_matrix[row * MATRIX_COLS + i]) {
      --debounce_matrix[row * MATRIX_COLS + i];
    } else {
      result |= (1 << i);
    }
    if (change & (1 << i)) {
      debounce_matrix[row * MATRIX_COLS + i] = DEBOUNCE;
    }
  }
  return result;
}

matrix_row_t debounce_read_cols(uint8_t row) {
  // Read the row without debouncing filtering and store it for later usage.
  matrix_row_t cols = read_cols(row);
  // Get the Debounce mask.
  matrix_row_t mask = debounce_mask(cols, row);
  // debounce the row and return the result.
  return (cols & mask) | (matrix[row] & ~mask);;
}

uint8_t matrix_scan(void) {
  // if (mcp23017_status) {
  //   if (++mcp23017_reset_loop == 0) {
  //     mcp23017_status = init_mcp23017();
  //       if (!mcp23017_status) {
  //           ergodox_blink_all_leds();
  //       }
  //   }
  // }
  for (uint8_t i = 0; i < MATRIX_ROWS_PER_SIDE; i++) {
    select_row(i);
    select_row(i + MATRIX_ROWS_PER_SIDE);

    matrix[i] = debounce_read_cols(i);
    matrix[i + MATRIX_ROWS_PER_SIDE] = debounce_read_cols(i + MATRIX_ROWS_PER_SIDE);

    unselect_rows();
  }
  matrix_scan_kb();
  return 0;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col) {
  return (matrix[row] & (1 << col));
}

inline
matrix_row_t matrix_get_row(uint8_t row) {
  return matrix[row];
}

void matrix_print(void) {
}

static matrix_row_t read_cols(uint8_t row) {
  if (row < MATRIX_ROWS_PER_SIDE) {
    // uint8_t data = 0xFF;
    // if (!mcp23017_status) {
    //   uint8_t regAddr = I2C_GPIOB;
    //   mcp23017_status = i2c_readReg(I2C_ADDR, regAddr, &data, 1, 10);
    // }
    // if (mcp23017_status) {
    //   return 0;
    // }
    // return (~data) & 0x3F;
    return 0;
  } else {
    // left:
    // "cols": ["B12", "B13", "B14", "B15", "A8", "A15"],
    // right:
    // "cols": ["A15", "A8", "B15", "B14", "B13", "B12"],
    // "rows": ["A7", "A6", "A5", "B3", "B4", "B5"]
    // uint8_t data_p = (GPIOB->IDR);
    // uint8_t data = data_p;
    // return ((~data) & 0x3f);

    // uint16_t dataA = GPIOA->IDR;
    // uint16_t dataB = GPIOB->IDR;

    // uint16_t bitsA = ~dataA;
    // uint16_t bitsB = ~dataB;

    // return
    //   ((bitsB >> 12) << 5) |
    //   ((bitsB >> 13) << 4) |
    //   ((bitsB >> 14) << 3) |
    //   ((bitsB >> 15) << 2) |
    //   ((bitsA >> 8) << 1) |
    //   ((bitsA >> 15) << 0)
    // ;

    return
      ((palReadPad(GPIOB, 12) ? 0 : 1) << 5) |
      ((palReadPad(GPIOB, 13) ? 0 : 1) << 4) |
      ((palReadPad(GPIOB, 14) ? 0 : 1) << 3) |
      ((palReadPad(GPIOB, 15) ? 0 : 1) << 2) |
      ((palReadPad(GPIOA, 8) ? 0 : 1) << 1) |
      ((palReadPad(GPIOA, 15) ? 0 : 1) << 0)
    ;
  }
}

static void init_cols(void) {
  // palSetPadMode(GPIOB, 0, PAL_MODE_INPUT_PULLUP);
  // palSetPadMode(GPIOB, 1, PAL_MODE_INPUT_PULLUP);
  // palSetPadMode(GPIOB, 2, PAL_MODE_INPUT_PULLUP);
  // palSetPadMode(GPIOB, 3, PAL_MODE_INPUT_PULLUP);
  // palSetPadMode(GPIOB, 4, PAL_MODE_INPUT_PULLUP);
  // palSetPadMode(GPIOB, 5, PAL_MODE_INPUT_PULLUP);

  // "cols": ["B12", "B13", "B14", "B15", "A8", "A15"],
  palSetPadMode(GPIOB, 12, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOB, 13, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOB, 14, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOB, 15, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOA, 8, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOA, 15, PAL_MODE_INPUT_PULLUP);
}

static void init_rows(void) {
  // palSetPadMode(GPIOA, 8, PAL_MODE_OUTPUT_PUSHPULL);
  // palSetPadMode(GPIOA, 9, PAL_MODE_OUTPUT_PUSHPULL);
  // palSetPadMode(GPIOA, 10, PAL_MODE_OUTPUT_PUSHPULL);
  // palSetPadMode(GPIOA, 11, PAL_MODE_OUTPUT_PUSHPULL);
  // palSetPadMode(GPIOA, 12, PAL_MODE_OUTPUT_PUSHPULL);
  // palSetPadMode(GPIOA, 13, PAL_MODE_OUTPUT_PUSHPULL);
  // palSetPadMode(GPIOB, 14, PAL_MODE_OUTPUT_PUSHPULL);

  // "rows": ["A7", "A6", "A5", "B3", "B4", "B5"]
  palSetPadMode(GPIOA, 7, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOA, 6, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOA, 5, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOB, 3, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOB, 4, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOB, 5, PAL_MODE_OUTPUT_PUSHPULL);
}

static void unselect_rows(void) {
  // "rows": ["A7", "A6", "A5", "B3", "B4", "B5"]
  // GPIOB->BSRR = 0b1111111 << 8;

  // set row pins to high
  // shift by 16 (<< 16) to set to low
  pal_lld_setport(GPIOA, (1 << 7) | (1 << 6) | (1 << 5));
  pal_lld_setport(GPIOB, (1 << 3) | (1 << 4) | (1 << 5));
  // GPIOA->BSRR = (uint32_t) ((1 << 7) | (1 << 6) | (1 << 5) << (1 << 1) | (1 << 3) | (1 << 2));
}

static void select_row(uint8_t row) {
  if (row < MATRIX_ROWS_PER_SIDE) {
    // if (!mcp23017_status) {
    //   uint8_t data = (0xFF & ~(1 << row));
    //   mcp23017_status = i2c_writeReg(I2C_ADDR, I2C_GPIOA, &data, 1, 10);
    // }
  } else {
    // "cols": ["B12", "B13", "B14", "B15", "A8", "A15"],
    // "rows": ["A7", "A6", "A5", "B3", "B4", "B5"]
    // GPIOB->BRR = 0x1 << (row+1);
    if (row == MATRIX_ROWS_PER_SIDE + 0) palClearPad(GPIOA, 7); // pal_lld_clearport(GPIOA, 1 << 7); // GPIOA->BSRR = 1 << (7 + 16);
    if (row == MATRIX_ROWS_PER_SIDE + 1) palClearPad(GPIOA, 6); // pal_lld_clearport(GPIOA, 1 << 6); // GPIOA->BSRR = 1 << (6 + 16);
    if (row == MATRIX_ROWS_PER_SIDE + 2) palClearPad(GPIOA, 5); // pal_lld_clearport(GPIOA, 1 << 5); // GPIOA->BSRR = 1 << (5 + 16);
    if (row == MATRIX_ROWS_PER_SIDE + 3) palClearPad(GPIOB, 3); // pal_lld_clearport(GPIOA, 1 << 1); // GPIOA->BSRR = 1 << (1 + 16);
    if (row == MATRIX_ROWS_PER_SIDE + 4) palClearPad(GPIOB, 4); // pal_lld_clearport(GPIOA, 1 << 3); // GPIOA->BSRR = 1 << (3 + 16);
    if (row == MATRIX_ROWS_PER_SIDE + 5) palClearPad(GPIOB, 5); // pal_lld_clearport(GPIOA, 1 << 2); // GPIOA->BSRR = 1 << (2 + 16);
  }
}


// #include "matrix.h"
// #include "hal.h"
// #include "debounce.h"
// #include "wait.h"
// #include "timer.h"

// #ifndef DEBOUNCE
// #define DEBOUNCE 10
// #endif

// matrix_row_t matrix[MATRIX_ROWS];
// matrix_row_t raw_matrix[MATRIX_ROWS];
// uint8_t debounce_matrix[MATRIX_ROWS * MATRIX_COLS];

// static matrix_row_t read_cols(uint8_t row);

// static void select_row(uint8_t row);

// matrix_row_t debounce_read_cols(uint8_t row);

// void unselect_rows(void);

// __attribute__((weak)) void matrix_init_user(void) {}

// __attribute__((weak)) void matrix_scan_user(void) {}

// __attribute__((weak)) void matrix_init_kb(void) {
//   matrix_init_user();
// }

// __attribute__((weak)) void matrix_scan_kb(void) {
//   matrix_scan_user();
// }

// void matrix_print(void) {}

// /*
// # Initialization

// All col and row IO pins are configured as Input with pullups activated.
// In my schematic case those are Row0, Row1, Col0 and Col1.

// # Matrix scan

// For each IO row pin(Row0, then Row1):

// 1. Configure row IO pin as output.
// 2. Set row IO pin in a low state (0) so that it sinks current.
// 3. Wait a little bit (QMK default is 30 microseconds).
// 4. For each IO col pin (Col0, then Col1):
//   a. Read pin.
//   b. If it is high, that means that the switch is opened and that the current flows from the pullup to the col pin.
//   c. If it is low, that means that the switch is closed and that the current is sinked by the row pin, no current flows to the col pin.
// 5. Configure back row IO pin as input with pullup.
// */

// // "cols": ["B12", "B13", "B14", "B15", "A8", "A15"],
// // "rows": ["A7", "A6", "A5", "A1", "A3", "A2"]

// static void init_cols(void) {
//   // "cols": ["B12", "B13", "B14", "B15", "A8", "A15"],
//   palSetPadMode(GPIOB, 12, PAL_MODE_INPUT_PULLUP);
//   palSetPadMode(GPIOB, 13, PAL_MODE_INPUT_PULLUP);
//   palSetPadMode(GPIOB, 14, PAL_MODE_INPUT_PULLUP);
//   palSetPadMode(GPIOB, 15, PAL_MODE_INPUT_PULLUP);
//   palSetPadMode(GPIOA, 8, PAL_MODE_INPUT_PULLUP);
//   palSetPadMode(GPIOA, 15, PAL_MODE_INPUT_PULLUP);
// }

// static void init_rows(void) {
//   // "rows": ["A7", "A6", "A5", "A1", "A3", "A2"]
//   // palSetPadMode(GPIOA, 7, PAL_MODE_OUTPUT_PUSHPULL);
//   // palSetPadMode(GPIOA, 6, PAL_MODE_OUTPUT_PUSHPULL);
//   // palSetPadMode(GPIOA, 5, PAL_MODE_OUTPUT_PUSHPULL);
//   // palSetPadMode(GPIOA, 1, PAL_MODE_OUTPUT_PUSHPULL);
//   // palSetPadMode(GPIOA, 3, PAL_MODE_OUTPUT_PUSHPULL);
//   // palSetPadMode(GPIOA, 2, PAL_MODE_OUTPUT_PUSHPULL);

//   palSetPadMode(GPIOA, 7, PAL_MODE_INPUT_PULLUP);
//   palSetPadMode(GPIOA, 6, PAL_MODE_INPUT_PULLUP);
//   palSetPadMode(GPIOA, 5, PAL_MODE_INPUT_PULLUP);
//   palSetPadMode(GPIOA, 1, PAL_MODE_INPUT_PULLUP);
//   palSetPadMode(GPIOA, 3, PAL_MODE_INPUT_PULLUP);
//   palSetPadMode(GPIOA, 2, PAL_MODE_INPUT_PULLUP);
// }

// void matrix_init(void) {
//   // TODO: initialize hardware and global matrix state here
//   init_rows();
//   unselect_rows();
//   init_cols();

//   for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
//     raw_matrix[row] = 0x00;
//     matrix[row] = 0x00;

//     for (uint8_t col = 0; col < MATRIX_COLS; col++) {
//       debounce_matrix[row * MATRIX_COLS + col] = 0;
//     }
//   }

//   // Unless hardware debouncing - Init the configured debounce routine
//   debounce_init(MATRIX_ROWS);

//   // This *must* be called for correct keyboard behavior
//   matrix_init_kb();
// }

// void matrix_power_up(void) {
//   init_rows();
//   unselect_rows();
//   init_cols();

//   for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
//     matrix[row] = 0;
//   }
// }

// void unselect_rows(void) {
//   // "rows": ["A7", "A6", "A5", "A1", "A3", "A2"]
//   // GPIOA->BSRR =
//   // ((1 << 7) |
//   // (1 << 6) |
//   // (1 << 5) |
//   // (1 << 4) |
//   // (1 << 3) |
//   // (1 << 2)) << 16;

//   // "rows": ["A7", "A6", "A5", "A1", "A3", "A2"]
//   pal_lld_clearport(GPIOA, 7);
//   pal_lld_clearport(GPIOA, 6);
//   pal_lld_clearport(GPIOA, 5);
//   pal_lld_clearport(GPIOA, 1);
//   pal_lld_clearport(GPIOA, 3);
//   pal_lld_clearport(GPIOA, 2);

//   palSetPadMode(GPIOA, 7, PAL_MODE_INPUT_PULLUP);
//   palSetPadMode(GPIOA, 6, PAL_MODE_INPUT_PULLUP);
//   palSetPadMode(GPIOA, 5, PAL_MODE_INPUT_PULLUP);
//   palSetPadMode(GPIOA, 1, PAL_MODE_INPUT_PULLUP);
//   palSetPadMode(GPIOA, 3, PAL_MODE_INPUT_PULLUP);
//   palSetPadMode(GPIOA, 2, PAL_MODE_INPUT_PULLUP);
// }

// uint8_t matrix_scan(void) {
//   // bool changed = false;

//   for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
//     select_row(row);

//     wait_us(50);

//     // matrix[row] = debounce_read_cols(row);
//     // matrix[row + MATRIX_ROWS_PER_SIDE] = debounce_read_cols(row + MATRIX_ROWS_PER_SIDE);

//     matrix[row] = read_cols(row);
//     // matrix[row + MATRIX_ROWS_PER_SIDE] = read_cols(row + MATRIX_ROWS_PER_SIDE);

//     unselect_rows();
//   }

//   // Unless hardware debouncing - use the configured debounce routine
//   // changed = debounce(raw_matrix, matrix, MATRIX_ROWS, changed);

//   // This *must* be called for correct keyboard behavior
//   matrix_scan_kb();

//   return 1;
// }

// matrix_row_t debounce_mask(matrix_row_t rawcols, uint8_t row) {
//   matrix_row_t result = 0;
//   matrix_row_t change = rawcols ^ raw_matrix[row];

//   raw_matrix[row] = rawcols;

//   for (uint8_t col = 0; col < MATRIX_COLS; col++) {
//     if (debounce_matrix[row * MATRIX_COLS + col]) {
//       --debounce_matrix[row * MATRIX_COLS + col];
//     } else {
//       result |= (1 << col);
//     }

//     if (change & (1 << col)) {
//       debounce_matrix[row * MATRIX_COLS + col] = DEBOUNCE;
//     }
//   }

//   return result;
// }

// matrix_row_t read_cols(uint8_t row) {
//   // "cols": ["B12", "B13", "B14", "B15", "A8", "A15"],
//   if (row < MATRIX_ROWS_PER_SIDE) {
//     return 0;
//   }

//   uint16_t portA = GPIOA->IDR;
//   uint16_t portB = GPIOB->IDR;
//   uint16_t bitsA = (~portA);
//   uint16_t bitsB = (~portB);

//   matrix_row_t result = 0x00;

//   if (bitsB & (1 << 12)) result |= (1 << 5);
//   if (bitsB & (1 << 13)) result |= (1 << 4);
//   if (bitsB & (1 << 14)) result |= (1 << 3);
//   if (bitsB & (1 << 15)) result |= (1 << 2);
//   if (bitsA & (1 << 8)) result |= (1 << 1);
//   if (bitsA & (1 << 15)) result |= (1 << 0);

//   return result;
// }

// matrix_row_t matrix_get_row(uint8_t row) {
//   return matrix[row];
// }

// matrix_row_t debounce_read_cols(uint8_t row) {
//   // Read the row without debouncing filtering and store it for later usage.
//   matrix_row_t cols = read_cols(row);

//   // Get the Debounce mask.
//   matrix_row_t mask = debounce_mask(cols, row);

//   // debounce the row and return the result.
//   return (cols & mask) | (matrix[row] & ~mask);
// }

// static void select_row(uint8_t row) {
//   // "rows": ["A7", "A6", "A5", "A1", "A3", "A2"]
//   if (row < MATRIX_ROWS_PER_SIDE) {
//     // send signal to the other half
//   } else {
//     if (row == MATRIX_ROWS_PER_SIDE + 0) {
//       palSetPadMode(GPIOA, 7, PAL_MODE_OUTPUT_PUSHPULL);
//       pal_lld_setport(GPIOA, 7); // GPIOA->BSRR = (uint32_t)(0x01 << 7);
//     }
//     else if (row == MATRIX_ROWS_PER_SIDE + 1) {
//       palSetPadMode(GPIOA, 6, PAL_MODE_OUTPUT_PUSHPULL);
//       pal_lld_setport(GPIOA, 6); // GPIOA->BSRR = (uint32_t)(0x01 << 6);
//     }
//     else if (row == MATRIX_ROWS_PER_SIDE + 2) {
//       palSetPadMode(GPIOA, 5, PAL_MODE_OUTPUT_PUSHPULL);
//       pal_lld_setport(GPIOA, 5); // GPIOA->BSRR = (uint32_t)(0x01 << 5);
//     }
//     else if (row == MATRIX_ROWS_PER_SIDE + 3) {
//       palSetPadMode(GPIOA, 1, PAL_MODE_OUTPUT_PUSHPULL);
//       pal_lld_setport(GPIOA, 1); // GPIOA->BSRR = (uint32_t)(0x01 << 1);
//     }
//     else if (row == MATRIX_ROWS_PER_SIDE + 4) {
//       palSetPadMode(GPIOA, 3, PAL_MODE_OUTPUT_PUSHPULL);
//       pal_lld_setport(GPIOA, 3); // GPIOA->BSRR = (uint32_t)(0x01 << 3);
//     }
//     else if (row == MATRIX_ROWS_PER_SIDE + 5) {
//       palSetPadMode(GPIOA, 2, PAL_MODE_OUTPUT_PUSHPULL);
//       pal_lld_setport(GPIOA, 2); // GPIOA->BSRR = (uint32_t)(0x01 << 2);
//     }
//   }
// }
