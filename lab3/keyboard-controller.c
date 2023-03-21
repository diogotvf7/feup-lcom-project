
#include <lcom/lcf.h>
#include "i8042.h"
#include "keyboard-controller.h"

int (read_KBC_status)(uint8_t* status) {
    if (util_sys_inb(KBC_STATUS_REG, status) != 0) return 1;
    return util_sys_inb(KBC_STATUS_REG, status);
}

int (write_KBC_command)(uint8_t port, uint8_t commandByte){
  uint8_t status;
  int attempts = 10;

  while (attempts) {
    
    if (read_KBC_status(&status) != 0) {printf("Error: Status is not available\n");
      return 1;
    }

    if ((status & BIT(1)) == 0){ // se estiver a 1, o input de entrada está cheio
      if(sys_outb(port,commandByte) != 0){
        printf("Could not write cmdByte\n");
        return 1;
      }
      return 0; // escreveu corretamente
    }
    tickdelay(micros_to_ticks(20000));
    attempts--;
  }
  return 1;
}

int (read_KBC_output)(uint8_t port, uint8_t *output) {
  uint8_t status;
  int attempts = 10;
  
  while (attempts){
    if (read_KBC_status(&status)){printf("Error: Status is not available\n");
    return 1;}

    if ((status & BIT(0)) != 0){ // averiguar se o obf está cheio

      if(util_sys_inb(port,output) != 0){
        printf("Could not read cmdByte\n");
        return 1;
      }
      if((status & BIT(7)) != 0){                   // verifica erro de paridade
        printf("Error: Parity error!\n");           // se existir, descarta
        return 1;
      }
      if((status & BIT(6)) != 0){                     // verifica erro de timeout
        printf("Error: Timeout error!\n");          // se existir, descarta
        return 1;
      }
      return 0;
    }
    tickdelay(micros_to_ticks(20000));
    attempts--;
  }
  return 1;
}
