#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <lcom/lcf.h>

int (keyboard_subscribe_interrupts)(uint8_t *bit_no);

int (keyboard_unsubscribe_interrupts)();

void (kbc_ih)();

int (keyboard_restore)();

#endif
