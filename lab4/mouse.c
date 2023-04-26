#include "mouse.h"


int hook_id = 3;

uint8_t byte_index = 0;       // [0..2]
uint8_t bytes[3];            // pacote        
uint8_t cur_byte;             // o byte mais recente lido

int (mouse_subscribe_int)(uint8_t *bit_no){
  if (bit_no == NULL){
    return 1;
  }

  *bit_no = hook_id;

  if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE,&hook_id) != 0) return 1;
  
  return 0;
}

int (mouse_unsubscribe_int)(){
  if (sys_irqrmpolicy(&hook_id) != 0) return 1;
  return 0;
}

void (mouse_ih)(){
  read_KBC_output(KBC_OUT_CMD,&cur_byte,1);
}

int (my_mouse_enable_data_reporting)(){

  uint8_t mouse_response;
  int c = 0;
  do {
    if (mouse_write(&mouse_response, ENABLE_DATA_REPORTING) != 0) return 1;
    if (mouse_response == ACK) return 0;
    if (mouse_response == NACK) c++;
  }while (mouse_response != ERROR && c < 2);
  return 1;
}

int (my_mouse_disable_data_reporting)(){

  uint8_t mouse_response;
  do {
    if (mouse_write(&mouse_response, DISABLE_DATA_REPORTING) != 0) return 1;
    if (mouse_response == ACK) return 0;
  }while (mouse_response != ERROR);
  return 1;

}

int (mouse_write)(uint8_t* mouse_response, uint8_t command){
  if (write_KBC_command(KBC_IN_CMD,0xD4) != 0) {printf("Couldnt warn KBC\n"); return 1;}
  if (write_KBC_command(KBC_OUT_CMD,command) != 0) {printf("Couldnt write command to KBC\n"); return 1;}
  tickdelay(micros_to_ticks(20000));
  if (read_KBC_output(KBC_OUT_CMD,mouse_response,1) != 0) {printf("Couldnt read response from mouse");return 1;}
  return 0;
}

void (mouse_receive)(){

  if (byte_index == 0 && (cur_byte & BIT(3))) { // byte control ativo, 1/3 do packet - control byte
   bytes[byte_index] = cur_byte;
   byte_index++; 
  }
  else if (byte_index > 0 && byte_index < 3){ // ler parte 2 e parte 3 do packet
  bytes[byte_index] = cur_byte;
  byte_index++;
  }
  else if (byte_index == 3){ // já leu o packet inteiro
    // talvez fazer algo com o packet aqui
    byte_index = 0;
  }
}

void (buildpacket)(uint8_t bytes[3], struct packet pack){

  for (int i = 0; i < 3;i++){
    pack.bytes[i] = bytes[i];
  }

  pack.lb = bytes[0] & BIT(0);
  pack.mb = bytes[0] & BIT(1);
  pack.rb = bytes[0] & BIT(2);
  pack.x_ov = bytes[0] & BIT(6);
  pack.y_ov = bytes[0] & BIT(7);

  pack.delta_x = bytes[1];
  pack.delta_y = bytes[2];

  if (bytes[0] & BIT(4)) pack.delta_x -= 256;
  if (bytes[0] & BIT(5)) pack.delta_y -= 256;
}

