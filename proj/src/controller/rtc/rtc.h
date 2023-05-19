#pragma once
#include <lcom/lcf.h>
#include "../utils.h"



#define IRQ_RTC 8
#define REGISTER_COUNTING 11
#define IS_BINARY BIT(2)
#define UPDATING BIT(7)
#define REG_INPUT 0x70
#define REG_OUTPUT 0x71
#define REGISTER_UPDATING   10
#define SECONDS    0
#define MINUTES    2
#define HOURS      4   
#define DAY        7
#define MONTH      8
#define YEAR       9

typedef struct{
  uint8_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
} real_time;


void rtc_init();

int rtc_subscribe_int();

int rtc_unsubscribe_int();

int is_binary();

int read_rtc_output(uint8_t command, uint8_t *output);

int rtc_is_updating();

int rtc_update_time();


