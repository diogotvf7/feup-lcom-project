#include "mouse_packet_queue.h"

Queue *mouse_packet_queue = NULL;

int packet_queue_size() {
  if (mouse_packet_queue == NULL) return 0;
  return mouse_packet_queue->size;
}

int packet_queue_empty() {
  return mouse_packet_queue == NULL;
} 

Position *packet_queue_front() {
  if (mouse_packet_queue == NULL) return NULL;
  return mouse_packet_queue->front->position;
}

Position *packet_queue_back() {
  if (mouse_packet_queue == NULL) return NULL;
  return mouse_packet_queue->back->position;
}

void packet_queue_push(struct Position *position) {
  Node *n = (Node *)malloc(sizeof(Node));
  n->position = position;
  n->prev = NULL;
  if (mouse_packet_queue == NULL) {
    mouse_packet_queue = (Queue *)malloc(sizeof(Queue));
    mouse_packet_queue->size = 1; 
    mouse_packet_queue->back = n;
    mouse_packet_queue->front = n;
  }
  else {
    mouse_packet_queue->back->prev = n;
    mouse_packet_queue->back = n;
    mouse_packet_queue->size++;
  }
  if (mouse_packet_queue->size > 20) 
    packet_queue_pop();    
}

void packet_queue_pop() {
  if (mouse_packet_queue == NULL) return;
  if (mouse_packet_queue->size == 1) {
    mouse_packet_queue = NULL;
    return;
  }
  Node *old_front = mouse_packet_queue->front;
  mouse_packet_queue->front = mouse_packet_queue->front->prev;
  mouse_packet_queue->size--;
  free(old_front);
}

void packet_queue_clear() {
  while (mouse_packet_queue != NULL) {
    packet_queue_pop();
  }
}

void packet_queue_print() {
  if (mouse_packet_queue == NULL) return;
  Node *temp = mouse_packet_queue->front;
  printf("|       |\nv Front v\n_________\n");
  while (temp != NULL) {
    // mouse_print_packet(&temp->mouse_packet);
    printf("X: %d      Y: %d\n", temp->position->x, temp->position->y);
    temp = temp->prev;
  }
  printf("_________\n^ Back  ^\n|       |\n");
}


