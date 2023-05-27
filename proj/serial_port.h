#include <lcom/lcf.h>
#include <stdint.h>
#include <stdbool.h>
#include "src/controller/utils.h"

#define COM1_IRQ 4
#define COM1_PORT 0x3F8

#define BIT(n) (1<<(n))


#define SP_INIT 2

/*              RBR               */
#define SER_RBR                   0

/*              THR               */
#define SER_THR                   0

/*              LSR               */
#define SER_LSR                   5
#define SER_RCVR_DATA           BIT(0)
#define SER_OVERRUN_ERROR       BIT(1)
#define SER_PARITY_ERROR        BIT(2)
#define SER_FRAMING_ERROR       BIT(3)
#define SER_UART_READY          BIT(5)
#define SER_TXMIT_EMPTY         BIT(6)

/*              IER               */
#define SER_IER                   1
#define SER_INT_PENDING          BIT(0)
#define SER_RCVR_LINE_STATUS    (BIT(2) | BIT(1))
#define SER_RCVR_DATA_AVLBL     (BIT(2))
#define SER_CHR_TIMEOUT         (BIT(3) | BIT(2))
#define SER_TX_EMPTY            (BIT(1))

int (sp_init)();

int (sp_subscribe_int)(uint8_t *bit_no);

int (sp_unsubscribe_int)();

int (sp_read_status)(uint8_t *st);

int (sp_read_data)();

int (sp_txmit_data)(uint8_t data);

int (sp_check_connection)();
 