#include "view.h"

extern Sprite* mouse;
extern Sprite* chooseColors;
extern Sprite* quitButton;
extern Sprite* startButton;
extern Sprite* zero;
extern Sprite* one;
extern Sprite* two;
extern Sprite* three;
extern Sprite* four;
extern Sprite* five;
extern Sprite* six;
extern Sprite* seven;
extern Sprite* eight;
extern Sprite* nine;


extern int x, y;
vbe_mode_info_t vmi_p;
extern MenuState menuState;
extern GameState gameState;
extern int game_counter;
int letter_pos = 0;

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
    if (gameState == DRAW) {
        draw_sprite_xpm(chooseColors, 0, 0);
        }
    else if (gameState == GUESS) draw_bar(0,0,1152,150,GREY);
    draw_bottom_bar(0,750,1152,114, GREY,80,780,900,70);
    draw_game_time(game_counter);


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

int draw_letter(int x, int y, int offset){
   // draw_sprite_xpm(letter, x + letter_pos *  75, y);
    letter_pos++;
    return 0;
}

int draw_game_time(int num)
{   
    int arr[2];
    int i = 0;
    int j, r;
  
    while (num != 0) {
  
        r = num % 10;
        
        arr[i] = r;
        i++;

        num = num / 10;
    }
    
    int length = 0;
    for (j = i - 1; j > -1; j--) {
        switch (arr[j])
        {
        case 0:
            draw_sprite_xpm(zero,1000 + length * 75,780);
            break;
        case 1:
            draw_sprite_xpm(one,1000 + length * 75,780);
            break;
        case 2:
            draw_sprite_xpm(two,1000 + length * 75,780);
            break;
        case 3:
            draw_sprite_xpm(three,1000 + length * 75,780);
            break;
        case 4:
            draw_sprite_xpm(four,1000 + length * 75,780);
            break;
        case 5:
            draw_sprite_xpm(five,1000 + length * 75,780);
            break;
        case 6:
            draw_sprite_xpm(six,1000 + length * 75,780);
            break;
        case 7:
            draw_sprite_xpm(seven,1000 + length * 75,780);
            break;
        case 8:
            draw_sprite_xpm(eight,1000 + length * 75,780);
            break;
        case 9:
            draw_sprite_xpm(nine,1000 + length * 75,780);
            break;
        default:
            break;
        }
        length++;

    }
    return 0;
}



