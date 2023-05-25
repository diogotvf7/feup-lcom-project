#pragma once

#include <lcom/lcf.h>
#include "global_vars.h"

typedef struct Position {
  uint16_t x;
  uint16_t y;
} Position;

typedef struct Node {
  void *data;
  int remaining;
  int size;
  struct Node *prev;
} Node;



typedef struct Queue {
  int size;
  struct Node *front;
  struct Node *back; 
} Queue;

int queue_size(Queue **queue);

int queue_empty(Queue **queue);

void *queue_front(Queue **queue);

// struct Position *get_back(Queue **queue);

void queue_push(Queue **queue, void *data, int size);

void queue_pop(Queue **queue);

void queue_clear(Queue **queue);

// void queue_print(Queue **queue);
