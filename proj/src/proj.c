#include "controller/graphics_card/VBE.h"
#include "controller/graphics_card/graphics_card.h"
#include "controller/keyboard/i8042.h"
#include "controller/timer/i8254.h"
#include "controller/timer/timer.h"
#include "controller/keyboard/keyboard.h"
#include "controller/mouse/mouse.h"
#include <lcom/lcf.h>
#include <lcom/video_gr.h>

int(main)(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/g1/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g1/proj/src/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(proj_main_loop)(int argc, char **argv) {
  if (vg_init(VBE_MODE) == NULL)
    return EXIT_FAILURE;
  int ipc_status, r, flag = 0, num_bytes = 1;
  uint8_t irq_set_timer, irq_set_keyboard, scancode_arr[2];
  int irq_set_mouse;
  message msg;

  if (set_data_reporting(TRUE) != OK) 
    return EXIT_FAILURE;
  if (keyboard_subscribe_int(&irq_set_keyboard) != OK)
    return EXIT_FAILURE;
  if (timer_subscribe_int(&irq_set_timer) != OK)
    return EXIT_FAILURE;
  if (mouse_subscribe_int(&irq_set_mouse) != OK)
    return EXIT_FAILURE;
  if (timer_set_frequency(SEL_TIMER0, 10) != OK)
    return EXIT_FAILURE;

  while (get_scancode() != ESC_BREAK) {
    if ((r = driver_receive(ANY, &msg, &ipc_status)) != OK) {
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
      case HARDWARE:
        if (msg.m_notify.interrupts & irq_set_mouse) {
          mouse_ih();
          parse_mouse_packet();
          if (get_byte_index() == 3)
            mouse_print_packet(get_mouse_packet());
        }
        if (msg.m_notify.interrupts & irq_set_timer) {
          timer_int_handler();
          if (get_counter() % 60 == 0) {
            vg_flip_frame();
          }
        } if (msg.m_notify.interrupts & irq_set_keyboard) {
          kbc_ih();
          if (get_scancode() == DOUBLE_BYTE) {
            scancode_arr[0] = get_scancode();
            flag = 1;
            num_bytes = 2;
          } else {
            scancode_arr[flag] = get_scancode();
            if (kbd_print_scancode(!((get_scancode() & SCANCODE_MSB) >> 7),
                                    num_bytes, scancode_arr) != OK)
              return EXIT_FAILURE;
            num_bytes = 1;
            flag = 0;
          }
        }
          break;
        default:
          break;
      }
    } else {
    }
  }

  if (keyboard_unsubscribe_int(&irq_set_keyboard) != OK)
    return EXIT_FAILURE;
  if (timer_unsubscribe_int() != OK)
    return EXIT_FAILURE;
  if (mouse_unsubscribe_int(&irq_set_mouse) != OK)
    return EXIT_FAILURE;  
  if (set_data_reporting(FALSE) != OK) 
    return EXIT_FAILURE;
  if (vg_exit() != EXIT_SUCCESS)
    return EXIT_FAILURE;
  return EXIT_SUCCESS;
}
