#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

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



