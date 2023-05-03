#include <lcom/lcf.h>
#include <stdbool.h>
#include <stdint.h>

#define TIMER0_PORT 0x20

#define TIMER1_PORT 0x21

#define TIMER2_PORT 0x22

#define CTRL_REG 0x23

#define TIMER_IRQ 10

#define BIT(n) (1 << (n))

enum l3223_time_units {
    l3223_microsecond,
    l3223_millisecond,
    l3223_second
};

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}

int pp_test_alarm(int timer, int interval, enum l3223_time_units unit){

    uint8_t commandWord = 0;

    switch (timer) // timer bit
    {
    case 0:
        break;
    case 1:
        commandWord = commandWord | BIT(6);
        break;
    case 2:
        commandWord = commandWord | BIT(7);
        break;
    default:
        return 1;
    }

    commandWord = commandWord | BIT(4); // alarm bit 

    switch(unit){ // unit bit
        case (l3223_microsecond):
            break;
        case (l3223_millisecond):
            commandWord = commandWord | BIT(0);
            break;
        case (l3223_second):
            commandWord = commandWord | BIT(1);
            break;
        default:
            return 1;
    }

    if (sys_outb(CTRL_REG,commandWord) != 0) return 1; // warn l3223 that you are configuring certain timer with alarm mode and with certain unit

    uint8_t msb = (uint8_t) interval >> 8; 

    uint8_t lsb = interval & 0xFF;

    if (sys_outb(TIMER0_PORT + timer,lsb) != 0) return 1; // write the lsb to timer

    if (sys_outb(TIMER0_PORT + timer,msb) != 0) return 1; // write the msb to timer

    int hook_id = TIMER_IRQ;

    int bit_no = BIT(hook_id);

    if (sys_irqsetpolicy(TIMER_IRQ,IRQ_REENABLE,&hook_id) != 0) return 1; // subscribe interrupts to that bit  

    int ipc_status;
    message msg;

    while (true){

        if (driver_receive(ANY, &msg, &ipc_status)) {
            printf("Driver_receive failed\n");
            continue;
        }

        if (is_ipc_notify(ipc_status)) { /* received message and is a notification */
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & bit_no){
                        
                        uint32_t res;

                        if (sys_inb(CTRL_REG,res) != 0) return 1;

                        uint8_t config = (uint8_t) res;

                        if (config & BIT(timer)){
                            pp_print_alarm(timer,interval,unit);
                            break;
                        } 

                    }

            }
        }
    }

    sys_irqrmpolicy(&hook_id);

    return 0;
}
