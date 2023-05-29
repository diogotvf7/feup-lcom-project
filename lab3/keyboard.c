#include <lcom/lcf.h>
#include "keyboard.h"
#include "keyboard-controller.h"
#include "i8042.h"

static int hook_id = 1;
uint8_t scancode = 0;

void (kbc_ih)(){
  if (read_KBC_output(KBC_OUT_CMD,&scancode) != 0) printf("Error: Could not read scancode\n");
}

int (kbd_subscribe_int)(uint8_t *bit_no){
  if (bit_no == NULL) return 1;
  *bit_no = BIT(hook_id);
  if (sys_irqsetpolicy(KEYBOARD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != 0) return 1;
  return 0;
}

int (kbd_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id) != 0) return 1;
  return 0;
}

void (kbd_int_handler)() {
  if (read_KBC_output(KBC_OUT_CMD, &scancode) != 0) printf("Error: Could not read scancode!\n");
}

int (keyboard_restore) () {
  uint8_t commandByte;

  // leitura da atual config
  if (write_KBC_command(KBC_IN_CMD,KBC_READ_CMD) != 0) return 1; // avisar i8042 da leitura
  if (read_KBC_output(KBC_OUT_CMD,&commandByte)!= 0) return 1; // ler config 

  commandByte |= BIT(0); // avisar bit das interrupts

  // escrever nova config
  if (write_KBC_command(KBC_IN_CMD,KBC_WRITE_CMD) != 0) return 1; // avisar i8042 da escrita
  if (write_KBC_command(KBC_WRITE_CMD,commandByte) != 0) return 1; // escrever config

  return 0;
}
