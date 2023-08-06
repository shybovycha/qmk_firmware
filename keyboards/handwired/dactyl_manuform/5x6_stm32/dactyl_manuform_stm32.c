#include "dactyl_manuform_stm32.h"
#include "i2c_master.h"

// volatile int mcp23017_status = 0x20;
// uint8_t i2c_initializied = 0;

void board_init(void) {
    // AFIO->MAPR |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
}

void bootloader_jump(void) {
    // This board doesn't use the "standard" stm32duino bootloader, and is resident in memory at the base location. All we can do here is reset.
    // NVIC_SystemReset();
}

void matrix_init_kb(void)
{
    matrix_init_user();
}

// uint8_t init_mcp23017(void) {
//     if (!i2c_initializied) {
//         i2c_init();
//         i2c_initializied = 1;
//     }

//     uint8_t data[2];
//     data[0] = 0x0;
//     data[1] = 0b00111111;
//     mcp23017_status = i2c_writeReg(I2C_ADDR, I2C_IODIRA, data, 2, 50000);
//     if (mcp23017_status) goto out;
//     data[0] = 0xFFU;
//     mcp23017_status = i2c_writeReg(I2C_ADDR, I2C_GPIOA, data, 1, 5000);
//     if (mcp23017_status) goto out;
//     mcp23017_status = i2c_writeReg(I2C_ADDR, I2C_GPPUB, data+1, 1, 2);
//     if (mcp23017_status) goto out;

//  out:
//     return mcp23017_status;
//     // i2c_readReg(I2C_ADDR, );
// }

