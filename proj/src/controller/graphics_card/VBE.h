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
#define SET_DISPLAY_START   0x07

#define INDEXED_COLOR_MODE  0x105

#define VBE_MODE            0x14C
#define DIRECT_COLOR_MODE   0x06
#define PACKED_PIXEL_MODE   0x04
#define LINEAR_MODE         BIT(14)

#define VBE_RETURN_SUCCESS  0x004F

#define RED 0xff0000
#define ORANGE 0xd17b4f
#define BLUE 0x19DBF0
#define GREEN 0x2FF019
#define YELLOW 0xF5DA2A
#define DARKBLUE 0x0000ff
#define PRESSED 0x333FFF
#define TRANSPARENT 0xFFFFFE



typedef struct {
  uint8_t color_index;
  char symbol;
} color_symbol;

#endif /* _LCOM_VBE */
