#ifndef __MOUSE_H
#define __MOUSE_H

#include <lcom/lcf.h>
#include "kbc.h"
#include "model/colors.h"
#include "i8042.h"

int (get_byte_index)();

struct packet *(get_mouse_packet)();

int (mouse_subscribe_int)(int *bit_no);

int (mouse_unsubscribe_int)();

int (set_data_reporting)(bool enable); 

void (mouse_ih)();

void (parse_mouse_packet)();

void updateMouseLocation();

void updateDrawSpecs(uint32_t *color, int *radius);

#endif /* __MOUSE_H */
