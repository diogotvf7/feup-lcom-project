#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

/*          CONSTANTS          */
#define KBD_IRQ           0x1 
#define TIMER0_IRQ 0 /**< @brief Timer 0 IRQ line */
#define MOUSE_IRQ 12 /*Mouse 12 IRQ line*/
#define DOUBLE_BYTE       0xE0
#define DELAY_US          20000
#define READ_ATTEMPTS     10

/*      I/O PORT ADDRESSES     */
#define KBC_STAT_REG      0x64
#define KBC_CTRL_REG      0x64
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
#define BREAK_BIT BIT(7)
#define SIZE 0xE0
#define KBD_ENABLE_BIT BIT(0)

#define WRITE_BYTE_MOUSE 0xD4
#define ENABLE_DATA_REP 0xF4
#define DISABLE_DATA_REP 0xF5

#define ACK 0xFA
#define NACK 0xFE
#define ERROR 0xFC

#define S_KEY 31
#define G_KEY 34
#define E_KEY 18
#define Q_KEY 16

#endif
