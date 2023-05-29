#include "utils.h"

int (util_sys_inb)(int port, uint8_t *value){
  if (value == NULL) return 1;
  uint32_t val = 0;
  if (sys_inb(port,&val) != 0){ return 1;}
  *value = 0xFF & val;
  return 0;
}

int (util_get_LSB)(uint16_t val, uint8_t *lsb){
  if (lsb == NULL) return 1;
  *lsb = 0xFF & val;
  return 0;
}

int (util_get_MSB)(uint16_t val, uint8_t *msb){
  if (msb == NULL) return 1;
  *msb = val >> 8;
  return 0;
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
