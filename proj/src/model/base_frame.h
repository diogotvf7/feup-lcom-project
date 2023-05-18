#pragma once
#include <lcom/lcf.h>
// #include "sprite.h"
// #include "controller/mouse/mouse.h"
// #include "controller/keyboard/keyboard.h"
// #include "controller/timer/timer.h"
// #include "view/view.h"
// #include "xpm/mouse.xpm"

typedef struct {
  uint16_t width;
  uint16_t height;
  uint16_t bytes_per_pixel;
  unsigned size;
  uint8_t *base_addr;
} frame_buffer_t;

void create_frame_buffer(uint16_t width, uint16_t height, uint16_t bytes_per_pixel);

int draw_frame_pixel(uint16_t x, uint16_t y, uint32_t color);

