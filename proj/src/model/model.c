#include "model.h"

Sprite *mouse;
Sprite *chooseColors;
Sprite* quitButton;
Sprite* startButton;
Sprite* zero;

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

void setup_sprites() {
    chooseColors = create_sprite_xpm((xpm_map_t) topBarGameMode_xpm);
    mouse = create_sprite_xpm((xpm_map_t) mouse_xpm);
    quitButton = create_sprite_xpm((xpm_map_t) quitButton_xpm);
    startButton = create_sprite_xpm((xpm_map_t) startButton_xpm);
    zero = create_sprite_xpm((xpm_map_t) zero_xpm);

}

void update_mouse_state() {
    mouse_ih();
    parse_mouse_packet();
    if (get_byte_index() == 3) {
        // nao se pode meter a desenhar a linha depois de desenhar a nova posiçao do cursor porque vai desenhar por cima
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
                    menuState = GAME;
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
    else if(get_counter() % 30 == 0){ 
        rtc_init();
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
        case Q_KEY:
            systemState = EXIT;
            break;
        case S_KEY:
            menuState = START;
            reset_frame();
            break;
        case G_KEY:
            menuState = GAME;
            gameState = DRAW;
            break;
        case T_KEY:
            menuState = GAME;
            gameState = GUESS;
            break;
        case E_KEY:
            menuState = END;
            break;
        default:
            break;
    }



}

void destroy_sprites() {
    destroy_sprite(mouse);
    destroy_sprite(chooseColors);
}




