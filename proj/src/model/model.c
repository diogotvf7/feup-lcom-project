#include "model.h"

Sprite *mouse;
Sprite *chooseColors;
Sprite* quitButton;
Sprite* startButton;
Sprite* numbers;
Sprite* a;
Sprite* b;

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

extern struct Queue *pos_queue;
extern struct Queue *garbage;

void setup_sprites() {
    chooseColors = create_sprite_xpm((xpm_map_t) topBarGameMode_xpm);
    mouse = create_sprite_xpm((xpm_map_t) mouse_xpm);
    quitButton = create_sprite_xpm((xpm_map_t) quitButton_xpm);
    startButton = create_sprite_xpm((xpm_map_t) startButton_xpm);
    numbers = create_sprite_xpm((xpm_map_t) numbers_xpm);
    a = create_sprite_xpm((xpm_map_t) a_xpm);
    b = create_sprite_xpm((xpm_map_t) b_xpm);

}

void initGame(){
    menuState = GAME;
    game_counter = ROUND_TIME;
}

void update_mouse_state() {
    mouse_ih();
    parse_mouse_packet();
    if (get_byte_index() == 3) {
        //nao se pode meter a desenhar a linha depois de desenhar a nova posiçao do cursor porque vai desenhar por cima
        switch (menuState) {
            case START:
                if(get_mouse_packet()->lb){
                    if(x >= 451 && x <= 711 && y >= 300 && y <= 425){
                        initGame();
                    }
                    else if(x >= 451 && x <= 711 && y >= 500 && y <= 625){
                        systemState = EXIT;
                    }
                }
                break;
            case GAME:
                if (get_mouse_packet()->lb) {
                    if (y < 150) {
                        updateDrawSpecs(&color, &radius);
                        while (!queue_empty(&pos_queue)) {
                            if (process_packet(color, radius) != 0)
                                break;
                        }
                        queue_clear(&pos_queue);
                        queue_clear(&garbage);
                    }
                    if (y >= 150) {
                        Position *position = (Position *) malloc(sizeof(Position));
                        position->x = x;
                        position->y = y;
                        queue_push(&pos_queue, position);
                    }
                } else {
                    queue_clear(&pos_queue);
                    queue_clear(&garbage);
                }
                if (get_mouse_packet()->rb) {
                    reset_frame();
                    queue_clear(&pos_queue);
                    queue_clear(&garbage);
                }
                if(menuState == START){
                }         
                break;
            default:
                break;
        }

        updateMouseLocation();   
    }
}

void update_timer_state() {
    timer_int_handler();
    vg_flip_frame();
    copy_base_frame(frame_buffer);
    for (int i = PACKETS_PER_INTERRUPT; i; i--) {
        if (process_packet(color, radius) != 0)
            break;
    }
    if (get_counter() % 30 == 0 && menuState == GAME){
        game_counter--;
        if (game_counter == 0){
            menuState = START;
        }
    }
    // Se diminuirmos o base frame para nao ocupar a memoria onde fica a barra de cima e a de baixo so precisamos de dar print a barra de cima quando o rato esta por cima dela
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
        // if (kbd_print_scancode(!((get_scancode() & SCANCODE_MSB) >> 7), num_bytes, scancode_arr) != OK)
        //     return;
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
            read_letter(get_scancode(), &offset);
            //draw_letter(100,800, );
            break;
    }


}

void destroy_sprites() {
    destroy_sprite(mouse);
    destroy_sprite(chooseColors);
}




