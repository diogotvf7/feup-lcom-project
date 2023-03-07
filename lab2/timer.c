#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

static int hook_id = 1;
int counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  if ( freq > TIMER_FREQ || freq < 19) return 1;

  uint8_t controlWord;
  if (timer_get_conf(timer, &controlWord) != 0) return 1;

  controlWord = (controlWord & 0x0F) | TIMER_LSB_MSB;

  uint32_t init_value = TIMER_FREQ / freq;

  uint8_t MSB, LSB;

  util_get_LSB(init_value, &LSB);

  util_get_MSB(init_value, &MSB);

  uint8_t selectedTimer;

  switch (timer)
  {
  case 0:
    controlWord = controlWord | TIMER_SEL0; // diz ao controlWord que queremos mudar o timer 0
    selectedTimer = TIMER_0;
    break;
  case 1:
    controlWord = controlWord | TIMER_SEL1;
    selectedTimer = TIMER_1;
    break;
  case 2:
    controlWord = controlWord | TIMER_SEL2;
    selectedTimer = TIMER_2;
    break;
  default:
    return 1;
  }

  if (sys_outb(TIMER_CTRL,controlWord) != 0) return 1; // avisar o i8254 que vamos configurá-lo

  if (sys_outb(selectedTimer, LSB)!= 0) return 1; // primeiro lsb como mandam as regras, injetar valor inicial no counter

  if (sys_outb(selectedTimer, MSB)!= 0) return 1; // then msb, injetar valor inicial no counter

  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1; // testar validez do pointer
  *(bit_no) = BIT(hook_id);
  if (sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&hook_id) !=0) return 1;
  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id) != 0) return 1;
  return 1;
}

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  
  if (timer < 0 || timer > 2 || st == 0){return 1;}

  uint8_t read_back_command = TIMER_RB_CMD | BIT(5) | BIT(timer+1);

  if (sys_outb(TIMER_CTRL,read_back_command) != 0) return 1;

  if (util_sys_inb(TIMER_0 + timer,st) != 0) return 1;

  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {

  union timer_status_field_val data;

  switch (field)
  {
  case tsf_all:
    data.byte = st;
    break;
  case tsf_initial:

    st = (st >> 4) & 0x03;

    switch (st)
    {
    case 1:
      data.in_mode = LSB_only;
      break;
    case 2:
      data.in_mode = MSB_only;
      break;
    case 3:
      data.in_mode = MSB_after_LSB;
      break;
    default:
      data.in_mode = INVAL_val;
    }
    break;
    
  case tsf_mode:
    
    st = (st >> 1) & (0x07);

    if (st == 2 || st == 6){
      data.count_mode = 2;
    }

    else if (st == 3 || st == 7){
      data.count_mode = 3;
    }

    else {
    data.count_mode = st;
    }
      break;
      
  case tsf_base:

    st = st & 0x01;
    data.bcd = st;

    break;
  }

  if (timer_print_config(timer,field,data) != 0) return 1;
  
  return 0;
}
