#include "mouse.h"

static int hook_id = 12;
static uint8_t mouse_byte;
static int byte_index = 0;
static struct packet mouse_packet;

int (get_byte_index)() {
  return byte_index;
}

struct packet *(get_mouse_packet)() {
  return &mouse_packet;
}

int (mouse_subscribe_int)(int *bit_no) 
{
  if (bit_no == NULL) return EXIT_FAILURE;
  *bit_no = BIT(hook_id);
  return sys_irqsetpolicy(MOUSE_KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int (mouse_unsubscribe_int)() 
{
  return sys_irqrmpolicy(&hook_id);
}

int (set_data_reporting)(bool enable) 
{
  uint8_t out;
  int attempts_left = ATTEMPTS;
  while (attempts_left-- && out != ACK) {
    if (mouse_write(KBC_CMD_REG, WRITE_BYTE_MOUSE) != OK) return EXIT_FAILURE;
    if (mouse_write(KBC_IN_BUF, enable ? ENABLE_DATA_REP : DISABLE_DATA_REP) != OK) return EXIT_FAILURE;
    tickdelay(micros_to_ticks(DELAY_US));
    if (util_sys_inb(KBC_OUT_BUF, &out) != OK) return EXIT_FAILURE;
    if (out != ACK) continue;
  }
  return EXIT_SUCCESS;
}

void (mouse_ih)() 
{ 
  uint8_t byte;
  if (read_output_buffer(&byte) != OK)
    printf("Error: Couldn't read byte from mouse!\n");
  mouse_byte = byte;
}

void (parse_mouse_packet)() {
  byte_index %= 3;
  switch (byte_index) {
    case 0:
      if (!(mouse_byte & FIRST_BYTE)) return;
      mouse_packet.bytes[0] = mouse_byte;
      mouse_packet.y_ov = mouse_byte & Y_OVFL;
      mouse_packet.x_ov = mouse_byte & X_OVFL;
      mouse_packet.mb = mouse_byte & MOUSE_MB;
      mouse_packet.rb = mouse_byte & MOUSE_RB;
      mouse_packet.lb = mouse_byte & MOUSE_LB;
      break;
    case 1:
      mouse_packet.bytes[1] = mouse_byte;
      mouse_packet.delta_x = mouse_packet.bytes[0] & MSB_X_DELTA ? 0xFF00 | mouse_byte : mouse_byte;
      break;
    case 2:
      mouse_packet.bytes[2] = mouse_byte;
      mouse_packet.delta_y = mouse_packet.bytes[0] & MSB_Y_DELTA ? 0xFF00 | mouse_byte : mouse_byte;
      break;
  }
  byte_index++;
}
