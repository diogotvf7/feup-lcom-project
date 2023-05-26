#include "view.h"

extern Sprite* mouse;
extern Sprite* chooseColors;
extern Sprite* quitButton;
extern Sprite* startButton;
extern Sprite* numbers;
extern Sprite* letters;
extern uint16_t bytes_per_pixel;
extern Sprite* leaderboardTable;
extern struct leaderboardValue leaderboard[5];

extern int x, y;
vbe_mode_info_t vmi_p;
extern MenuState menuState;
extern GameState gameState;
extern int game_counter;

extern int number_letters;
extern int word_guess[10];

extern int word_solution[12];
extern int word_sol_number_letters;

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
    draw_bottom_bar(0,750,1152,114, GREY,80,780,900,70);

    if (gameState == DRAW) {
        draw_sprite_xpm(chooseColors, 0, 0);
        draw_word_sol();
    }

    else if (gameState == GUESS){ 
        draw_bar(0,0,1152,150,GREY);
        draw_word();    
    }
    draw_game_time(game_counter);

}

void draw_finish_menu() {
    draw_sprite_xpm(leaderboardTable, 0 , 0);
    int y_gap = 0;

    for (int i = 0; i < 5; i++) {
        int x_pos = 20;
        int y_pos = 270 + y_gap;



        //draw month
        if(leaderboard[i].month < 9){
            draw_number(numbers, x_pos, y_pos, leaderboard[i].month % 10);
            x_pos += 140;
        } 
        else{
            draw_number(numbers, x_pos, y_pos, leaderboard[i].month / 10);
            x_pos += 70;
            draw_number(numbers, x_pos, y_pos, leaderboard[i].month % 10);  
            x_pos += 70;
        }

        //draw day
        if(leaderboard[i].day < 9){
            draw_number(numbers, x_pos, y_pos, leaderboard[i].day % 10);  
            x_pos += 190;
        }else{
            draw_number(numbers, x_pos, y_pos, leaderboard[i].day / 10);  
            x_pos += 70;
            draw_number(numbers, x_pos, y_pos, leaderboard[i].day % 10);  
            x_pos += 120;
        }

        //draw hour
        if(leaderboard[i].hour < 9){
            x_pos += 70;
            draw_number(numbers, x_pos, y_pos, leaderboard[i].hour % 10);  
            x_pos += 70;
        }else{
            draw_number(numbers, x_pos, y_pos, leaderboard[i].hour / 10);  
            x_pos += 70;
            draw_number(numbers, x_pos, y_pos, leaderboard[i].hour % 10);  
            x_pos += 70;
        }

        //draw minute
        if(leaderboard[i].minute < 9){
            x_pos += 70;
            draw_number(numbers, x_pos, y_pos, leaderboard[i].minute % 10);  
            x_pos += 70;
        }else{
            draw_number(numbers, x_pos, y_pos, leaderboard[i].minute / 10); 
            x_pos += 70;
            draw_number(numbers, x_pos, y_pos, leaderboard[i].minute % 10);  
            x_pos += 70;
        }

        //draw second
        if(leaderboard[i].second < 9){
            x_pos += 70;
            draw_number(numbers, x_pos, y_pos, leaderboard[i].second % 10);  
            x_pos += 200;
        }else{
            draw_number(numbers, x_pos, y_pos, leaderboard[i].second / 10);  
            x_pos += 70;
            draw_number(numbers, x_pos, y_pos, leaderboard[i].second % 10);  
            x_pos += 200;
        }

        //draw score
        if(leaderboard[i].score < 9){
            draw_number(numbers, x_pos, y_pos, leaderboard[i].score % 10);  
        }else{
            draw_number(numbers, x_pos, y_pos, leaderboard[i].score / 10);  
            x_pos += 70;
            draw_number(numbers, x_pos, y_pos, leaderboard[i].score % 10);  
        }

        y_gap += 125;  
    }
}


int draw_sprite_xpm(Sprite *sprite, int x, int y) {
    uint16_t width;
    uint16_t height;
    width = sprite->width;
    height = sprite->height;
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

int draw_word(){
    int letter_pos = 0;
    for(int i = 0; i < number_letters; i++){
        draw_letter(80 + letter_pos * 60, 780, word_guess[i]);
        letter_pos++;
    }
    return 0;
}

int draw_word_sol(){
    int letter_pos = 0;
    for(int i = 0; i < word_sol_number_letters; i++){
        draw_letter(80 + letter_pos * 60, 780, word_solution[i]);
        letter_pos++;
    }
    return 0;
}


int draw_letter(int x, int y, int letter_index){

    uint16_t width = 70;
    uint16_t height = 70;
    uint16_t img_width = letters->width;

    uint32_t current_color;
    for(uint16_t h = 0; h < height; h++) {
        for(uint16_t w = 0; w < width; w++) {
            current_color = letters->colors[(70 * letter_index  + w) + (img_width * h)];
            if(current_color != TRANSPARENT) {
                vg_draw_pixel(x + w, y + h, current_color);
            }
        }
    }
    return 0;

}


int draw_number(Sprite* sprite, int x, int y, int index){
    uint16_t width = 70;
    uint16_t height = 70;
    uint16_t img_width = sprite->width;

    uint32_t current_color;
    for(uint16_t h = 0; h < height; h++) {
        for(uint16_t w = 0; w < width; w++) {
            current_color = sprite->colors[(70 * index  + w) + (img_width * h)];
            if(current_color != TRANSPARENT) {
                vg_draw_pixel(x + w, y + h, current_color);
            }
        }
    }
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
        draw_number(numbers, 1000 + length * 60, 780, arr[j]);
        length++;
    }
    return 0;
}



