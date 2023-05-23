#ifndef __GRAPHICS_CARD_H
#define __GRAPHICS_CARD_H

#include <lcom/lcf.h>
#include <lcom/video_gr.h>

#include "model/base_frame.h"
#include "model/colors.h"
#include "VBE.h"

void *(map_graphics_memory)(uint16_t mode);

int (vg_flip_frame)();

int (copy_base_frame)(frame_buffer_t frame_buffer);

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) ;

void *(vg_init)(uint16_t mode);

// int (vg_draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

uint32_t (red)(unsigned y, uint32_t first, uint8_t step);

uint32_t (green)(unsigned x, uint32_t first, uint8_t step);

uint32_t (blue)(unsigned x, unsigned y, uint32_t first, uint8_t step);

uint32_t (get_red_bits)(uint32_t color);

uint32_t (get_green_bits)(uint32_t color);

uint32_t (get_blue_bits)(uint32_t color);

uint32_t (index_color)(uint32_t first, uint8_t step, uint8_t no_rectangles, uint8_t row, uint8_t col);

uint32_t (direct_color)(uint32_t red, uint32_t green, uint32_t blue);

uint8_t (get_xpm_width)(xpm_map_t xpm);

uint8_t (get_xpm_height)(xpm_map_t xpm);

uint8_t (get_xpm_colors_no)(xpm_map_t xpm);

color_symbol *(get_xpm_colors)(xpm_map_t xpm, uint8_t colors_no);

#endif /* __GRAPHICS_CARD_H */
