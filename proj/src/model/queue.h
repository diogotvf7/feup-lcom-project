#pragma once

#include <lcom/lcf.h>
#include "global_vars.h"

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

int queue_size(Queue **queue);

// int empty(Queue **queue);

struct Position *queue_front(Queue **queue);

// struct Position *get_back(Queue **queue);

void queue_push(Queue **queue, Position *position);

void queue_pop(Queue **queue);

void queue_clear(Queue **queue);

void queue_print(Queue **queue);

