#pragma once

#include <lcom/lcf.h>

typedef struct Node {
  struct packet *mouse_packet;
  struct Node *prev;
} Node;

typedef struct Queue {
  int size;
  struct Node *front;
  struct Node *back; 
} Queue;

int packet_queue_size();

int packet_queue_empty();

struct Node *packet_queue_front();

struct Node *packet_queue_back();

void packet_queue_push(struct packet mouse_packet);

void packet_queue_pop();

void packet_queue_clear();

void packet_queue_print();


