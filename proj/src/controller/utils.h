#ifndef __UTILS_H
#define __UTILS_H

#include <lcom/lcf.h>
#include <stdint.h>

uint8_t(util_get_n_byte)(uint32_t val, int n);

int(util_get_LSB)(uint16_t val, uint8_t *lsb);

int(util_get_MSB)(uint16_t val, uint8_t *msb);

int (util_sys_inb)(int port, uint8_t *value);

uint8_t to_binary(uint8_t bcd_number);

#endif
