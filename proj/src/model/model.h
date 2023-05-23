#pragma once
#include <lcom/lcf.h>
#include "sprite.h"
#include "controller/mouse/mouse.h"
#include "controller/rtc/rtc.h"
#include "controller/keyboard/keyboard.h"
#include "controller/graphics_card/graphics_card.h"
#include "controller/timer/timer.h"
#include "view/view.h"
#include "colors.h"
#include "xpm/mouse.xpm"
#include "xpm/topBarGameMode.xpm"
#include "xpm/quitButton.xpm"
#include "xpm/zero.xpm"
#include "xpm/startButton.xpm"
#include "model/base_frame.h"


typedef enum{
    RUNNING, 
    EXIT,
} SystemState;

typedef enum{
    START,
    GAME,
    END
} MenuState;

typedef enum{
    DRAW,
    GUESS
} GameState;

void update_timer_state();
void update_keyboard_state();
void update_mouse_state();
void setup_sprites();
void destroy_sprites();
