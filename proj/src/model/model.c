#include "model.h"

Sprite *mouse;
Sprite *chooseColors;
int flag = 0, num_bytes = 1;
uint8_t scancode_arr[2];
extern int x, y;
extern frame_buffer_t frame_buffer;
extern real_time curr_time;
uint32_t color = RED;
int radius = 10;
SystemState systemState = RUNNING;
MenuState menuState = START; 

extern struct Queue *pos_queue;
extern struct Queue *garbage;

void setup_sprites() {
    chooseColors = create_sprite_xpm((xpm_map_t) topBarGameMode_xpm);
    mouse = create_sprite_xpm((xpm_map_t) mouse_xpm);
}

void update_mouse_state() {
    mouse_ih();
    parse_mouse_packet();
    if (get_byte_index() == 3) {
        //nao se pode meter a desenhar a linha depois de desenhar a nova posiçao do cursor porque vai desenhar por cima
        if (get_mouse_packet()->lb) {
            if (y < 150) {
                updateDrawSpecs(&color, &radius);
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
        updateMouseLocation();
    }
}

void update_timer_state() {
    timer_int_handler();
    vg_flip_frame();
    copy_base_frame(frame_buffer);

    if (get_counter() % 1 == 0) {
        for (int i = PACKETS_PER_INTERRUPT; i; i--) {
            // queue_print(&pos_queue);
            int size = queue_size(&pos_queue);
            if (size == 0) break;
            struct Position *position1 = queue_front(&pos_queue);

            if (size == 1) {
                draw_frame_circle(position1, radius, color);
                break;
            } else {
                queue_pop(&pos_queue);
                struct Position *position2 = queue_front(&pos_queue);
                draw_bresenham_line(position1, position2, color, radius);
            }
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
        case Q_KEY:
            systemState = EXIT;
            break;
        case S_KEY:
            menuState = START;
            break;
        case G_KEY:
            menuState = GAME;
            break;
        case E_KEY:
            menuState = END;
        default:
            break;
    }



}

void destroy_sprites() {
    destroy_sprite(mouse);
    destroy_sprite(chooseColors);
}




