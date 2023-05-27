#include "ser_port.h"

int hook_id = 4;
static uint8_t iir;
static Queue *xmit_fifo = NULL;
static Queue *rcvr_fifo = NULL;

int config_uart() 
{
  uint8_t lcr = 0, ier = 0, fcr = 0;

  lcr = BITS_PER_CHAR(8) | STOP_BITS_2 | ODD_PARITY;
  ier = ENABLE_RECEIVED_DATA_INT | ENABLE_TRANSMITTER_EMPTY_INT | ENABLE_RECEIVER_LINE_INT;
  fcr = ENABLE_FIFO | CLEAR_RCVR_FIFO | CLEAR_XMIT_FIFO | RCVR_TRIGGER_8;

  if (set_uart_lcr(lcr) != OK) return !OK;      // SEND LCR CONFIG

  if (set_uart_dlab(1) != OK) return !OK;       //
  if (set_uart_freq(BITRATE_115200) != OK) return !OK;    // SEND BIT RATE FREQUENCY
  if (set_uart_dlab(0) != OK) return !OK;       //

  if (set_uart_ier(ier) != OK) return !OK;      // SEND IER CONFIG
  
  if (set_uart_fcr(fcr) != OK) return !OK;  // SEND FCR CONFIG

  return OK;
}

int set_uart_dlab(uint8_t status) {
  uint8_t lcr = 0;
  if (get_uart_lcr(&lcr) != OK) return 1;
  if (status) lcr |= DLAB;
  else lcr &= ~DLAB;
  return set_uart_lcr(lcr);
}

int subscribe_uart_int(uint8_t *bit_no) 
{
  if (bit_no == NULL) return 1;
  *bit_no = BIT(hook_id);
  return sys_irqsetpolicy(COM1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id);
}

int set_uart_fcr(uint8_t byte) 
{
  return sys_outb(COM1 + SER_FCR, byte);
}

int unsubscribe_uart_int() 
{
  return sys_irqrmpolicy(&hook_id);
}

int set_uart_ier(uint8_t byte)
{
  return sys_outb(COM1 + SER_IER, byte);
}

int get_uart_ier(uint8_t *byte)
{
  return util_sys_inb(COM1 + SER_IER, byte);
}

int set_uart_lcr(uint8_t byte)
{
  return sys_outb(COM1 + SER_LCR, byte);
}

int get_uart_lcr(uint8_t *byte)
{
  return util_sys_inb(COM1 + SER_LCR, byte);
}

int set_uart_iir(uint8_t byte)
{
  return sys_outb(COM1 + SER_IIR, byte);
}

int get_uart_iir(uint8_t *byte)
{
  return util_sys_inb(COM1 + SER_IIR, byte);
}

int get_uart_lsr(uint8_t *byte)
{
  return util_sys_inb(COM1 + SER_LSR, byte);
}

int set_uart_freq(uint16_t freq)
{
  uint8_t lcr, lsb, msb;
  if (get_uart_lcr(&lcr)) return !OK;
  if (set_uart_lcr(lcr | DLAB)) return !OK;
  
  util_get_LSB(freq, &lsb);
  util_get_MSB(freq, &msb);
  if (sys_outb(COM1 + SER_DLL, lsb) || sys_outb(COM1 + SER_DLM, msb))
    return !OK;
  // uint8_t shit = BIT(0) | BIT(1) | BIT(2)  | BIT(3)  | BIT(4)  | BIT(5) | BIT(6);
  // lcr &= ; // unset DLAB
  return OK;
}

void uart_ih() 
{
  printf("INSIDE UART IH!\n");
  uint8_t byte;

  util_sys_inb(COM1 + SER_IIR, &iir);
  printBits(sizeof iir, &iir);
  if(!(iir & INTERRUPT_PENDING)) {
    printf("INterrupt peding\n");
    switch(iir & INTERRUPT_ORIGIN_MASK) {
      case MODEM_STATUS_INT:
        printf("MODEM_STATUS_INT\n");
        break;
      case TRANSMITTER_EMPTY_INT:
        printf("TRANSMITTER_EMPTY_INT\n");
        while (!queue_empty(&xmit_fifo)) {
          if (uart_send_byte(*((uint8_t *)queue_front(&xmit_fifo))) != OK)
            break;
          queue_pop(&xmit_fifo);
        }
        break;
      case CHAR_TIMEOUT:
        printf("CHAR_TIMEOUT\n");
        while (uart_receive_byte(&byte))
          queue_push(&rcvr_fifo, &byte, sizeof byte);
        break;
      case RECEIVED_DATA_INT:
        printf("RECEIVED_DATA_INT\n");
        while (uart_receive_byte(&byte))
          queue_push(&rcvr_fifo, &byte, sizeof byte);
        break;
      case LINE_STATUS_INT:
        printf("LINE_STATUS_INT\n");
        break;
      default:
        printf("UART_IH: Invalid interrupt origin\n");
        break;
    }
  }
  // VER SLIDES 29/33
  // while( lsr & SER_RX_RDY ) { // Read all characters in FIFO
  //   ... // check errors
  //   sys_inb(ser_port + SER_DATA, &c);
  //   ... // "process" character read
  //   sys_inb(ser_port + SER_LSR, &lsr);
  // }
  // void ser_ih() { // serial port IH
  //   ...
  //   while( !queue_is_full(qptr) && (lsr & SER_RX_RDY)) {
  //   ...
  // }
}

int uart_send_byte(uint8_t byte) {
  uint8_t lsr, attempts_left = XMIT_TRIES;
  while (attempts_left--) {
    get_uart_lsr(&lsr);
    if (lsr & TRANSMITTER_EMPTY) {
      printf("Sending byte: %x\n", byte);
      return sys_outb(COM1 + SER_THR, byte);
    }
    micro_delay(UART_COOLDOWN);
  }
  return !OK;
}

int uart_receive_byte(uint8_t *byte) {

  uint8_t lsr, attempts_left = RCVR_TRIES;

  while (attempts_left--) {
    get_uart_lsr(&lsr);
    if (lsr & RECEIVER_DATA) {
      if (lsr & (PARITY_ERROR | FRAME_ERR | OVERRUN_ERR))
        continue;
      return util_sys_inb(COM1 + SER_RBR, byte);
    }
    micro_delay(UART_COOLDOWN);
  }
  return !OK;
}

int uart_send_string(char *string) {
  printf("Sending string: %s\n", string);
  while (*string) {
    if (uart_send_byte(*string) != OK)
      return !OK;
    string++;
  }
  return OK;
}

int uart_receive_string(char *string) {
  uint8_t byte;
  while (1) {
    if (queue_empty(&rcvr_fifo)) {
      if (uart_receive_byte(&byte) != OK)
        return !OK;
      queue_push(&rcvr_fifo, &byte, sizeof byte);
    }
    if (queue_empty(&rcvr_fifo))
      continue;
    queue_pop(&rcvr_fifo);
    if (byte == '\n')
      break;
    *string = byte;
    string++;
  }
  *string = 0;
  return OK;
}

int uart_get_conf(unsigned long *bits, unsigned long *stop, long *parity, unsigned long *rate);

int uart_poll();

int uart_int();

int uart_fifo();

