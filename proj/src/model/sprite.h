#pragma once
#include <lcom/lcf.h>

typedef struct {
    uint16_t height, width;
    uint32_t *colors;
    uint32_t color;
} Sprite;

Sprite *create_sprite_xpm(xpm_map_t sprite);

void destroy_sprite(Sprite *sprite);

