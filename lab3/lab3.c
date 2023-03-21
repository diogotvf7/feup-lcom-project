#include <lcom/lcf.h>
// #include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>
#include "i8254.h"
#include "i8042.h"
#include "keyboard-controller.h"
#include "keyboard.h"

extern int counter_TIMER;
extern uint32_t counter_KBC;
extern uint8_t scancode;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int(kbd_test_scan)() {
  int ipc_status;
  uint8_t irq_set;
  message msg;

  if (kbd_subscribe_int(&irq_set) != 0) return 1;

  while (scancode != ESC_BREAK_CODE) { // pára quando obter um scancode da tecla ESC 

    if (driver_receive(ANY, &msg, &ipc_status) != 0){
      printf("Error");
      continue;
    }

    if(is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set) {
              kbc_ih(); // aumenta o contador interno
              kbd_print_scancode(!(scancode & MAKE_CODE), scancode == TWO_BYTES ? 2 : 1, &scancode);
          }
      }
    }
  }

  if (kbd_unsubscribe_int() != 0) return 1;
  if (kbd_print_no_sysinb(counter_KBC) != 0) return 1;

  return 0;


}

int(kbd_test_poll)() {
  
  while (scancode != ESC_BREAK_CODE){
    if (read_KBC_output(KBC_OUT_CMD,&scancode) == 0){
      kbd_print_scancode(!(scancode & MAKE_CODE),scancode == TWO_BYTES ? 2 : 1,&scancode);
    }
  }

  if (keyboard_restore() == 0) return 1;
  return 0;

}

int(kbd_test_timed_scan)(uint8_t n) {

  int ipc_status;
  uint8_t irq_set_TIMER, irq_set_KBC;
  message msg;

  int seconds = 0;  // timer seconds

  if (timer_subscribe_int(&irq_set_TIMER) != 0) return 1;
  if (kbd_subscribe_int(&irq_set_KBC) != 0) return 1;

  while (scancode != ESC_BREAK_CODE && seconds < n){

    if(driver_receive(ANY, &msg, &ipc_status) != 0 ){
      printf("Error");
      continue;
    }

    if(is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set_KBC) {
            kbc_ih();
            kbd_print_scancode(!(scancode & MAKE_CODE), scancode == TWO_BYTES ? 2 : 1, &scancode);
            seconds = 0;
            counter_TIMER = 0;
          }
          if (msg.m_notify.interrupts & irq_set_TIMER) {
            timer_int_handler();
            if (counter_TIMER % 60 == 0) seconds++;
          }
      }
    }
  }

  if (timer_unsubscribe_int() != 0) return 1;
  if (kbd_unsubscribe_int() != 0) return 1;
  if (kbd_print_no_sysinb(counter_KBC) != 0) return 1;
  
  return 0;
}
