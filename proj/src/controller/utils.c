#include <lcom/lcf.h>

#include <stdint.h>
#include "utils.h"


uint8_t(util_get_n_byte)(uint32_t val, int n) {
  return (val >> 8 * n) & 0xFF;
}

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1;
  *lsb = val & 0xFF;
  return EXIT_SUCCESS;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1;
  *msb = (val >> 8) & 0xFF;
  return EXIT_SUCCESS;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t buffer;
  if (sys_inb(port, &buffer) != 0)
    return EXIT_FAILURE;
  *value = buffer;
  return EXIT_SUCCESS;
}

uint8_t to_binary(uint8_t bcd_number) {
  return ((bcd_number >> 4) * 10) + (bcd_number & 0xF);
}

void printBits(size_t const size, void const * const ptr)
{
  unsigned char *b = (unsigned char*) ptr;
  unsigned char byte;
  int i, j;
  
  for (i = size-1; i >= 0; i--) {
    for (j = 7; j >= 0; j--) {
      byte = (b[i] >> j) & 1;
      printf("%u", byte);
    }
  }
  printf("\n");
  // puts("");
}

void append_char(char *s, char c, int *size) {
  int len = strlen(s);
  s[len] = c;
  s[len+1] = '\0';
  *size += 1;
}
