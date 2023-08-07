#include "matrix.h"
#include "hal.h"
#include "debounce.h"
#include "wait.h"
#include "timer.h"

#ifndef DEBOUNCE
#define DEBOUNCE 10
#endif

matrix_row_t matrix[MATRIX_ROWS];
matrix_row_t raw_matrix[MATRIX_ROWS];
uint8_t debounce_matrix[MATRIX_ROWS * MATRIX_COLS];

static matrix_row_t read_cols(uint8_t row);

static void select_row(uint8_t row);

matrix_row_t debounce_read_cols(uint8_t row);

void unselect_rows(void);

__attribute__((weak)) void matrix_init_user(void) {}

__attribute__((weak)) void matrix_scan_user(void) {}

__attribute__((weak)) void matrix_init_kb(void) {
  matrix_init_user();
}

__attribute__((weak)) void matrix_scan_kb(void) {
  matrix_scan_user();
}

void matrix_print(void) {}

/*
# Initialization

All col and row IO pins are configured as Input with pullups activated.
In my schematic case those are Row0, Row1, Col0 and Col1.

# Matrix scan

For each IO row pin(Row0, then Row1):

1. Configure row IO pin as output.
2. Set row IO pin in a low state (0) so that it sinks current.
3. Wait a little bit (QMK default is 30 microseconds).
4. For each IO col pin (Col0, then Col1):
  a. Read pin.
  b. If it is high, that means that the switch is opened and that the current flows from the pullup to the col pin.
  c. If it is low, that means that the switch is closed and that the current is sinked by the row pin, no current flows to the col pin.
5. Configure back row IO pin as input with pullup.
*/

// "cols": ["B12", "B13", "B14", "B15", "A8", "A15"],
// "rows": ["A7", "A6", "A5", "A1", "A3", "A2"]

static void init_cols(void) {
  // "cols": ["B12", "B13", "B14", "B15", "A8", "A15"],
  palSetPadMode(GPIOB, 12, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOB, 13, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOB, 14, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOB, 15, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOA, 8, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOA, 15, PAL_MODE_INPUT_PULLUP);
}

static void init_rows(void) {
  // "rows": ["A7", "A6", "A5", "A1", "A3", "A2"]
  // palSetPadMode(GPIOA, 7, PAL_MODE_OUTPUT_PUSHPULL);
  // palSetPadMode(GPIOA, 6, PAL_MODE_OUTPUT_PUSHPULL);
  // palSetPadMode(GPIOA, 5, PAL_MODE_OUTPUT_PUSHPULL);
  // palSetPadMode(GPIOA, 1, PAL_MODE_OUTPUT_PUSHPULL);
  // palSetPadMode(GPIOA, 3, PAL_MODE_OUTPUT_PUSHPULL);
  // palSetPadMode(GPIOA, 2, PAL_MODE_OUTPUT_PUSHPULL);

  palSetPadMode(GPIOA, 7, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOA, 6, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOA, 5, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOA, 1, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOA, 3, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOA, 2, PAL_MODE_INPUT_PULLUP);
}

void matrix_init(void) {
  // TODO: initialize hardware and global matrix state here
  init_rows();
  unselect_rows();
  init_cols();

  for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
    raw_matrix[row] = 0x00;
    matrix[row] = 0x00;

    for (uint8_t col = 0; col < MATRIX_COLS; col++) {
      debounce_matrix[row * MATRIX_COLS + col] = 0;
    }
  }

  // Unless hardware debouncing - Init the configured debounce routine
  debounce_init(MATRIX_ROWS);

  // This *must* be called for correct keyboard behavior
  matrix_init_kb();
}

void matrix_power_up(void) {
  init_rows();
  unselect_rows();
  init_cols();

  for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
    matrix[row] = 0;
  }
}

void unselect_rows(void) {
  // "rows": ["A7", "A6", "A5", "A1", "A3", "A2"]
  // GPIOA->BSRR =
  // ((1 << 7) |
  // (1 << 6) |
  // (1 << 5) |
  // (1 << 4) |
  // (1 << 3) |
  // (1 << 2)) << 16;

  // "rows": ["A7", "A6", "A5", "A1", "A3", "A2"]
  pal_lld_clearport(GPIOA, 7);
  pal_lld_clearport(GPIOA, 6);
  pal_lld_clearport(GPIOA, 5);
  pal_lld_clearport(GPIOA, 1);
  pal_lld_clearport(GPIOA, 3);
  pal_lld_clearport(GPIOA, 2);

  palSetPadMode(GPIOA, 7, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOA, 6, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOA, 5, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOA, 1, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOA, 3, PAL_MODE_INPUT_PULLUP);
  palSetPadMode(GPIOA, 2, PAL_MODE_INPUT_PULLUP);
}

