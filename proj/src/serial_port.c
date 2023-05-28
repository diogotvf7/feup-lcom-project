#include "serial_port.h"

int sp_hook_id = 7;
uint8_t read_data = 0x00;
bool other_ready = false;


int (sp_init)(){
    if (sys_outb(COM1_PORT + SER_IER, SER_INT_PENDING) != 0) {
        printf("Could not write to IER\n");
        return 1;
    }
    while (!sp_read_data())
    sp_txmit_data(SP_INIT);
    return 0;
}

int (sp_subscribe_int)(uint8_t *bit_no){
    *bit_no = sp_hook_id;
    if (sys_irqsetpolicy(COM1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &sp_hook_id) != 0) return 1;
    return 0;
}

int (sp_unsubscribe_int)(){
    if (sys_irqrmpolicy(&sp_hook_id) != 0) return 1;
    return 0;
}

int (sp_read_status)(uint8_t *st){
    if (util_sys_inb(COM1_PORT + SER_LSR, st) != 0) {
        printf("Error reading LSR status\n");
        return 1;
    }
    if (*st & (SER_OVERRUN_ERROR | SER_PARITY_ERROR | SER_FRAMING_ERROR)) {
        printf("Error detected \n");
        return 1;
    }
    return 0;
}

int (sp_read_data)(){
    uint8_t st;
    if (sp_read_status(&st) != 0) return 1;

    if (!(st & SER_RCVR_DATA)){
        printf("No data available for receiving\n");
        read_data = 0;
        return 1;
    }
    while (st & SER_RCVR_DATA){
        if (util_sys_inb(COM1_PORT + SER_RBR, &read_data)){
            printf("Error reading data\n");
            return 1;
        }
        printf("Data received: %d\n", read_data);
        // handle messages

        sp_read_status(&st);
    }
    return 0;
}

int (sp_txmit_data)(uint8_t data){
    uint8_t st;
    while(1){ // limit tries
        sp_read_status(&st);
        if (st & (SER_UART_READY & SER_TXMIT_EMPTY)){
            if (sys_outb(COM1_PORT + SER_THR, data) != 0){
                printf("Error writing data\n");
                return 1;
            }
            printf("Data sent: %d\n", data);
            return 0;
        }
        tickdelay(micros_to_ticks(5));
    }
}

int (sp_check_connection)(){
    if (read_data == SP_INIT){
        other_ready = true;
        sp_txmit_data(3);
        return 0;
    }
    if (read_data == 3){
        return 3;
    }
    return 0;
}
