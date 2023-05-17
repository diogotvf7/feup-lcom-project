#include "base_frame.h"


frame_buffer_t frame_buffer;
uint16_t _width;
uint16_t _height;
uint8_t _bytes_per_pixel;
unsigned _size;
uint8_t *_base_addr;

void create_frame_buffer(uint16_t width, uint16_t height, uint16_t bytes_per_pixel) {
  frame_buffer.width = width;
  frame_buffer.height = height;
  frame_buffer.bytes_per_pixel = bytes_per_pixel;
  frame_buffer.size = width * height * bytes_per_pixel;
  frame_buffer.base_addr = (uint8_t *)malloc(frame_buffer.size);
  bzero(frame_buffer.base_addr, frame_buffer.size);
}

int draw_frame_pixel(uint16_t x, uint16_t y, uint32_t color) {
  if (frame_buffer.base_addr == NULL) 
    return EXIT_FAILURE;
  if (x >= frame_buffer.width || x < 0 || y >= frame_buffer.height || y < 0)
    return EXIT_FAILURE;

  int pixel_pos = frame_buffer.width * y + x;
  int byte_offset = pixel_pos * frame_buffer.bytes_per_pixel;

  if (memcpy(
    &_base_addr[byte_offset], 
    &color, 
    (unsigned) frame_buffer.bytes_per_pixel) == NULL)
      return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
