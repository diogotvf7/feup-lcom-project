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
int to_qwerty[26] = {10,23,21,12,2,13,14,15,7,16,17,18,25,24,8,9,0,3,11,4,6,22,1,20,5,19};
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
extern struct Queue *garbage;

int word_guess[12] = {-1};
int number_letters = 0;

int word_solution[12] = {-1};
int word_sol_number_letters;

void setup_sprites() {
    chooseColors = create_sprite_xpm((xpm_map_t) topBarGameMode_xpm);
    mouse = create_sprite_xpm((xpm_map_t) mouse_xpm);
    quitButton = create_sprite_xpm((xpm_map_t) quitButton_xpm);
    startButton = create_sprite_xpm((xpm_map_t) startButton_xpm);
    numbers = create_sprite_xpm((xpm_map_t) numbers_xpm);
    letters = create_sprite_xpm((xpm_map_t) font_xpm);
    leaderboardTable = create_sprite_xpm((xpm_map_t) leaderboardTable_xpm);
}

void initGame(){
    menuState = GAME;
    game_counter = ROUND_TIME;
    getRandomWord();
}

void updateLeaderboard(leaderboardValue *newValue){
    int insertIndex = -1;
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
            if (gameState == DRAW){
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
                        queue_push(&pos_queue, position, sizeof(Position));
                    }
                } else {
                    queue_clear(&pos_queue);
                }
                if (get_mouse_packet()->rb) {
                    reset_frame();
                    queue_clear(&pos_queue);
                }
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
        game_counter--;
        if (game_counter == 0){
            menuState = START;
            reset_frame();
        }
    }else if(get_counter() % 30 == 0){
        rtc_init();
       // getRandomWord();
    }
    //printf("Queue size:     %d\n", queue_size(&pos_queue));
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


                newValue->month = curr_time.month;

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
        if (gameState == GUESS) read_letter(get_scancode(), word_guess, &number_letters);
            break;
    }
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

char* getRandomWord() {
    FILE* file = fopen("/home/lcom/labs/proj/src/model/test.txt", "r");
    if (file == NULL) {
        printf("Error opening file: \n" );
        return NULL;
    }
    
    int random_line_number = (rand() % 49);    
    char* line = malloc(MAX_WORD_LENGTH * sizeof(char));
    if (line == NULL) {
        printf("Error allocating memory: \n");
        fclose(file);
        return NULL;
    }
    for (int i = 0; i < random_line_number; i++) {
        if (fgets(line, MAX_WORD_LENGTH, file) == NULL) {
            printf("Error reading file: \n");
            fclose(file);
            free(line); 
            return NULL;
        }
    }
    word_sol_number_letters = strlen(line) - 1;

      printf("Line number: %d \n Word associated: %s\n Size of word: %d\n\n", random_line_number, line, word_sol_number_letters);

    for (uint8_t i = 0; i < strlen(line); i++) {
        int index = to_qwerty[(*(line + i)) - 'a'];
        word_solution[i] = index;
        printf("%d - ", word_solution[i]);
    }


    fclose(file); 
    return line;
}
