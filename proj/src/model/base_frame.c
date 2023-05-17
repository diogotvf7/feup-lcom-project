#include "base_frame.h"

void *create_frame_buffer(uint32_t width, uint32_t height, uint32_t bytes_per_pixel) {
  uint8_t *mem = (uint8_t *)malloc(width * height * bytes_per_pixel);
  bzero(mem, sizeof mem);
  return mem;
}
