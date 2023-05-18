#include "model.h"

Sprite *mouse;
int flag = 0, num_bytes = 1;
uint8_t scancode_arr[2];
extern int x, y;
extern frame_buffer_t frame_buffer;

void setup_sprites() {
    
    mouse = create_sprite_xpm((xpm_map_t) mouse_xpm);
}

void update_mouse_state() {
    mouse_ih();
    parse_mouse_packet();
    if (get_byte_index() == 3) {
        //nao se pode meter a desenhar a linha depois de desenhar a nova posiçao do cursor porque vai desenhar por cima
        if (get_mouse_packet()->lb) {
            draw_frame_pixel(x, y, RED);
        }
        if (get_mouse_packet()->rb) {
            //fill_frame(TRANSPARENT);
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
}

void destroy_sprites() {
    destroy_sprite(mouse);
}




