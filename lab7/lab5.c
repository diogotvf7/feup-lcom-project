#include <lcom/lcf.h>
#include <lcom/lab5.h>
#include <stdint.h>
#include <stdio.h>
#include "ser_port.h"
#include "SER.h"
#include "keyboard.h"
#include "i8042.h"


const char keyMap[] = { '\0',
  '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '?', '\0', '\b',
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '+', '\'', '\n',
  '\0', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', '\0', '\0', '\0', '\0', '~',
  'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '-', '\0', '\0', '\0', ' ' 
};

char buffer[21] = "";
int buffer_size = 0;

extern uint8_t scancode;
uint8_t scancode_arr[2];

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/g1/lab7/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g1/lab7/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {

  uint8_t lcr;
  get_uart_lcr(&lcr);
  printf("lcr: ");
  printBits(sizeof(lcr), &lcr);

  printf("DLAB:           %d\n", (lcr & DLAB_MASK) >> 7);
  printf("BREAK:          %d\n", (lcr & BREAK_CONTROL_MASK) >> 6);
  printf("PARITY:         %d\n", (lcr & PARITY_MASK) >> 3);
  printf("STOP:           %d\n", (lcr & STOP_BITS_MASK) >> 2);
  printf("BITS P CHAR:    %d\n", lcr & BITS_PER_CHAR_MASK);
  
  uint8_t ctrl = 0; 
  config_uart();
  get_uart_lcr(&ctrl);

  
  printf("____________________________\n");
  printf("lcr: ");
  printBits(sizeof(ctrl), &ctrl);

  printf("DLAB:           %d\n", (ctrl & DLAB_MASK) >> 7);
  printf("BREAK:          %d\n", (ctrl & BREAK_CONTROL_MASK) >> 6);
  printf("PARITY:         %d\n", (ctrl & PARITY_MASK) >> 3);
  printf("STOP:           %d\n", (ctrl & STOP_BITS_MASK) >> 2);
  printf("BITS P CHAR:    %d\n", ctrl & BITS_PER_CHAR_MASK);
  
  
  int ipc_status, r, flag = 0, num_bytes = 1;
  uint8_t irq_set_kbd, irq_set_uart;
  message msg;

  if (subscribe_uart_int(&irq_set_uart) != OK)
    return EXIT_FAILURE;

  if (keyboard_subscribe_int(&irq_set_kbd) != 0) 
    return EXIT_FAILURE;

  while (scancode != ESC_BREAK) {
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != OK ) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set_kbd) {
            kbc_ih();
            if (scancode == DOUBLE_BYTE) {
              scancode_arr[0] = scancode;
              flag = 1; num_bytes = 2;
            } else {
              scancode_arr[flag] = scancode;
              // if (kbd_print_scancode(!((scancode & SCANCODE_MSB) >> 7), num_bytes, scancode_arr) != OK)
                // return EXIT_FAILURE;
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
              num_bytes = 1; flag = 0;
            } 
          }
          if (msg.m_notify.interrupts & irq_set_uart) {
            uart_ih();
          }
          break;
        default:
          break;
      }
    } else {
    }

  }

  if (keyboard_unsubscribe_int() != 0) return 1;

  if (unsubscribe_uart_int() != OK)
    return EXIT_FAILURE;

  set_uart_lcr(lcr);




  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  return 1;
}

int(video_test_controller)() {
  return 1;
}

