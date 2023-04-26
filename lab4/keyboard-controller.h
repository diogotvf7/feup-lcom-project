#ifndef _KEYBOARD_CONTROLLER_H 
#define _KEYBOARD_CONTROLLER_H

#include "utils.c"
#include <stdlib.h>

int (read_KBC_status)(uint8_t* status);

int (write_KBC_command)(uint8_t port, uint8_t commandByte);

int (read_KBC_output)(uint8_t port, uint8_t* output, uint8_t mouse);

#endif
