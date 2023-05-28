#include "model.h"

Sprite *mouse;
Sprite *chooseColors;
Sprite* quitButton;
Sprite* startButton;
Sprite* leaderboardTable;
Sprite* numbers;
Sprite* letters;
Sprite* playAgainButton;
Sprite* leaderboardButton;
Sprite* victory;
Sprite* defeat;
Sprite* initialMenuButton;
Sprite* ldbdButtonInitialPage;
Sprite* coopGuessButton;
Sprite* coopDrawButton;


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
int delayTime = 0;
bool gameResult= false;

int word_guess[12] = {-1};
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

// extern Queue *xmit_fifo;
extern Queue *rcvr_fifo;

/*               DANGER                  */

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
    playAgainButton = create_sprite_xpm((xpm_map_t) Play_Again_xpm);
    initialMenuButton = create_sprite_xpm((xpm_map_t) Quit_xpm);
    leaderboardButton = create_sprite_xpm((xpm_map_t) Leaderboard_xpm);
    victory = create_sprite_xpm((xpm_map_t) Victory_xpm);
    defeat = create_sprite_xpm((xpm_map_t) Defeat_xpm);
    ldbdButtonInitialPage = create_sprite_xpm((xpm_map_t) leaderboardButtonInitialPage_xpm);
    coopGuessButton = create_sprite_xpm((xpm_map_t) coopGuessButton_xpm);
    coopDrawButton = create_sprite_xpm((xpm_map_t) coopDrawButton_xpm);

}

