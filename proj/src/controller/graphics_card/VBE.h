#ifndef _LCOM_VBE_
#define _LCOM_VBE_

#include <lcom/lcf.h>

/*        MASK                */
#define COLOR_SIZE_MASK(n)  BIT(n) - 1

/*        BIOS SERVICES       */
#define GRAPHICS_INT_NO     0x10

/*          FUNCTIONS         */
#define VESA_FUNC           0x4F
#define BIOS_FUNC           0x00

/*                            */
#define SET_GRAPHICS_MODE   0x02

#define INDEXED_COLOR_MODE  0x105

#define DIRECT_COLOR_MODE   0x06
#define PACKED_PIXEL_MODE   0x04
#define LINEAR_MODE         BIT(14)

typedef struct {
  uint8_t color_index;
  char symbol;
} color_symbol;

#endif /* _LCOM_VBE */
