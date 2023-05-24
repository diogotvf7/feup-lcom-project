#pragma once

#include <lcom/lcf.h>
#include "controller/mouse/mouse.h"
#include "controller/graphics_card/graphics_card.h"
#include "model/sprite.h"
#include "model/colors.h"
#include "model/model.h"



void draw_new_frame();
void draw_mouse();
void draw_initial_menu();
void draw_game_menu();
void draw_finish_menu();
int draw_sprite_xpm(Sprite *sprite, int x, int y);
int draw_bar(int x, int y, int width, int height, uint32_t color);
int draw_bottom_bar(int x, int y, int width, int height, uint32_t color, int square_vertex_x , int square_vertex_y , int square_width , int square_height);
int draw_game_time(int num);
int draw_letter(int x, int y, int offset);

