#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include <stdint.h>

void (kbc_ih)();

int (kbd_subscribe_int)(uint8_t *bit_no);

int (kbd_unsubscribe_int)();

void (kbd_int_handler)();

int (keyboard_restore)();

#endif
