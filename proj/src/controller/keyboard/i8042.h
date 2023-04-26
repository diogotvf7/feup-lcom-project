#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/*          CONSTANTS          */
#define KBD_IRQ           0x1 
#define DOUBLE_BYTE       0xE0
#define DELAY_US          20000
#define READ_ATTEMPTS     10

/*      I/O PORT ADDRESSES     */
#define KBC_STAT_REG      0x64
#define KBC_CMD_REG       0x64
#define KBC_IN_BUF        0x60
#define KBC_OUT_BUF       0x60

/*    STATUS REGISTER BITS     */
#define PARITY_ERROR      BIT(7)
#define TIMEOUT_ERROR     BIT(6)
#define AUX               BIT(5)
#define INH               BIT(4)
#define IBF               BIT(1)
#define OBF               BIT(0)

/*          SCANCODES          */
#define SCANCODE_MSB      0x80
#define DOUBLE_SCANCODE   0xE0
#define ESC_MAKE          0x1
#define ESC_BREAK         0x81

/*          COMMANDS           */
#define READ_CMD_BYTE     0x20
#define WRITE_CMD_BYTE    0x60

/*         COMMAND BYTE        */
#define KBC_ENABLE_INT    BIT(0)

#endif /* _LCOM_I8254_H */
