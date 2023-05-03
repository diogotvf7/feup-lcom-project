#include "mouse.h"

static int hook_id;
uint8_t scancode;
uint8_t status;



/*
Subscrever as interrupções do mouse - irq12 e exclusive
*/
int(mouse_subscribe_int)(uint8_t *bit_no){
  hook_id = 3;
  *bit_no = hook_id;
  if(sys_irqsetpolicy(MOUSE_IRQ,(IRQ_REENABLE | IRQ_EXCLUSIVE), &hook_id) != OK){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;

}

int(mouse_unsubscribe_int)(){
   // uint8_t temp;

  if (sys_irqrmpolicy(&hook_id) != 0) {return 3;}

  return 0;
}

/*Verificar se o outputBuffer tem data para ser lida*/
bool (checkOutputBuffer)(){
  if(util_sys_inb(KBC_STAT_REG, &status) != OK){return false;}
  if(status & OBF){
    if((status & (PARITY_ERROR | TIMEOUT_ERROR)) == 0)
      return true;
  }
  return false;

}

bool (checkInputBuffer)(){
  if(util_sys_inb(KBC_STAT_REG, &status) != OK){return false;}
  if((status & IBF) != 0) //ta full logo nao escrever
    return false;
  return true;
}


void(mouse_ih)(){
  
  if(checkOutputBuffer()){
      if(util_sys_inb(KBC_OUT_BUF, &scancode) != OK){scancode = 0x00;}
  }
  
}


int (mouse_enable_data_report)() {
  uint8_t resp;
  uint8_t seconderror = 0;

  do {
    if (sys_outb(KBC_CTRL_REG, WRITE_BYTE_MOUSE) != 0) {return 1;}

    if (mouse_write_cmd(ENABLE_DATA_REP, &resp) != 0) {return 1;}

    if (resp == ACK) {return 0;}
    else if (resp == NACK) {seconderror++;}
    else if (resp == ERROR){return 1;}
    
  } while (seconderror < 2);

  return 1;
}

int (mouse_disable_data_reporting)(){

  uint8_t ans;
  uint8_t numberErrors = 0;

  do{
      if(sys_outb(KBC_CTRL_REG, WRITE_BYTE_MOUSE) != OK){return EXIT_FAILURE;}
      if(mouse_write_cmd(DISABLE_DATA_REP, &ans) != OK){return EXIT_FAILURE;}

      if(ans == ACK) return EXIT_SUCCESS; //todos os bytes estao bem
      else if(ans == ERROR) return EXIT_FAILURE;

  }while(numberErrors < 2);

  return EXIT_SUCCESS;

}
/*chamamos mais do que uma vez porque pode ter o erro de nack e por isso temos de reler*/

int (mouse_write_cmd)(uint32_t cmd, uint8_t *ans){
  int count = 0;

  //one for each byte 
  while(count < 3){
      if(checkInputBuffer()){
        if(sys_outb(KBC_IN_BUF, cmd) != OK){return EXIT_FAILURE;}
        if(util_sys_inb(KBC_OUT_BUF, ans) != OK){return EXIT_FAILURE;}
        return 0;
    }
    count++;
  }

  return EXIT_FAILURE;

}



void buildPacket(struct packet* pacote, uint8_t bytes[3]){



  pacote->lb = (bytes[0] & BIT(0));
  pacote->rb = (bytes[0] & BIT(1));
  pacote->mb = (bytes[0] & BIT(2));
  pacote->x_ov = (bytes[0] & BIT(6));
  pacote->y_ov = (bytes[0] & BIT(7));

  /*Se o msb tiver a 1 o numero e negativo*/
  if(bytes[0] & BIT(4)){
    pacote->delta_x = bytes[1] - 256;
  }else{pacote->delta_x = bytes[1];}

  if(bytes[0] & BIT(5)){
    pacote->delta_y = bytes[2] - 256;
  }else{pacote->delta_y = bytes[2];}


}



