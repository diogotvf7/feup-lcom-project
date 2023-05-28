#include "model.h"

Sprite *mouse;
Sprite *chooseColors;
Sprite* quitButton;
Sprite* startButton;
Sprite* leaderboardTable;
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
struct leaderboardValue leaderboard[5];
extern struct Queue *pos_queue;
int word_guess[10] = {-1};
int number_letters = 0;

/*               DANGER                  */
const char keyMap[] = { '\0',
  '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '?', '\0', '\b',
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '+', '\'', '\n',
  '\0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '\0', '\0', '\0', '\0', '~',
  'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', '\0', '\0', '\0', ' ' 
};
char buffer[21] = "";
int buffer_size = 0;
/*               DANGER                  */

void setup_sprites() {
    chooseColors = create_sprite_xpm((xpm_map_t) topBarGameMode_xpm);
    mouse = create_sprite_xpm((xpm_map_t) mouse_xpm);
    quitButton = create_sprite_xpm((xpm_map_t) quitButton_xpm);
    startButton = create_sprite_xpm((xpm_map_t) startButton_xpm);
    numbers = create_sprite_xpm((xpm_map_t) numbers_xpm);
    letters = create_sprite_xpm((xpm_map_t) letters_xpm);
    leaderboardTable = create_sprite_xpm((xpm_map_t) leaderboardTable_xpm);
}

void initGame(){
    menuState = GAME;
    game_counter = ROUND_TIME;
}

void updateLeaderboard(leaderboardValue *newValue){
    int insertIndex = -1;
    printf("The value passed here is: %d\n", newValue->month);
    for (int i = 0; i < 5; i++) {
        if (newValue->score > leaderboard[i].score) {
            insertIndex = i;
            break;
        }
    }

    if (insertIndex >= 0) {
        for (int i = 4; i > insertIndex; i--) {
            leaderboard[i] = leaderboard[i - 1];
        }
        leaderboard[insertIndex] = *newValue;
    }

}

void update_mouse_state() {
    mouse_ih();
    parse_mouse_packet();
    if (get_byte_index() == 3) {
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
                    }
                    if (y >= 150) {
                        Position *position = (Position *) malloc(sizeof(Position));
                        position->x = x;
                        position->y = y;
                        queue_push(&pos_queue, position);
                    }
                } else {
                    queue_clear(&pos_queue);
                }
                if (get_mouse_packet()->rb) {
                    reset_frame();
                    queue_clear(&pos_queue);
                }
                if(menuState == START){
                }         
                break;
            case END:
                if(get_mouse_packet()->lb){
                    if((x > 0 && x <= 70) && (y > 0 && y <= 70)){
                        clearLeaderboardFile();
                    }
                }
               
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
        // printf("Acutal current month is: %d\n", curr_time.month);
        game_counter--;
        if (game_counter == 0){
            menuState = START;
        }
    }
    // else if(get_counter() % 30 == 0){
    //     rtc_init();
    //     printf("Acutal current month is: %d\n", curr_time.month);
    // }
    // if (queue_size(&pos_queue) > QUEUE_LIMIT)
    //     queue_clear(&pos_queue);

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
        if (!(scancode_arr[flag] & BREAK_BIT)) 
        {
            char c = keyMap[scancode_arr[flag]];
            if (c == '\n') {
                printf("Message sent: %s\n", buffer);
                // uart_send_string(buffer);
                *buffer = '\0';
                buffer_size = 0;
            } else if (c == '\b') {
                if (buffer_size > 0) {
                buffer[buffer_size-1] = '\0';
                buffer_size--;
                }
            } else if (buffer_size < 20) {
                // printf("char: %c\n", c);
                // append_char(buffer, c, &buffer_size);
                send_uart_byte(c);
            }
            // printf("Message: %s\n", buffer);
        }
        num_bytes = 1;
        flag = 0;
    }

    switch(get_scancode()){
        case ZERO_KEY:
            systemState = EXIT;
            break;
        case ONE_KEY:
            if (menuState == START) break;
            game_counter = 0;
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
            {
                if(menuState == END) break;
                leaderboardValue* newValue = malloc(sizeof(leaderboardValue));
                if (newValue == NULL) {
                    printf("Error: Memory allocation failed.\n");
                    break;
                }
                printf("The month i am passing is : %d\n", curr_time.month);


                newValue->month = curr_time.month;

                printf("The value in my newValue is: %d\n", newValue->month);
                newValue->day = curr_time.day;
                newValue->hour = curr_time.hour;
                newValue->minute = curr_time.minute;
                newValue->second = curr_time.second;
                newValue->score = game_counter;
                
                updateLeaderboard(newValue);                
                free(newValue); 

                menuState = END;
                reset_frame();
                break;            
            }
           
        default:
            read_letter(get_scancode(), word_guess, &number_letters);
            break;
    }


}

void update_uart_state() {
    uart_ih();
}

void destroy_sprites() {
    destroy_sprite(mouse);
    destroy_sprite(chooseColors);
}

void loadLeaderboardFromFile(leaderboardValue leaderboard[]) {
    FILE *file = fopen("leaderboard.txt", "r");
    if (file != NULL) {
        fread(leaderboard, sizeof(leaderboardValue), 5, file);
        fclose(file);
    } else {
        for (int i = 0; i < 5; i++) {
            leaderboard[i].month = 0;
            leaderboard[i].day = 0;
            leaderboard[i].hour = 0;
            leaderboard[i].minute = 0;
            leaderboard[i].second = 0;
            leaderboard[i].score = 0;
        }
    }
}

void saveLeaderboardToFile(const leaderboardValue leaderboard[]) {
    FILE *file = fopen("leaderboard.txt", "w");
    if (file != NULL) {
        fwrite(leaderboard, sizeof(leaderboardValue), 5, file);
        fclose(file);
    } else {
        printf("Error: Couldn't open file for writing leaderboard.\n");
    }
}

void clearLeaderboardFile() {
    for (int i = 0; i < 5; i++) {
        leaderboard[i].month = 0;
        leaderboard[i].day = 0;
        leaderboard[i].hour = 0;
        leaderboard[i].minute = 0;
        leaderboard[i].second = 0;
        leaderboard[i].score = 0;
    }
}

