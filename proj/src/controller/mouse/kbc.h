#ifndef __KBC_H
#define __KBC_H

#include <lcom/lcf.h>
#include "i8042.h"

int (read_mouse_status)(uint8_t *st);

int (read_output_buffer)(uint8_t *out);

int (mouse_write)(int port, uint8_t cmd);

#endif /* __KBC_H */
