BOOTMAGIC_ENABLE = no   # Enable Bootmagic Lite
MOUSEKEY_ENABLE = no	# Mouse keys
EXTRAKEY_ENABLE = yes	# Audio control and System control
CONSOLE_ENABLE = no	    # Console for debug
COMMAND_ENABLE = no     # Commands for debug and configuration
NKRO_ENABLE = yes       # Enable N-Key Rollover
CUSTOM_MATRIX = yes     # Custom matrix file
UNICODE_ENABLE = yes    # Unicode
SPLIT_KEYBOARD = yes

# SPLIT_TRANSPORT = custom # https://github.com/qmk/qmk_firmware/blob/master/docs/config_options.md#split-keyboard-options
SERIAL_DRIVER = usart

SRC += matrix.c
