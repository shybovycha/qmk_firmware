# MCU name
MCU = STM32F103
MCU_LDSCRIPT = STM32F103x6_stm32duino_bootloader # stm32f103_bootloader

# Bootloader selection
# BOARD = ST_NUCLEO64_F103RB
# MCU_LDSCRIPT = STM32F103xB # Configure for 128K flash
# BOOTLOADER = custom

# Bootloader selection
# Cannot use `BOOTLOADER = stm32duino` due to the need to override
# `MCU_LDSCRIPT`, therefore all parameters need to be specified here manually.
OPT_DEFS += -DBOOTLOADER_STM32DUINO
# MCU_LDSCRIPT = k552_f103
BOARD = STM32_F103_STM32DUINO
BOOTLOADER_TYPE = stm32duino
STM32_BOOTLOADER_ADDRESS = 0x80000000
DFU_ARGS = -d 1EAF:0003 -a 2 -R
DFU_SUFFIX_ARGS = -v 1EAF -p 0003

BOOTMAGIC_ENABLE = no       # Enable Bootmagic Lite
MOUSEKEY_ENABLE = no	# Mouse keys
EXTRAKEY_ENABLE = yes	# Audio control and System control
CONSOLE_ENABLE = no	# Console for debug
COMMAND_ENABLE = no    # Commands for debug and configuration
NKRO_ENABLE = yes           # Enable N-Key Rollover
CUSTOM_MATRIX = yes # Custom matrix file
UNICODE_ENABLE = yes # Unicode
SPLIT_KEYBOARD = yes

SPLIT_TRANSPORT = custom # https://github.com/qmk/qmk_firmware/blob/master/docs/config_options.md#split-keyboard-options

# Enter lower-power sleep mode when on the ChibiOS idle thread
OPT_DEFS += -DCORTEX_ENABLE_WFI_IDLE=TRUE

SRC += matrix.c
QUANTUM_LIB_SRC += i2c_master.c

# Wear-levelling driver
EEPROM_DRIVER = wear_leveling
WEAR_LEVELING_DRIVER = embedded_flash
