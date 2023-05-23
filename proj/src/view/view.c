#include "view.h"

extern Sprite* mouse;
extern Sprite* chooseColors;
extern Sprite* quitButton;
extern Sprite* startButton;
extern Sprite* zero;


extern int x, y;
vbe_mode_info_t vmi_p;
extern MenuState menuState;
extern GameState gameState;

void draw_new_frame() {
    switch(menuState){
        case START:
            draw_initial_menu();
            break;
        case GAME:
            draw_game_menu();
            break;
        case END:
            draw_finish_menu();
            break;    

    }
    draw_mouse();
}

void draw_mouse() {
    draw_sprite_xpm(mouse, x, y);
}

void draw_initial_menu() {
    draw_sprite_xpm(startButton, 451, 300);
    draw_sprite_xpm(quitButton, 451, 500);
    
}

void draw_game_menu() {
<<<<<<< HEAD
    draw_sprite_xpm(chooseColors, 0, 0);

=======
    if (gameState == DRAW) {
        draw_sprite_xpm(chooseColors, 0, 0);
        draw_bottom_bar(0,750,1152,114, GREY,80,780,900,70);
        }
    else if (gameState == GUESS) draw_bar(0,0,1152,150,GREY);

  //  draw_mouse();
>>>>>>> 574b0aba08819c74c4a910da2c826c86fdf15897
}

void draw_finish_menu() {
    
}

int draw_sprite_xpm(Sprite *sprite, int x, int y) {
    uint16_t width = sprite->width;
    uint16_t height = sprite->height;
    uint32_t current_color;
    for(uint16_t h = 0; h < height; h++) {
        for(uint16_t w = 0; w < width; w++) {
            current_color = sprite->colors[w + h*width];
            if(current_color != TRANSPARENT) {
                vg_draw_pixel(x + w, y + h, current_color);
            }
        }
    }
    return 0;
}

int draw_bar(int x, int y, int width, int height, uint32_t color){
    for (uint16_t h = 0; h < height; h++){
        for (uint16_t w = 0; w < width; w++){
            vg_draw_pixel(x + w, y + h, color);
        }
    }
    return 0;
}

int draw_bottom_bar(int x, int y, int width, int height, uint32_t color, int square_vertex_x , int square_vertex_y , int square_width , int square_height){
    
    for (uint16_t h = 0; h < height; h++){
        for (uint16_t w = 0; w < width; w++){
            // (x+w),(y+h) -> position to draw 
            if ( (x + w >= square_vertex_x && x + w <= square_vertex_x + square_width) && (y + h >= square_vertex_y && y + h <=  square_vertex_y + square_height)) vg_draw_pixel(x + w, y + h, WHITE);
            else {
                vg_draw_pixel(x + w, y + h, color);
            }
        }
    }
    return 0;
}

void extractDigits(unsigned int num)
{
    int dig;
    while(num>0)
    {
        dig = num%10;
        // draw xpm according to its number
        num = num/10;
    }
}



