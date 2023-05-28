#pragma once

#include <lcom/lcf.h>
#include "global_vars.h"

typedef struct Node {
  void *data;
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

void queue_push(Queue **queue, void *data);

void queue_pop(Queue **queue);

void queue_clear(Queue **queue);

// void queue_print(Queue **queue);

