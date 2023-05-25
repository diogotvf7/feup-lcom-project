#include "queue.h"

Queue *pos_queue = NULL;
Queue *garbage = NULL;

int queue_size(Queue **queue) {
  if (*queue == NULL) return 0;
  return (*queue)->size;
}

int queue_empty(Queue **queue) {
  return *queue == NULL;
} 

void *queue_front(Queue **queue) {
  if (*queue == NULL) return NULL;
  return (*queue)->front->data;
}

// Position *back(Queue **queue) {
//   if (*queue == NULL) return NULL;
//   return (*queue)->back->position;
// }

void queue_push(Queue **queue, void *data, int size) {  
  Node *n = (Node *)malloc(sizeof(Node));
  n->data = (uint16_t *) data;
  // printf("------> Pushing: %p\n", data);
  n->remaining = size;
  n->size = size;
  n->prev = NULL;
  if (*queue == NULL) {
    *queue = (Queue *)malloc(sizeof(Queue));
    (*queue)->size = 1; 
    (*queue)->back = n;
    (*queue)->front = n;
  }
  else {
    (*queue)->back->prev = n;
    (*queue)->back = n;
    (*queue)->size++;
  }
}

void queue_pop(Queue **queue) {
  if (*queue == NULL) return;
  if ((*queue)->size == 1) {
    *queue = NULL;
    return;
  }
  (*queue)->front = (*queue)->front->prev;
  (*queue)->size--;
}

void queue_clear(Queue **queue) {
  while (*queue != NULL) {
    Node *old_front = (*queue)->front;
    queue_pop(queue);
    free(old_front->data);  
    free(old_front); 
  }
}

// void queue_print(Queue **queue) {
//   if (*queue == NULL) return;
//   Node *temp = (*queue)->front;
//   printf("|       |\nv Front v\n_________\n");
//   while (temp != NULL) {
//     printf("X: %d      Y: %d\n", temp->data->x, temp->data->y);
//     temp = temp->prev;
//   }
//   printf("_________\n^ Back  ^\n|       |\n\n");
// }