void initGame() {
    menuState = GAME;
    game_counter = ROUND_TIME;
    getRandomWord();
    delayTime = 0;
    gameResult = false;
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
                    updateStateMouseClick();
                }
                break;
            case GAME:
                if (gameState == DRAW || gameState == DRAW_GUESS){
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
                //isto e preciso ?
                if(menuState == START){
                }         
                break;
            case LEADERBOARD:
                if(get_mouse_packet()->lb){
                    if((x > 0 && x <= 70) && (y > 0 && y <= 70)){
                        clearLeaderboardFile();
                    }
                }
                break;
            case END:
                if(get_mouse_packet()->lb){
                    if(( x >= 131 && x <= 331) && (y >= 481 && y <= 681)) {menuState = GAME; initGame();}
                    else if(( x >= 462 && x <= 662) && (y >= 481 && y <= 681)) {menuState = LEADERBOARD;}
                    else if(( x >= 793 && x <= 993) && (y >= 481 && y <= 681)) {menuState = START;}
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
    
    if (menuState == START) {
        if (!queue_empty(&rcvr_fifo)) {
            printf("Starting as guesser!\n");
            uint8_t *byte = queue_front(&rcvr_fifo);
            printf("Byte: %d      START_GAME: %d\n", *byte, START_GAME);
            if (*byte == START_GAME) {
                gameState = GUESS;
                initGame();
            }
            queue_pop(&rcvr_fifo);
        }
    }
    copy_base_frame(frame_buffer);
    for (int i = PACKETS_PER_INTERRUPT; i; i--) {
        if (process_packet(color, radius) != 0)
            break;
    }
    if (get_counter() % 30 == 0 && menuState == GAME){


        //tempo de delay onde so conseguimos ver a palavra - o game counter nao diminui aqui
        if(gameState == DRAW_GUESS && delayTime < 6){delayTime++;}
        else{
            game_counter--;
        }

        //Acabou os 5 segundos para ver a imagem, podemos começar a escrever - limpamos tudo aquilo que o user possa ter carregado para o word_guess
        if(delayTime == 5){
            for(int i = 0; i < 12 ; i++){word_guess[i] = -1;}
            number_letters = 0;
        }

        //jogo acabou
        if (game_counter == 0){
            menuState = END;
            reset_frame();
        }
    }
    //Podemos nao estar no modo de jogo mas o rtc tem sempre de se atualizar
    // else if(get_counter() % 30 == 0){
    //     rtc_init();
    // }


    //printf("Queue size:     %d\n", queue_size(&pos_queue));
    // if (queue_size(&pos_queue) > QUEUE_LIMIT)
    //     queue_clear(&pos_queue);
    
    // printf("RCVR SIZE: %d\n", queue_size(&rcvr_fifo));
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

    updateStateKeyboardClick();
}

void update_uart_state() {
    uart_ih();
}

void destroy_sprites() {
    destroy_sprite(mouse);
    destroy_sprite(chooseColors);
    destroy_sprite(quitButton);
    destroy_sprite(startButton);
    destroy_sprite(numbers);
    destroy_sprite(letters);
    destroy_sprite(leaderboardTable);
    destroy_sprite(playAgainButton);
    destroy_sprite(initialMenuButton);
    destroy_sprite(leaderboardButton);
    destroy_sprite(victory);
    destroy_sprite(defeat);
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
    FILE* file = fopen("/home/lcom/labs/g1/proj/src/model/test.txt", "r");
    
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
    printf("Word: %s\n", line);
    printf("\n");

    for (uint8_t i = 0; i < strlen(line); i++) {
        if (*(line + i) == '\n' || *(line + i) == '\r') {
            break;
        }
        printf("%c", *(line + i));
        printf("\n");
        int index = to_qwerty[(*(line + i)) - 'a'];
        printf("index: %d\n", index);
        word_solution[i] = index;
    }

    fclose(file); 
    return line;
}

bool checkResult(){

    if(number_letters == word_sol_number_letters){
        for(int i = 0; i < number_letters; i++){
            if(word_guess[i] != word_solution[i])
                return false;
        }
        return true;
    }
    return false;
}

void addValueToLeaderboard(){
    leaderboardValue* newValue = malloc(sizeof(leaderboardValue));
    if (newValue == NULL) {
        printf("Error: Memory allocation failed.\n");
    }
    newValue->month = curr_time.month;
    newValue->day = curr_time.day;
    newValue->hour = curr_time.hour;
    newValue->minute = curr_time.minute;
    newValue->second = curr_time.second;
    newValue->score = game_counter;
    
    updateLeaderboard(newValue);                
    free(newValue);
}

void updateStateMouseClick() {
    //single player
    if(x >= 100 && x <= 370 && y >= 300 && y <= 425){
        initGame();
        gameState = DRAW_GUESS;
    }
    //coop draw
    else if(x >= 450 && x <= 710 && y >= 300 && y <= 425){
        //send_start_msg();
        send_uart_byte(START_GAME);
        initGame();
        gameState = DRAW;
    }
    else if(x >= 800 && x <= 1060 && y >= 300 && y <= 425){
        initGame();
        gameState = GUESS;
    }
    else if(x >= 250 && x <= 510 && y >= 500 && y <= 625){
        menuState = LEADERBOARD;
    }
    else if(x >= 600 && x <= 860 && y >= 500 && y <= 625){
        systemState = EXIT;
    }
}

void updateStateKeyboardClick(){
        switch(get_scancode()){
        case ZERO_KEY:
            systemState = EXIT;
            break;

        //Ecra Inicial
        case ONE_KEY:
            if (menuState == START) break;
            menuState = START;
            reset_frame();
            break;

        //Modo de jogo multiplayer - desenhar
        case TWO_KEY:
            if (menuState == GAME) break;
            initGame();
            gameState = DRAW;
            break;

        //Modo de jogo multiplayer - adivinhar
        case THREE_KEY:
            if (menuState == GAME) break;
            initGame();
            gameState = GUESS;
            break;

        //Ver a leaderboard
        case FOUR_KEY:{
            if(menuState == LEADERBOARD) break;
        
            menuState = LEADERBOARD;
            reset_frame();
            break;               
        }

        //Modo de jogo singleplayer
        case FIVE_KEY:{
            if(menuState == GAME) break;
            gameState = DRAW_GUESS;
            game_counter = 0;
            delayTime = 0;
            initGame();
            reset_frame();
            break;
        }

        //Ecra que aparece depois de acabar um jogo
        case SIX_KEY:{
            if(menuState != GAME) break;
            menuState = END;
            reset_frame();
            break;
        }

        //User verifica se a sua repsosta esta correcta
        case ENTER:{
            gameResult = checkResult();
            if(gameResult) {
                menuState = END;
                addValueToLeaderboard();
            };
            break;
        }

        //User escreveu uma letra
        default:
        if (gameState == GUESS || gameState == DRAW_GUESS) read_letter(get_scancode(), word_guess, &number_letters);
            break;
    }
}

