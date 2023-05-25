int ser_subscribe_int(uint8_t *bit_no);

int ser_unsubscribe_int();

int ser_send_byte(uint8_t byte);

int ser_receive_byte(uint8_t *byte);

int ser_send_string(char *string);

int ser_receive_string(char *string);

int ser_set_conf(unsigned long bits, unsigned long stop, long parity, unsigned long rate);

int ser_get_conf(unsigned long *bits, unsigned long *stop, long *parity, unsigned long *rate);

void ser_ih() {
    sys_inb(ser_port + SER_IIR, &iir);
    if( iir & SER_INT_PEND ) {
        switch( iir & INT_ID ) {
            case SER_RX_INT:
                /* read received character */
                break;
            case SER_TX_INT:
                /* put character to sent */
                break;
            case SER_RX_ERR:
                /* notify upper level */
                break;
            case SER_XXXX:
                /* depends on XXX */
                break;
        }
    }
}

int ser_poll();

int ser_int();

int ser_fifo();
