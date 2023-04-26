#ifndef _I8254_H_
#define _I8254_H_

#define TIMER0_IRQ 0

#define TIMER1_IRQ 1

#define TIMER2_IRQ 2

#define TIMER0_PORT 0x40

#define TIMER1_PORT 0x41

#define TIMER2_PORT 0x42

#define CONTROL_REG 0x43

#define TIMER_FREQ 1193182

#define TIMER_SEL0 0x00

#define TIMER_SEL1 BIT(6)

#define TIMER_SEL2 BIT(7)

#endif
