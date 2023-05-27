#include "keyboard.h"

static int hook_id = 1;
uint8_t scancode = 0;

uint8_t (get_scancode)() {
  return scancode;
}

int (keyboard_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return !OK;
  *bit_no = BIT(hook_id);
  return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int (keyboard_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

int (keyboard_check_errors)(uint8_t st) {
  if (st & PARITY_ERROR) {
    printf("Error: Parity error!\n");
    return !OK;
  }
  if (st & TIMEOUT_ERROR) {
    printf("Error: Timeout error!\n");
    return !OK;
  }
  return OK;
}

int (read_keyboard_status)(uint8_t *st) {
  if (util_sys_inb(KBC_STAT_REG, st) != OK) {
    printf("Error: Unable to read status!\n");
    return !OK;
  }
  return OK;
}

void (kbc_ih)() {
  uint8_t st;
  if (read_keyboard_status(&st) != OK) return;
  if (keyboard_check_errors(st) != OK) return;
  if (read_kbc_scancode(st, &scancode) != OK) return;
}

int (read_kbc_scancode)(uint8_t st, uint8_t *scancode) {
  
  if (st & OBF) {      

    if (util_sys_inb(KBC_OUT_BUF, scancode) != OK) {
      printf("Error: Unable to read KBC output buffer!\n");
      return !OK;
    }
    return OK;
  }

  return !OK;
}

int (keyboard_write)(int port, uint8_t cmd) {
  uint8_t st;
  if (read_keyboard_status(&st) != OK) return !OK;
  if (st & IBF) return !OK;

  if (sys_outb(port, cmd) != OK) return !OK;
  return OK;
}

int (keyboard_restore)() {
  uint8_t cmd;
  if (keyboard_write(KBC_CMD_REG, READ_CMD_BYTE) != OK) return !OK;
  if (util_sys_inb(KBC_OUT_BUF, &cmd) != OK) return !OK;

  cmd |= KBC_ENABLE_INT;

  if (keyboard_write(KBC_CMD_REG, WRITE_CMD_BYTE) != OK) return !OK;
  if (keyboard_write(KBC_IN_BUF, cmd) != OK) return !OK;

  return OK;
}

int read_letter(int scancode, int* word_guess, int* number_letters) {
  if (scancode == A_KEY) {
    word_guess[*number_letters] = 0;
    (*number_letters)++;
  }
  else if (scancode == B_KEY) {
    word_guess[*number_letters] = 1;
    (*number_letters)++;
  }else if(scancode == BACKSPACE_KEY){
    (*number_letters)--;
    word_guess[*number_letters] = -1;
  }else if (scancode == SPACE_KEY) {
    for (int i = 0; i < *number_letters; i++) {
        word_guess[i] = -1;
    }
    *number_letters = 0;
  }

  return 0;
}

