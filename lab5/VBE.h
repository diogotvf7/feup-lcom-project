#ifndef VBE_H_
#define VBE_H_

#define BIT(n) (1 << (n))

#define AH_TEXT_MODE 0x00
#define AH_GRAPHICS_MODE 0x4F

#define AL_TEXT_MODE 0x03
#define AL_GRAPHICS_MODE 0x02

#define AX_TEXT_MODE 0x0003
#define AX_GRAPHICS_MODE 0x4F02

#define INTNO_TEXT_GRAPHICS_MODE 0X10

#define BX_TEXT_MODE 0x0000

#define BX_LINEAR BIT(14)

#define DIRECT_COLOR_MODE 0x06
#define VBE_1024x768p_INDEXED 0x105
#define VBE_640x480p_DC 0x110
#define VBE_800x600p_DC 0x115
#define VBE_1280x1024p_DC 0x11A
#define VBE_1152x864p_DC 0x14C

#endif
