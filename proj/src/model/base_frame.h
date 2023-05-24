#pragma once
#include <lcom/lcf.h>
#include "model/mouse_packet_queue.h"
#include "colors.h"

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

void draw_frame_circle(Position *p, uint16_t thickness, uint32_t color);

void draw_bresenham_line(Position *p1, Position *p2, uint32_t color, uint16_t thickness);

void reset_frame();
