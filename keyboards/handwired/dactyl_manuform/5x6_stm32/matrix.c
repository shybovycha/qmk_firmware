#include "matrix.h"
#include "hal.h"
#include "debounce.h"
#include "wait.h"
#include "timer.h"

matrix_row_t matrix[MATRIX_ROWS];
matrix_row_t raw_matrix[MATRIX_ROWS];
uint8_t debounce_matrix[MATRIX_ROWS * MATRIX_COLS];

static matrix_row_t read_cols(uint8_t row);

static void select_row(uint8_t row);

__attribute__((weak)) void matrix_init_user(void) {}

__attribute__((weak)) void matrix_scan_user(void) {}

__attribute__((weak)) void matrix_init_kb(void) {
  matrix_init_user();
}

__attribute__((weak)) void matrix_scan_kb(void) {
  matrix_scan_user();
}

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
  return (palReadPad(GPIOA, 8) << 0) |
    (palReadPad(GPIOA, 9) << 1) |
    (palReadPad(GPIOA, 10) << 2) |
    (palReadPad(GPIOA, 11) << 3) |
    (palReadPad(GPIOA, 12) << 4) |
    (palReadPad(GPIOA, 15) << 5);
}

matrix_row_t matrix_get_row(uint8_t row) {
  return matrix[row];
}

void matrix_print(void) {}

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
}
