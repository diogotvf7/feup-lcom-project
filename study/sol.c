#include <lcom/.h>
#include <stdbool.h>
#include <stdint.h>
​
int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}
​
/*
​
Timer L3223:
​
Three 16bit counters + 8bit register;
​
Timer has 2 modes:
        - perodic (equal to mode 3 in i8254)
        - alarm
​
No modo alarme, um int é gerado dps do tempo programado acabar
​
3 unidades de tempo:
        1 microsec
        1 milisec
        1 sec
​
Carregar valores (tem de ser binario):
        LSB primeiro. Dps MSB
​
​
​
CTRL_WORD:
​
        Bits 6,7 para escolher timer
​
        Bit 4(mode):
                0 - periodic
                1 - alarm
​
        Bits 0,1 (units):
                00 - microsec
                01 - milisec
                10 - sec
​
        Remaning bits at 0!
​
​
​
STATUS_BYTE:
​
        BIT 2 - interrupt pendente de timer 2
        BIT 1 - interrupt pendente de timer 1
        BIT 0 - interrupt pendente de timer 0
​
        Remaning bits at 0!s
​
*/

#define BIT(n) (1 << (n))

// Ports
#define PORT_TIMER0 0x20
#define PORT_TIMER1 0x21
#define PORT_TIMER2 0x22

#define PORT_REG 0X23

// Control Word
#define CW_TIMER_SEL(n) ((n) << 6)
#define CW_ALARM_MODE BIT(4)

enum l3223_time_units { l3223_microsecond, l3223_millisecond, l3223_second };

#define TIMER_IRQ 10

/*
Implementar:
​
configurar timer para gerar alarme (interrupt)
apos um tempo especificado em 2º argumento e tipo
de tempo no 3º argumento.
​
Quando o tempo acaba, invocar (já definida):
​
int pp_print_alarm(int timer, int interval, enum l3223_time_units unit);
​
​
Policy : IRQ_REENABLE
IRQ : 10
​
​
*/
int pp_test_alarm(int timer, int interval, enum l3223_time_units unit) {
​
  uint8_t cmd = 0;
​
  cmd |= CW_TIMER_SEL(timer) | CW_ALARM_MODE;
​
  switch (unit) {
​
  case (l3223_microsecond):
    break; // change nothing
​
  case (l3223_millisecond):
    cmd |= BIT(0);
    break;
​
  case (l3223_second):
    cmd |= BIT(1);
    break;
​
  default:
    break;
  }
​
  // Control word feita. Escrever no controlo timer
  sys_outb(PORT_REG, cmd);
​
  // Calcular intervalo que queremos no timer
  uint8_t lsb = (uint8_t)(interval & 0xff);
  uint8_t msb = (uint8_t)((interval >> 8) & 0xff);
​
  // Escrever agora o tempo no timer LSB, MSB
  sys_outb(PORT_TIMER0 + timer, lsb);
  sys_outb(PORT_TIMER0 + timer, msb);
​
  // Agora temos de subscrever e esperar um interrupt
  int hook_id = TIMER_IRQ;
  sys_irqsetpolicy(TIMER_IRQ, IRQ_REENABLE, &hook_id);

  int ipc_status;
  message msg;

  while (1) {
    if (driver_receive(ANY, &msg, &ipc_status)) {
      printf("Driver_receive failed\n");
      continue;
    }

    if (is_ipc_notify(ipc_status) && _ENDPOINT_P(msg.m_source) == HARDWARE)
      if (msg.m_notify.interrupts & BIT(TIMER_IRQ)) {

        // Leitura de status para ver se/que timers tem interrupts
        uint32_t aux;

        sys_inb(PORT_REG, &aux);

        uint8_t st = (uint8_t)(aux & 0XFF);

				// se há interrupt do timer que queremos fazer print e parar
        if (st & BIT(timer)) {
          pp_print_alarm(timer, interval, unit);
          break;
        }
				
      }
  }

  sys_irqrmpolicy(&hook_id);
  return 0;
}