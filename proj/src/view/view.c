#include "view.h"

extern Sprite* mouse;
extern int x, y;
vbe_mode_info_t vmi_p;


void draw_new_frame() {
    draw_mouse();
}

void draw_mouse() {
    //vg_draw_rectangle(0, 0, vmi_p.XResolution, vmi_p.YResolution, 0x00000);
    draw_sprite_xpm(mouse, x, y);


}

void draw_initial_menu() {

}

void draw_game_menu() {

}

void draw_finish_menu() {
    
}

int draw_sprite_xpm(Sprite *sprite, int x, int y) {
    uint16_t width = sprite->width;
    uint16_t height = sprite->height;
    uint32_t current_color;
    if(get_mouse_packet()->lb) {
        vg_draw_pixel(x,y,RED);
    }
    for(uint16_t h = 0; h < height; h++) {
        for(uint16_t w = 0; w < width; w++) {
            current_color = sprite->colors[w + h*width];
            if(current_color != TRANSPARENT) {
                if(vg_draw_pixel(x + w, y + h, current_color) != 0) return 1;
            }
        }
    }
    return 0;

}


