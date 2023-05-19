#pragma once

#include <lcom/lcf.h>
#include "controller/mouse/mouse.h"
#include "controller/graphics_card/graphics_card.h"
#include "model/sprite.h"
#include "model/colors.h"



void draw_new_frame();
void draw_mouse();
void draw_initial_menu();
void draw_game_menu();
void draw_finish_menu();
int draw_sprite_xpm(Sprite *sprite, int x, int y);