uint8_t matrix_scan(void) {
  // bool changed = false;

  for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
    select_row(row);

    wait_us(50);

    // matrix[row] = debounce_read_cols(row);
    // matrix[row + MATRIX_ROWS_PER_SIDE] = debounce_read_cols(row + MATRIX_ROWS_PER_SIDE);

    matrix[row] = read_cols(row);
    // matrix[row + MATRIX_ROWS_PER_SIDE] = read_cols(row + MATRIX_ROWS_PER_SIDE);

    unselect_rows();
  }

  // Unless hardware debouncing - use the configured debounce routine
  // changed = debounce(raw_matrix, matrix, MATRIX_ROWS, changed);

  // This *must* be called for correct keyboard behavior
  matrix_scan_kb();

  return 1;
}

matrix_row_t debounce_mask(matrix_row_t rawcols, uint8_t row) {
  matrix_row_t result = 0;
  matrix_row_t change = rawcols ^ raw_matrix[row];

  raw_matrix[row] = rawcols;

  for (uint8_t col = 0; col < MATRIX_COLS; col++) {
    if (debounce_matrix[row * MATRIX_COLS + col]) {
      --debounce_matrix[row * MATRIX_COLS + col];
    } else {
      result |= (1 << col);
    }

    if (change & (1 << col)) {
      debounce_matrix[row * MATRIX_COLS + col] = DEBOUNCE;
    }
  }

  return result;
}

matrix_row_t read_cols(uint8_t row) {
  // "cols": ["B12", "B13", "B14", "B15", "A8", "A15"],
  if (row < MATRIX_ROWS_PER_SIDE) {
    return 0;
  }

  uint16_t portA = GPIOA->IDR;
  uint16_t portB = GPIOB->IDR;
  uint16_t bitsA = (~portA);
  uint16_t bitsB = (~portB);

  matrix_row_t result = 0x00;

  if (bitsB & (1 << 12)) result |= (1 << 5);
  if (bitsB & (1 << 13)) result |= (1 << 4);
  if (bitsB & (1 << 14)) result |= (1 << 3);
  if (bitsB & (1 << 15)) result |= (1 << 2);
  if (bitsA & (1 << 8)) result |= (1 << 1);
  if (bitsA & (1 << 15)) result |= (1 << 0);

  return result;
}

matrix_row_t matrix_get_row(uint8_t row) {
  return matrix[row];
}

matrix_row_t debounce_read_cols(uint8_t row) {
  // Read the row without debouncing filtering and store it for later usage.
  matrix_row_t cols = read_cols(row);

  // Get the Debounce mask.
  matrix_row_t mask = debounce_mask(cols, row);

  // debounce the row and return the result.
  return (cols & mask) | (matrix[row] & ~mask);
}

static void select_row(uint8_t row) {
  // "rows": ["A7", "A6", "A5", "A1", "A3", "A2"]
  if (row < MATRIX_ROWS_PER_SIDE) {
    // send signal to the other half
  } else {
    if (row == MATRIX_ROWS_PER_SIDE + 0) {
      palSetPadMode(GPIOA, 7, PAL_MODE_OUTPUT_PUSHPULL);
      pal_lld_setport(GPIOA, 7); // GPIOA->BSRR = (uint32_t)(0x01 << 7);
    }
    else if (row == MATRIX_ROWS_PER_SIDE + 1) {
      palSetPadMode(GPIOA, 6, PAL_MODE_OUTPUT_PUSHPULL);
      pal_lld_setport(GPIOA, 6); // GPIOA->BSRR = (uint32_t)(0x01 << 6);
    }
    else if (row == MATRIX_ROWS_PER_SIDE + 2) {
      palSetPadMode(GPIOA, 5, PAL_MODE_OUTPUT_PUSHPULL);
      pal_lld_setport(GPIOA, 5); // GPIOA->BSRR = (uint32_t)(0x01 << 5);
    }
    else if (row == MATRIX_ROWS_PER_SIDE + 3) {
      palSetPadMode(GPIOA, 1, PAL_MODE_OUTPUT_PUSHPULL);
      pal_lld_setport(GPIOA, 1); // GPIOA->BSRR = (uint32_t)(0x01 << 1);
    }
    else if (row == MATRIX_ROWS_PER_SIDE + 4) {
      palSetPadMode(GPIOA, 3, PAL_MODE_OUTPUT_PUSHPULL);
      pal_lld_setport(GPIOA, 3); // GPIOA->BSRR = (uint32_t)(0x01 << 3);
    }
    else if (row == MATRIX_ROWS_PER_SIDE + 5) {
      palSetPadMode(GPIOA, 2, PAL_MODE_OUTPUT_PUSHPULL);
      pal_lld_setport(GPIOA, 2); // GPIOA->BSRR = (uint32_t)(0x01 << 2);
    }
  }
}
