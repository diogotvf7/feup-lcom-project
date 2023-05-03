#include <lcom/lab4.h>

extern uint8_t byte_index;
extern uint8_t bytes[3];

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (mouse_test_packet)(uint32_t counter){

  message msg;
  int ipc_status;
  uint8_t mouse_mask;

  if (my_mouse_enable_data_reporting() != 0) return 1;
  if (mouse_subscribe_int(&mouse_mask) != 0) return 1;

  while (counter){

    if(driver_receive(ANY, &msg, &ipc_status) != 0 ){
      printf("Error");
      continue;
    }

    if(is_ipc_notify(ipc_status)) {
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
          {
            if (msg.m_notify.interrupts & mouse_mask){
              mouse_ih(); // read byte
              mouse_receive(); // receber bytes
              if (byte_index == 3){
                struct packet pack;
                buildpacket(bytes, pack);
                mouse_print_packet(&pack);
                byte_index = 0;
                counter--;
              }
            }
          }
      }
    }
  }
  return 0;
}

