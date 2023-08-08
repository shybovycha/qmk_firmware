#include "matrix.h"
#include <string.h>
#include "timer.h"
#include "wait.h"
#include "debug.h"
#include <stdint.h>
#include <stdbool.h>
#include <hal.h>
// #include "i2c_master.h"

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

bool is_keyboard_left(void);

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
  if ((is_keyboard_left() && row >= MATRIX_ROWS_PER_SIDE) || (!is_keyboard_left() && row < MATRIX_ROWS_PER_SIDE)) {
    // read from usart
    return 0;
  }

  if (is_keyboard_left()) {
    // left:
    // "cols": ["B12", "B14", "B15", "A0", "A1", "A15"],
    return
      ((palReadPad(GPIOB, 12) ? 0 : 1) << 5) |
      ((palReadPad(GPIOB, 14) ? 0 : 1) << 4) |
      ((palReadPad(GPIOB, 15) ? 0 : 1) << 3) |
      ((palReadPad(GPIOA, 0) ? 0 : 1) << 2) |
      ((palReadPad(GPIOA, 1) ? 0 : 1) << 1) |
      ((palReadPad(GPIOA, 15) ? 0 : 1) << 0)
    ;
  } else {
    // right:
    // "cols": ["B12", "B13", "B14", "B15", "A8", "A15"],
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
  if (is_keyboard_left()) {
    // left:
    // "cols": ["B12", "B14", "B15", "A0", "A1", "A15"],
    palSetPadMode(GPIOB, 12, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOB, 14, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOB, 15, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA, 0, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA, 1, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA, 15, PAL_MODE_INPUT_PULLUP);
  } else {
    // right:
    // "cols": ["B12", "B13", "B14", "B15", "A8", "A15"],
    palSetPadMode(GPIOB, 12, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOB, 13, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOB, 14, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOB, 15, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA, 8, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA, 15, PAL_MODE_INPUT_PULLUP);
  }
}

static void init_rows(void) {
  // "rows": ["A7", "A6", "A5", "B3", "B4", "B5"]
  palSetPadMode(GPIOA, 7, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOA, 6, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOA, 5, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOB, 3, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOB, 4, PAL_MODE_OUTPUT_PUSHPULL);
  palSetPadMode(GPIOB, 5, PAL_MODE_OUTPUT_PUSHPULL);
}

static void unselect_rows(void) {
  // set row pins to high
  pal_lld_setport(GPIOA, (1 << 7) | (1 << 6) | (1 << 5));
  pal_lld_setport(GPIOB, (1 << 3) | (1 << 4) | (1 << 5));
}

static void select_row(uint8_t row) {
  // if (row >= MATRIX_ROWS_PER_SIDE) {
  //   // if (!mcp23017_status) {
  //   //   uint8_t data = (0xFF & ~(1 << row));
  //   //   mcp23017_status = i2c_writeReg(I2C_ADDR, I2C_GPIOA, &data, 1, 10);
  //   // }
  // } else {
    // "rows": ["A7", "A6", "A5", "B3", "B4", "B5"]
    // right half
    if (row == MATRIX_ROWS_PER_SIDE + 0) palClearPad(GPIOA, 7);
    if (row == MATRIX_ROWS_PER_SIDE + 1) palClearPad(GPIOA, 6);
    if (row == MATRIX_ROWS_PER_SIDE + 2) palClearPad(GPIOA, 5);
    if (row == MATRIX_ROWS_PER_SIDE + 3) palClearPad(GPIOB, 3);
    if (row == MATRIX_ROWS_PER_SIDE + 4) palClearPad(GPIOB, 4);
    if (row == MATRIX_ROWS_PER_SIDE + 5) palClearPad(GPIOB, 5);

    // left half
    if (row == 0) palClearPad(GPIOA, 7);
    if (row == 1) palClearPad(GPIOA, 6);
    if (row == 2) palClearPad(GPIOA, 5);
    if (row == 3) palClearPad(GPIOB, 3);
    if (row == 4) palClearPad(GPIOB, 4);
    if (row == 5) palClearPad(GPIOB, 5);
  // }
}

bool is_keyboard_left(void) {
  static enum { UNKNOWN, LEFT, RIGHT } hand_side = UNKNOWN;

  // only check once, as this is called often
  if (hand_side == UNKNOWN) {
    // #if defined(SPLIT_HAND_PIN)
    // Test pin SPLIT_HAND_PIN for High/Low, if low it's right hand
    // setPinInput(SPLIT_HAND_PIN);
    // hand_side = readPin(SPLIT_HAND_PIN) ? LEFT : RIGHT;
    palSetPadMode(GPIOB, 10, PAL_MODE_INPUT_PULLUP);
    hand_side = palReadPad(GPIOB, 10) ? LEFT : RIGHT;
  }

  return (hand_side == LEFT);
}
