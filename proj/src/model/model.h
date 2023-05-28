#pragma once
#include <lcom/lcf.h>
#include "sprite.h"
#include "controller/mouse/mouse.h"
#include "controller/rtc/rtc.h"
#include "controller/keyboard/keyboard.h"
#include "controller/graphics_card/graphics_card.h"
#include "controller/uart/ser_port.h"
#include "model/queue.h"
#include "controller/timer/timer.h"
#include "view/view.h"
#include "colors.h"
#include "xpm/mouse.xpm"
#include "xpm/topBarGameMode.xpm"
#include "xpm/quitButton.xpm"
#include "xpm/startButton.xpm"
#include "model/base_frame.h"
#include "global_vars.h"
#include "xpm/letters.xpm"
#include "xpm/numbers.xpm"
#include "xpm/leaderboardTable.xpm"



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

typedef struct leaderboardValue{
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t score;
} leaderboardValue;

void update_timer_state();
void update_keyboard_state();
void update_mouse_state();
void update_uart_state();
void setup_sprites();
void destroy_sprites();
void updateLeaderboard(leaderboardValue *newValue);
void loadLeaderboardFromFile(leaderboardValue leaderboard[]);
void saveLeaderboardToFile(const leaderboardValue leaderboard[]);
void clearLeaderboardFile();
/* int compareScores(const void* a, const void* b);
 */
