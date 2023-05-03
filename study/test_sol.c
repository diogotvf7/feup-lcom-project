#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#define TIMER_IRQ 10

#define TIMER_PORT0 0x20

#define TIMER_PORT1 0x21

#define TIMER_PORT2 0x22

#define CTRL_REG 0x23

#define BIT(n) (1 << (n))

#define CWD_SEL_ALARM BIT(4)

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}

enum l3223_time_units {
    l3223_microsecond,
    l3223_millisecond,
    l3223_second
};


int pp_test_alarm(int timer, int interval, enum l3223_time_units unit) {
    
    uint8_t commandWord = 0;

    switch (timer)
    {
    case 0:
        break;
    case 1:
        commandWord |= BIT(6);
        break;
    case 2:
        commandWord |= BIT(7);
        break;
    default:
        return 1;
    }

    commandWord |= CWD_SEL_ALARM;

    switch (unit)
    {
    case l3223_microsecond:
        break;
    case l3223_millisecond:
        commandWord |= BIT(0);
        break;
    case l3223_second:
        commandWord |= BIT(1);
        break;
    default:
        return 1;
    }

    int ipc_status;
    message msg;

    int hook_id = TIMER_IRQ;

    int bit_no = BIT(hook_id);

    uint8_t lsb,msb;

    lsb = (uint8_t)(interval & 0xFF);

    msb = (uint8_t)(interval >> 8);

    if (sys_outb(CTRL_REG,commandWord) != 0) return 1;

    if (sys_outb(TIMER_PORT0 + timer,lsb) != 0) return 1;

    if (sys_outb(TIMER_PORT0 + timer, msb) != 0) return 1;

    if (sys_irqsetpolicy(TIMER_IRQ,IRQ_REENABLE,&hook_id) != 0) return 1;

    while(true) {

    if (driver_receive(ANY, &msg, &ipc_status) != 0){ // didnt receive any message
        printf("driver_receive failed");
        continue;
    }

    if (is_ipc_notify(ipc_status)) { /* received message and is a notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:                 
            if (msg.m_notify.interrupts & bit_no){
                
                uint32_t temp;

                if (sys_inb(CTRL_REG,&temp) != 0) return 1;

                uint8_t config = (uint8_t)(0xFF & temp);

                if (config & BIT(timer)){
                    pp_print_alarm(timer,interval,unit);
                    break;
                }

            }
          break;
        default:
          break;     
        }
      } 
  }

  if (sys_irqrmpolicy(&hook_id) != 0) return 1;

  return 0;
}