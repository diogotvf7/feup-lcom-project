#pragma once

#include <lcom/lcf.h>

typedef struct Position {
  uint16_t x;
  uint16_t y;
} Position;

typedef struct Node {
  struct Position *position;
  struct Node *prev;
} Node;

typedef struct Queue {
  int size;
  struct Node *front;
  struct Node *back; 
} Queue;

int packet_queue_size();

int packet_queue_empty();

struct Position *packet_queue_front();

struct Position *packet_queue_back();

void packet_queue_push(struct Position *position);

void packet_queue_pop();

void packet_queue_clear();

void packet_queue_print();

