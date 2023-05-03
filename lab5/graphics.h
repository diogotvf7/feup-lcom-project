#include <lcom/lcf.h>
#include "vbe.h"
#include <math.h>

#ifndef _LCOM_GRAPHICS_H_
#define _LCOM_GRAPHICS_H_

int(set_graphic_mode)(uint16_t submode);

int(set_text_mode)();

int (create_frame_buffer)(uint16_t mode);

int (normalize_color)(uint32_t color, uint32_t *newColor);

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int (vg_draw_rectangulo)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int (vg_draw_hline)(uint16_t x, uint16_t y,uint16_t width, uint32_t color);

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

uint32_t (indexed_color_mode)(uint32_t first, uint32_t step, uint32_t row, uint32_t col, uint32_t no_rectangles);

uint32_t (direct_color_mode)(uint32_t red, uint32_t green, uint32_t blue);

uint32_t (R)(uint32_t first);

uint32_t (G)(uint32_t first);

uint32_t (B)(uint32_t first);

#endif
