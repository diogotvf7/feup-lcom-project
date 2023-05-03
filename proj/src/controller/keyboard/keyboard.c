#include "keyboard.h"

int hook_id = 1;
uint8_t scancode = 0;

int (keyboard_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL)
    return EXIT_FAILURE;
  *bit_no = BIT(hook_id);
  return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int (keyboard_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook_id);
}

int (keyboard_check_errors)(uint8_t st) {
  if (st & PARITY_ERROR) {
    printf("Error: Parity error!\n");
    return EXIT_FAILURE;
  }
  if (st & TIMEOUT_ERROR) {
    printf("Error: Timeout error!\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

int (read_kbc_status)(uint8_t *st) {
  if (util_sys_inb(KBC_STAT_REG, st) != OK) {
    printf("Error: Unable to read status!\n");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

void (kbc_ih)() {
  uint8_t st;
  if (read_kbc_status(&st) != OK) return;
  if (keyboard_check_errors(st) != OK) return;
  if (read_kbc_scancode(st, &scancode) != OK) return;
}

int (read_kbc_scancode)(uint8_t st, uint8_t *scancode) {
  
  if (st & OBF) {      

    if (util_sys_inb(KBC_OUT_BUF, scancode) != OK) {
      printf("Error: Unable to read KBC output buffer!\n");
      return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
  }

  return EXIT_FAILURE;
}

int (kbc_write)(int port, uint8_t cmd) {
  uint8_t st;
  if (read_kbc_status(&st) != OK) return EXIT_FAILURE;
  if (st & IBF) return EXIT_FAILURE;

  if (sys_outb(port, cmd) != OK) return EXIT_FAILURE;
  return EXIT_SUCCESS;
}

int (keyboard_restore)() {
  uint8_t cmd;
  if (kbc_write(KBC_CMD_REG, READ_CMD_BYTE) != OK) return EXIT_FAILURE;
  if (util_sys_inb(KBC_OUT_BUF, &cmd) != OK) return EXIT_FAILURE;

  cmd |= KBC_ENABLE_INT;

  if (kbc_write(KBC_CMD_REG, WRITE_CMD_BYTE) != OK) return EXIT_FAILURE;
  if (kbc_write(KBC_IN_BUF, cmd) != OK) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
