#ifndef _MOUSE_H_
#define _MOUSE_H_

#include <lcom/lcf.h>
#include "i8042.h"
#include "keyboard-controller.h"

int (mouse_subscribe_int)(uint8_t *bit_no);

int (mouse_unsubscribe_int)();

void (mouse_ih)();

int (mouse_write)(uint8_t* mouse_response, uint8_t command );

void (mouse_receive)();

int (my_mouse_enable_data_reporting)();

int (my_mouse_disable_data_reporting)();

void (buildpacket)(uint8_t bytes[3], struct packet pack);

#endif
