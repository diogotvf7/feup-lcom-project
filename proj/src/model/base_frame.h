#pragma once
#include <lcom/lcf.h>
// #include "sprite.h"
// #include "controller/mouse/mouse.h"
// #include "controller/keyboard/keyboard.h"
// #include "controller/graphics_card/graphics_card.h"
// #include "controller/timer/timer.h"
// #include "view/view.h"
// #include "xpm/mouse.xpm"

// struct  {
//   uint32_t width;
//   uint32_t height;
//   uint32_t bytes_per_pixel;
//   uint8_t *pixel_data;
// } frame_buffer;

void *create_frame_buffer(uint32_t width, uint32_t height, uint32_t bytes_per_pixel);
