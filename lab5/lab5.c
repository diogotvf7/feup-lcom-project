// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include "graphics.h"
#include "keyboard.h"
#include "i8042.h"
#include <lcom/lab5.h>
#include <stdint.h>
#include <stdio.h>

extern vbe_mode_info_t vbe_mode_info;
extern uint8_t scancode;


// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(ESC_key_pressed)(){
  
  int ipc_status;
  message msg;
  uint8_t bit_no = 1;

  if (keyboard_subscribe_interrupts(&bit_no) != 0) return 1;

  while(scancode != BREAK_ESC){
    if (driver_receive(ANY, &msg, &ipc_status) != 0) { 
    printf("driver_receive failed");
    continue;
  }
    if (is_ipc_notify(ipc_status)){
      switch (_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
        if(msg.m_notify.interrupts & bit_no){
          kbc_ih();
          break;
        }
        default:
          break;
      }
    }
  }

  if (keyboard_unsubscribe_interrupts() != 0) return 1;
  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  
  if (set_graphic_mode(mode) != 0) return 1;

  sleep(delay);

  if (vg_exit() != 0) return 1;
  

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {

  /* construction of the virtual/physic buffer */
  if (create_frame_buffer(mode) != 0) return 1;

  /* setting graphic mode*/
  if (set_graphic_mode(mode) != 0) return 1;

  /* normalize colour depending on mode*/
  uint32_t newColor;
  if (normalize_color(color, &newColor) != 0) return 1;

  /* draw rectangle*/
  if (vg_draw_rectangulo(x, y, width, height, newColor) != 0) return 1;

  /* function that returns when ESC is clicked*/
  if (ESC_key_pressed() != 0) return 1;

  /* setting text mode*/
  if (vg_exit() != 0) return 1;

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {

  if (create_frame_buffer(mode) != 0) return 1;

  if (set_graphic_mode(mode) != 0) return 1;

  uint16_t rheight = vbe_mode_info.YResolution / no_rectangles;
  uint16_t rwidth = vbe_mode_info.XResolution / no_rectangles;
  uint16_t stripe_height = vbe_mode_info.YResolution % no_rectangles;
  uint16_t stripe_width = vbe_mode_info.XResolution % no_rectangles;

  for (int i = 0; i < no_rectangles; i++){
    if (rheight * i >= (vbe_mode_info.YResolution - stripe_height)) break;
    for (int j = 0; j < no_rectangles; j++){

      if (rwidth * j >= (vbe_mode_info.XResolution - stripe_width)) break;
      
      uint32_t color;

      if (vbe_mode_info.MemoryModel != DIRECT_COLOR_MODE){

        color = indexed_color_mode(first,step,no_rectangles,i,j);

      }
      else{
        uint32_t red = (R(first) + j * step) % (1 << vbe_mode_info.RedMaskSize);
        uint32_t green = (G(first) + i * step) % (1 << vbe_mode_info.GreenMaskSize);
        uint32_t blue = (B(first) + (i + j) * step) % (1 << vbe_mode_info.BlueMaskSize);

        color = direct_color_mode(red, green, blue);
      }

      if (vg_draw_rectangle(i * rwidth, j * rheight, rwidth, rheight, color) != 0) return 1;
    }
  }

  /* returns if ESC key pressed == end*/
  if (ESC_key_pressed() != 0) return 1;

  /* setting text mode*/
  if (vg_exit() != 0) return 1;

  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  
  if (create_frame_buffer(0x105) != 0) return 1;

  if (set_graphic_mode(0x105) != 0) return 1;

  xpm_image_t xpm_image;

  uint8_t* pixmap = xpm_load(xpm,XPM_INDEXED,&xpm_image);

  for (int h = 0; h < xpm_image.height; h++){
    for (int w = 0; w < xpm_image.width; w++){
      if (vg_draw_pixel(x + w, y + h, *pixmap) != 0) return 1;
      pixmap++;
    }
  }

  if (ESC_key_pressed() != 0) return 1;

  if (vg_exit() != 0) return 1;

  return 1;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  
  

  return 1;
}

int(video_test_controller)() {



  return 1;
}
