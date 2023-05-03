#pragma once
#include <lcom/lcf.h>
#include "../keyboard/i8042.h"
#include "../utils.h"

int(mouse_subscribe_int)(uint8_t *bit_no);

int(mouse_unsubscribe_int)();

void(mouse_ih)();

int (mouse_disable_data_reporting)();

bool (checkOutputBuffer)();

bool (checkInputBuffer)();

void buildPacket(struct packet* pacote, uint8_t bytes[3]);

int (mouse_write_cmd)(uint32_t cmd, uint8_t* ans);

int (mouse_enable_data_report)();

bool (draw_handler)(uint8_t x_len, uint8_t tolerance, struct packet* pacote);

struct mouse_ev * mouse_events(struct packet * pack);

