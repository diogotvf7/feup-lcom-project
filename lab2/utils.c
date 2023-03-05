#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  if (lsb == NULL) return 1; // O apontador deve ser válido
  *lsb = 0xFF & val;         // Coloca no apontador os 8 bits menos significativos do valor
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  if (msb == NULL) return 1; // O apontador deve ser válido
  *msb = (val >> 8) & 0xFF;  // Coloca no apontador os 8 bits mais significativos do valor
  return 0;
}

int (util_sys_inb)(int port, uint8_t *value) {
  uint32_t v;
  if (sys_inb(port,&v)) return EXIT_FAILURE;
  *(value) = 0xFF & v;
  return 0;
}
