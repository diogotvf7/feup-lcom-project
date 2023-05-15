#pragma once
#include <lcom/lcf.h>
#include "sprite.h"
#include "controller/mouse/mouse.h"
#include "controller/keyboard/keyboard.h"
#include "controller/graphics_card/graphics_card.h"
#include "controller/timer/timer.h"
#include "view/view.h"
#include "xpm/mouse.xpm"


typedef enum{
    RUNNING, 
    EXIT,
} SystemState;

typedef enum{
    START,
    GAME,
    END
} MenuState;

void update_timer_state();
void update_keyboard_state();
void update_mouse_state();
void setup_sprites();
void destroy_sprites();

