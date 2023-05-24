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
#include "xpm/startButton.xpm"
#include "model/base_frame.h"
#include "xpm/0.xpm"
#include "xpm/1.xpm"
#include "xpm/2.xpm"
#include "xpm/9.xpm"
#include "xpm/3.xpm"
#include "xpm/4.xpm"
#include "xpm/8.xpm"
#include "xpm/5.xpm"
#include "xpm/7.xpm"
#include "xpm/6.xpm"
#include "xpm/a.xpm"
#include "xpm/b.xpm"


#define ROUND_TIME 60

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

