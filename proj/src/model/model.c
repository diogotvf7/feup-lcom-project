#include "model.h"

Sprite *mouse;
Sprite *chooseColors;
Sprite* quitButton;
Sprite* startButton;
Sprite* numbers;
Sprite* letters;

int flag = 0, num_bytes = 1;
uint8_t scancode_arr[2];
extern int x, y;
extern frame_buffer_t frame_buffer;
extern real_time curr_time;
uint32_t color = RED;
int radius = 10;
SystemState systemState = RUNNING;
MenuState menuState = START;
GameState gameState;
int game_counter;
int offset;

int word_guess[10] = {-1};
int number_letters = 0;

void setup_sprites() {
    chooseColors = create_sprite_xpm((xpm_map_t) topBarGameMode_xpm);
    mouse = create_sprite_xpm((xpm_map_t) mouse_xpm);
    quitButton = create_sprite_xpm((xpm_map_t) quitButton_xpm);
    startButton = create_sprite_xpm((xpm_map_t) startButton_xpm);
    numbers = create_sprite_xpm((xpm_map_t) numbers_xpm);
    letters = create_sprite_xpm((xpm_map_t) letters_xpm);

}

void initGame(){
    menuState = GAME;
    game_counter = ROUND_TIME;
}

void update_mouse_state() {
    mouse_ih();
    parse_mouse_packet();
    if (get_byte_index() == 3) {
        if(menuState == GAME){
            if (get_mouse_packet()->lb) {

                if(y < 150 && gameState == DRAW){
                    updateDrawSpecs(&color, &radius);
                }
                if(y >= 150 && y < 750){
                    draw_frame_circle(x, y, radius, color);
                }
            }
            if (get_mouse_packet()->rb) {
                reset_frame();
            }
        }

        if(menuState == START){
            if(get_mouse_packet()->lb){
                if(x >= 451 && x <= 711 && y >= 300 && y <= 425){
                    initGame();
                }
                else if(x >= 451 && x <= 711 && y >= 500 && y <= 625){
                    systemState = EXIT;
                }
            }
        }
        updateMouseLocation();
    }
}

void update_timer_state() {
    timer_int_handler();
    if (get_counter() % 1 == 0) {
        vg_flip_frame();
        copy_base_frame(frame_buffer);
    }
    if (get_counter() % 30 == 0 && menuState == GAME){
        game_counter--;
        if (game_counter == 0){
            menuState = START;
        }
    }
    draw_new_frame();

}

void update_keyboard_state() {
    kbc_ih();
    if (get_scancode() == DOUBLE_BYTE) {
        scancode_arr[0] = get_scancode();
        flag = 1;
        num_bytes = 2;
    } else {
        scancode_arr[flag] = get_scancode();
        if (kbd_print_scancode(!((get_scancode() & SCANCODE_MSB) >> 7), num_bytes, scancode_arr) != OK)
            return;
        num_bytes = 1;
        flag = 0;
    }

    switch(get_scancode()){
        case ZERO_KEY:
            systemState = EXIT;
            break;
        case ONE_KEY:
            if (menuState == START) break;
            menuState = START;
            reset_frame();
            break;
        case TWO_KEY:
            if (menuState == GAME) break;
            initGame();
            gameState = DRAW;
            break;
        case THREE_KEY:
            if (menuState == GAME) break;
            initGame();
            gameState = GUESS;
            break;
        case FOUR_KEY:
            menuState = END;
            break;
        default:
            read_letter(get_scancode(), word_guess, &number_letters);
            break;
    }


}

void destroy_sprites() {
    destroy_sprite(mouse);
    destroy_sprite(chooseColors);
}




