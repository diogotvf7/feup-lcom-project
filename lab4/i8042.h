#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define ENABLE_DATA_REPORTING 0xF4

#define DISABLE_DATA_REPORTING 0xF5

#define ACK 0xFA

#define NACK 0xFE

#define ERROR 0xFC

#define MOUSE_IRQ 3

#define KEYBOARD_IRQ 1 

#define ESC_MAKE_CODE 0x01

#define ESC_BREAK_CODE 0x81

#define DELAY_US  20000

#define KBC_STATUS_REG  0x64

#define KBC_IN_CMD      0x64

#define KBC_OUT_CMD     0x60

#define KBC_READ_CMD    0x20

#define KBC_WRITE_CMD   0x60

#define MAKE_CODE       BIT(7)

#define TWO_BYTES       0xE0


#endif



