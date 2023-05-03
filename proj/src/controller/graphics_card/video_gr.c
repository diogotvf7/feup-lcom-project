#include <lcom/lcf.h>
#include <lcom/video_gr.h>
#include <VBE.h>
#include "graphics_card.h"

uint8_t *video_mem = NULL;
unsigned bytes_per_pixel;
unsigned h_res;
unsigned v_res;
uint8_t red_field_position;
uint8_t red_mask_size;
uint8_t green_field_position;
uint8_t green_mask_size;
uint8_t blue_field_position;
uint8_t blue_mask_size;
uint8_t mem_model;

void *(vg_init)(uint16_t mode) {
    void* video_mem = map_graphics_memory(mode);
    reg86_t reg86;
    bzero(&reg86, sizeof reg86);
    
    reg86.intno = GRAPHICS_INT_NO;
    reg86.ah = VESA_FUNC;
    reg86.al = SET_GRAPHICS_MODE;
    reg86.bx = mode | LINEAR_MODE;

    if (sys_int86(&reg86) != OK)
        return NULL;
    return video_mem;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (int i = 0; i < len; i++, x++)
        vg_draw_pixel(x, y, color);
    return EXIT_SUCCESS;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) 
{
    for (int i = 0; i < height; i++, y++)
        vg_draw_hline(x, y, width, color);
    return EXIT_SUCCESS;
}
