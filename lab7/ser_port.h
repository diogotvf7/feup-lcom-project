#include <lcom/lcf.h>

#include "global_vars.h"
#include "utils.h"
#include "SER.h"
#include "keyboard.h"
#include "queue.h"

int config_uart();

int set_uart_dlab(uint8_t status);

int subscribe_uart_int(uint8_t *bit_no);

int set_uart_fcr(uint8_t byte);

int unsubscribe_uart_int();

int set_uart_ier(uint8_t byte);

int get_uart_ier(uint8_t *byte);

int set_uart_lcr(uint8_t byte);

int get_uart_lcr(uint8_t *byte);

int set_uart_iir(uint8_t byte);

int get_uart_iir(uint8_t *byte);

int get_uart_lsr(uint8_t *byte);

int set_uart_freq(uint16_t freq);

void uart_ih();

int send_uart_byte(uint8_t byte);

int receive_uart_byte(uint8_t *byte);

/* tenso daqui para baixo */

// int uart_send_byte(uint8_t byte);

// int uart_receive_byte(uint8_t *byte);

// int uart_send_string(char *string);

// int uart_receive_string(char *string);

// int uart_get_conf(unsigned long *bits, unsigned long *stop, long *parity, unsigned long *rate);

// int uart_poll();

// int uart_int();

// int uart_fifo();

