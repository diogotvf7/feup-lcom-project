#include "graphics.h"

vbe_mode_info_t vbe_mode_info;
uint8_t *frame_buffer;

// set graphic mode 
int(set_graphic_mode)(uint16_t submode){
  reg86_t r;
  memset(&r, 0, sizeof(r));
  r.intno = INTNO_TEXT_GRAPHICS_MODE;
  r.ax = AX_GRAPHICS_MODE;
  r.bx = submode | BIT(14);
  if (sys_int86(&r) != 0){
    printf("Error in setting graphic mode\n");
    return 1;
  }
  return 0;
}

// set text mode
int(set_text_mode)(){
  reg86_t r;
  memset(&r, 0, sizeof(r));
  r.intno = INTNO_TEXT_GRAPHICS_MODE;
  r.ah = AH_TEXT_MODE;
  r.al = AL_TEXT_MODE;
  r.bx = 0x0000;
  if(sys_int86(&r) != 0){
    printf("Error in setting text mode\n");
    return 1;
  }
  return 0;
}

int (create_frame_buffer)(uint16_t mode){

  //alocacao de memoria para a estrutura
  memset(&vbe_mode_info, 0, sizeof(vbe_mode_info));
  
  // retirar info do modo
  if(vbe_get_mode_info(mode,&vbe_mode_info) != 0) return 1;

  // cálculo do tamanho do frame buffer (bytes)
  unsigned int bytesPerPixel = (vbe_mode_info.BitsPerPixel + 7) / 8;

  unsigned int frameSize = (vbe_mode_info.XResolution * vbe_mode_info.YResolution) * bytesPerPixel;

  struct minix_mem_range phys_addresses;
  phys_addresses.mr_base = vbe_mode_info.PhysBasePtr; // pointer para inicio fisico do buffer
  phys_addresses.mr_limit = phys_addresses.mr_base + frameSize; // limite fisico do buffer

  // alocacao fisica dos buffers
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &phys_addresses) != 0) return 1;

  // alocaçao da memoria necessaria para o frame buffer
  frame_buffer = vm_map_phys(SELF, (void*) phys_addresses.mr_base,frameSize);
  if (frame_buffer == NULL) return 1;
  return 0;
}

int (normalize_color)(uint32_t color, uint32_t *newColor){
  if(vbe_mode_info.BitsPerPixel == 32){
    *newColor = color;
  }
  else{
    *newColor = color & (BIT(vbe_mode_info.BitsPerPixel) - 1);
  }
  return 0;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color){

  if (x > vbe_mode_info.XResolution || y > vbe_mode_info.YResolution) return 1;

  unsigned BytesPerPixel = (vbe_mode_info.BitsPerPixel + 7) / 8;

  unsigned int index = (vbe_mode_info.XResolution * y + x) * BytesPerPixel;

  if (memcpy(frame_buffer + index,&color,BytesPerPixel) == NULL) return 1;
  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y,uint16_t width, uint32_t color){
  for (unsigned i = 0; i < width; i++){
    if (vg_draw_pixel(x + i, y, color) != 0) return 1;
  }
  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  for (unsigned i = 0; i < height; i++){
    if (vg_draw_hline(x, y + i, width, color) != 0) {
      vg_exit();
      return 1;
    }
    /*
    for (unsigned i = 0; i < height; i++){
      for (unsigned j = 0; j < width; j++{
        if (vg_draw_pixel(x + j, y + i, color) != 0) {vg_exit(); return 1;}
      })

     */
  }
  return 0;
}

int (vg_draw_rectangulo)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
  for (unsigned i = 0; i < height; i++){
      for (unsigned j = 0; j < width; j++){
        if (vg_draw_pixel(x + j, y + i, color) != 0) {vg_exit(); return 1;}
  }}
  printf("PASSEI AQUI");
  return 0;
}

uint32_t (indexed_color_mode)(uint32_t first, uint32_t step, uint32_t row, uint32_t col, uint32_t no_rectangles){
  return (first + (row * no_rectangles + col) * step) % (1 << vbe_mode_info.BitsPerPixel);
}

uint32_t (direct_color_mode)(uint32_t red, uint32_t green, uint32_t blue){
  return (red << vbe_mode_info.RedFieldPosition) | (green << vbe_mode_info.GreenFieldPosition) | (blue << vbe_mode_info.BlueFieldPosition);
}

uint32_t (R)(uint32_t first){
  return (first >> vbe_mode_info.RedFieldPosition) & ((1 << vbe_mode_info.RedMaskSize) - 1);
}


uint32_t (G)(uint32_t first){
  return (first >> vbe_mode_info.GreenFieldPosition) & ((1 << vbe_mode_info.GreenMaskSize) - 1);
}


uint32_t (B)(uint32_t first){
  return (first >> vbe_mode_info.BlueFieldPosition) & ((1 << vbe_mode_info.BlueMaskSize) - 1);
}

