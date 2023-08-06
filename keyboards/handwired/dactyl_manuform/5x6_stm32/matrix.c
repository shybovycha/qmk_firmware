#include "matrix.h"
// #include <string.h>
// #include "timer.h"
// #include "wait.h"
// #include "debug.h"
// #include "i2c_master.h"
// #include "dactyl_manuform_stm32.h"

static matrix_row_t matrix[MATRIX_ROWS];

void matrix_init(void) {}

uint8_t matrix_scan(void) {
  for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
    select_row(row);

    /* The default hardware works down to at least 100us, but I have a replacement
     * photodiode that responds a little more slowly. Cranking it up to 1000us fixes
     * shadowing issues.
     */
    _delay_us(1000);

    matrix[row] = read_cols();
  }

  // matrix_scan_kb();

  return 1;
}

matrix_row_t read_cols(void) {
  return (GPIOA-> & 0b00111100) >> 2;
}

matrix_row_t matrix_get_row(uint8_t row) {
  return matrix[row];
}

void matrix_print(void) {}

// #ifndef DEBOUNCE
// #define DEBOUNCE 10
// #endif

// // static uint8_t mcp23017_reset_loop = 0;

// volatile matrix_row_t matrix[MATRIX_ROWS];
// volatile matrix_row_t raw_matrix[MATRIX_ROWS];
// volatile uint8_t debounce_matrix[MATRIX_ROWS * MATRIX_COLS];

// static matrix_row_t read_cols(uint8_t row);

// static void init_cols(void);

// static void unselect_rows(void);

// static void select_row(uint8_t row);

// static void init_rows(void);

// __attribute__((weak)) void matrix_init_user(void) {}

// __attribute__((weak)) void matrix_scan_user(void) {}

// __attribute__((weak)) void matrix_init_kb(void) {
//   matrix_init_user();
// }

// __attribute__((weak)) void matrix_scan_kb(void) {
//   matrix_scan_user();
// }

// void matrix_print(void) {}

// void transport_master_init(void) {}

// void transport_slave_init(void) {}

// void matrix_init(void) {
//   // mcp23017_status = init_mcp23017();

//   // (void) mcp23017_reset_loop;

//   init_rows();
//   unselect_rows();
//   init_cols();

static void unselect_rows(void) {
  // GPIOA->BSRR = 0b1111111 << 8; // make all pins of port A to outputs
  // *((volatile uint32_t *)&GPIOA->BSRRL) = ((uint32_t)GPIO_Pin_8); //set bit
  // GPIOA->IDR // read port A
  // GPIOA->ODR // write port A
  // *((volatile uint32_t *)&GPIOA->BSRRL) = ((uint32_t)GPIO_Pin_8) << 16; //reset bit
  palClearPad(GPIOA, 8);
  palClearPad(GPIOA, 9);
  palClearPad(GPIOA, 10);
  palClearPad(GPIOA, 11);
  palClearPad(GPIOA, 12);
  palClearPad(GPIOA, 15);
  // palSetPad(GPIOA, 8);


  // uint8_t bits = 0x00;
  // for (int colPins = 0; colPins < MATRIX_COLS; colPins++) {
  //   bits ^= 0x01 << MATRIX_COLS(colPins);
  // }
  // pal_lld_writeport(GPIOA, bits);
}

static void select_row(uint8_t row) {
  palSetPad(GPIOA, row);
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
