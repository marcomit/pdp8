#include "std.h"
#include <stdio.h>
#include <stdlib.h>

dnode *newdnode(void *data) {
  dnode *node = malloc(sizeof(dnode));
  node->data = data;
  node->next = NULL;
  node->prev = NULL;
  return node;
}

void freednode(dnode *node) {
  if (!node)
    return;
  free(node->data);
  free(node->next);
  free(node->prev);
}

dlist *newdlist() {
  dlist *l = malloc(sizeof(dlist));
  l->len = 0;
  return l;
}

void freedlist(dlist *l) {
  freednode(l->head);
  freednode(l->tail);
  free(l);
}

void dpush(dlist *l, void *data) {
  printf("dlist push called\n");
  dnode *node = newdnode(data);
  if (!l->head) {
    l->head = data;
    l->len = 1;
  }
  if (!l->tail) {
    l->tail = data;
    l->len = 1;
    return;
  }
  printf("setting the next tail pointer\n");
  l->tail->next = node;
  printf("setting the prev pointer of the newnode\n");
  node->prev = l->tail;
  printf("Advance the tail\n");
  l->tail = node;
  printf("Tail advanced\n");
  l->len++;
  printf("Len update\n");
}

void dunshift(dlist *l, void *data) {
  dnode *node = newdnode(data);

  if (!l->tail) {
    l->tail = node;
    l->len = 1;
  }
  if (!l->head) {
    l->head = node;
    l->len = 1;
    return;
  }

  node->next = l->head;
  l->head->prev = node;
  l->head = node;
  l->len++;
}

dnode *dpop(dlist *l) {
  if (!l->tail)
    return NULL;
  dnode *node = l->tail;

  l->tail = l->tail->prev;
  l->tail->next = NULL;
  node->prev = NULL;
  l->len--;

  return node;
}
dnode *dshift(dlist *l) {
  if (!l->head) {
    return NULL;
  }
  dnode *node = l->head;
  l->head = l->head->next;
  l->head->prev = NULL;
  node->next = NULL;
  l->len--;
  return node;
}
