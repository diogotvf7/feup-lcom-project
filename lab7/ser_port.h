#include <lcom/lcf.h>

int ser_subscribe_int(uint8_t *bit_no);

int ser_unsubscribe_int();

int ser_send_byte(uint8_t byte);

int ser_receive_byte(uint8_t *byte);

int ser_send_string(char *string);

int ser_receive_string(char *string);

int ser_set_conf(unsigned long bits, unsigned long stop, long parity, unsigned long rate);

int ser_get_conf(unsigned long *bits, unsigned long *stop, long *parity, unsigned long *rate);

void ser_ih();

int ser_poll();

int ser_int();

int ser_fifo();