#include <lcom/lcf.h>
#include "lcom/timer.h"
#include <stdint.h>
#include "i8254.h"

int hook_id = 0;
int counter = 0;

int (timer_subscribe_int)(uint8_t *bit_no){
  if (bit_no == NULL) return 1;
  *bit_no = BIT(hook_id);
  if (sys_irqsetpolicy(TIMER0_IRQ,IRQ_REENABLE,&hook_id) != 0) return 1;
  return 0;
}

int (timer_unsubscribe_int)(){
  if (sys_irqrmpolicy(&hook_id) != 0) return 1;
  return 0;
}

void (timer_int_handler)(){
  counter++;
}

int(timer_get_conf)(uint8_t timer, uint8_t *st){
  if (st == NULL || timer > 2 || timer < 0) return 1; 
  uint8_t read_back_command = BIT(7) | BIT(6) | BIT(5) | BIT(timer + 1); // formar RBC de maneira a dizer que queremos que seja do tipo RBC (bit 7 e 6 ativado) , que queremos apenas a config (bit 5 ativado) e que queremos a config do timer respetivo (bit 1 é timer 0)
  if (sys_outb(CONTROL_REG, read_back_command) != 0) return 1; // avisar control reg que vamos ler
  if (util_sys_inb(TIMER0_PORT + timer,st) != 0) return 1; // leitura da config/status do timer
  return 0;
}

int(timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field){
  
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
      return 1;
    }
    break;
  
  case tsf_mode: 
    st = (st >> 1) & 0x07;
    
    if (st == 6) data.count_mode = 2;
    if (st == 7) data.count_mode = 3;
    else data.count_mode = st;

    break;

  case tsf_base:

    st = st & 0x01;
    data.bcd = st;
    break;

  default:
    return 1;
  }

  if (timer_print_config(timer,field,data) != 0) return 1;
  
  return 0;
}

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {

  if(freq < 19 || freq > TIMER_FREQ || timer < 0 || timer > 2) return 1;
  uint8_t controlWord;

  if(timer_get_conf(timer, &controlWord) != 0) return 1;

  controlWord = (controlWord & 0x0F) | (BIT(4) | BIT(5));

  uint16_t counter = TIMER_FREQ / freq;
  uint8_t MSB, LSB;
  if(util_get_MSB(counter, &MSB) != 0) return 1;
  if(util_get_LSB(counter, &LSB) != 0) return 1;

  uint8_t selectedTime;

  switch (timer){
    case 0:
      controlWord |= TIMER_SEL0;
      selectedTime = TIMER0_PORT;
      break;
    case 1:
      controlWord |= TIMER_SEL1;
      selectedTime = TIMER1_PORT;
      break;

    case 2:
      controlWord |= TIMER_SEL2;
      selectedTime = TIMER2_PORT;
    default:
        return 1;
  }
  if(sys_outb(CONTROL_REG, controlWord) != 0) return 1;

  if(sys_outb(selectedTime, LSB) != 0) return 1;
  if(sys_outb(selectedTime, MSB) != 0) return 1;

  return 0;
}

