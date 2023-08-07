#include "matrix.h"
#include "hal.h"
#include "debounce.h"
#include "wait.h"
#include "timer.h"

// #include <string.h>
// #include "debug.h"
// #include "i2c_master.h"

matrix_row_t matrix[MATRIX_ROWS];
matrix_row_t raw_matrix[MATRIX_ROWS];
uint8_t debounce_matrix[MATRIX_ROWS * MATRIX_COLS];

static matrix_row_t read_cols(uint8_t row);

// static void init_cols(void);

// static void unselect_rows(void);

static void select_row(uint8_t row);

// static void init_rows(void);

__attribute__((weak)) void matrix_init_user(void) {}

__attribute__((weak)) void matrix_scan_user(void) {}

__attribute__((weak)) void matrix_init_kb(void) {
  matrix_init_user();
}

__attribute__((weak)) void matrix_scan_kb(void) {
  matrix_scan_user();
}

/* matrix state(1:on, 0:off) */
// static matrix_row_t matrix[MATRIX_ROWS];     // debounced values
// static matrix_row_t raw_matrix[MATRIX_ROWS]; // raw values

// static const pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
// static const pin_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;

// static matrix_row_t matrix[MATRIX_ROWS];

void matrix_init(void) {
  // TODO: initialize hardware and global matrix state here
  for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
    raw_matrix[row] = 0x00;
    matrix[row] = 0x00;
  }

  // Unless hardware debouncing - Init the configured debounce routine
  debounce_init(MATRIX_ROWS);

  // This *must* be called for correct keyboard behavior
  matrix_init_kb();
}

uint8_t matrix_scan(void) {
  bool changed = false;

  for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
    select_row(row);

    wait_ms(2);

    matrix[row] = read_cols(row);
  }

  // Unless hardware debouncing - use the configured debounce routine
  changed = debounce(raw_matrix, matrix, MATRIX_ROWS, changed);

  // This *must* be called for correct keyboard behavior
  matrix_scan_kb();

  return changed;
}

matrix_row_t read_cols(uint8_t row) {
  // "cols": ["A8", "A9", "A10", "A11", "A12", "A15"],
  // return (GPIOA->IDR & COL_PIN_MASK);
  // return (GPIOA->IDR & (1 << ))

  return (palReadPad(GPIOA, 8) << 0) |
    (palReadPad(GPIOA, 9) << 1) |
    (palReadPad(GPIOA, 10) << 2) |
    (palReadPad(GPIOA, 11) << 3) |
    (palReadPad(GPIOA, 12) << 4) |
    (palReadPad(GPIOA, 15) << 5);

  // for (uint8_t col = 0; col < MATRIX_COLS; col++) {
  //   readPin(MATRIX_COL_PINS[col]);
  // }
}

matrix_row_t matrix_get_row(uint8_t row) {
  return matrix[row];
}

void matrix_print(void) {}

// #ifndef DEBOUNCE
// #define DEBOUNCE 10
// #endif

// // static uint8_t mcp23017_reset_loop = 0;

// void matrix_print(void) {}

// void transport_master_init(void) {}

// void transport_slave_init(void) {}

// void matrix_init(void) {
//   // mcp23017_status = init_mcp23017();

//   // (void) mcp23017_reset_loop;

//   init_rows();
//   unselect_rows();
//   init_cols();

// static void unselect_rows(void) {
//   // GPIOA->BSRR = 0b1111111 << 8; // make all pins of port A to outputs
//   // *((volatile uint32_t *)&GPIOA->BSRRL) = ((uint32_t)GPIO_Pin_8); //set bit
//   // GPIOA->IDR // read port A
//   // GPIOA->ODR // write port A
//   // *((volatile uint32_t *)&GPIOA->BSRRL) = ((uint32_t)GPIO_Pin_8) << 16; //reset bit

//   // "rows": ["A7", "A6", "A5", "A4", "A3", "A2"]

//   // palClearPad(GPIOA, 7);
//   // palClearPad(GPIOA, 6);
//   // palClearPad(GPIOA, 5);
//   // palClearPad(GPIOA, 4);
//   // palClearPad(GPIOA, 3);
//   // palClearPad(GPIOA, 2);

//   // uint8_t bits = 0x00;
//   // for (int colPins = 0; colPins < MATRIX_COLS; colPins++) {
//   //   bits ^= 0x01 << MATRIX_COLS(colPins);
//   // }

//   // pal_lld_writeport(GPIOA, bits);

//   for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
//     writePinLow(MATRIX_ROW_PINS[row]);
//   }
// }

static void select_row(uint8_t row) {
  if (row < MATRIX_ROWS_PER_SIDE) {
    // send signal to the other half
  } else {
    palClearPad(GPIOA, 9);
    palClearPad(GPIOA, 10);
    palClearPad(GPIOA, 11);
    palClearPad(GPIOA, 12);
    palClearPad(GPIOA, 15);

    if (row == 0)
      palSetPad(GPIOA, 9);
    else if (row == 1)
      palSetPad(GPIOA, 10);
    else if (row == 2)
      palSetPad(GPIOA, 11);
    else if (row == 3)
      palSetPad(GPIOA, 12);
    else if (row == 4)
      palSetPad(GPIOA, 15);
  }

  // palSetPad(GPIOA, row);
  // palClearPad(GPIOA, 9);
  // palClearPad(GPIOA, 10);
  // palClearPad(GPIOA, 11);
  // palClearPad(GPIOA, 12);
  // palClearPad(GPIOA, 15);
  // if (row < MATRIX_ROWS_PER_SIDE) {
  //   // if (!mcp23017_status) {
  //   //   uint8_t data = (0xFF & ~(1 << row));
  //   //   mcp23017_status = i2c_writeReg(I2C_ADDR, I2C_GPIOA, &data, 1, 10);
  //   // }
  // } else {
  //   GPIOA->BSRR = 0x1 << (row+1);
  // }
}
