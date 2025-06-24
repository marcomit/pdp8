#include "dlist.h"
#include <stdlib.h>

dlist *dnew() {
  dlist *l = malloc(sizeof(dlist));
  return l;
}

static dnode *newnode(void *data) {
  dnode *node = malloc(sizeof(dnode));
  node->data = data;
  node->len = 0;
  return node;
}

void dpush(dlist *l, void *data) {
  dnode *n = newnode(data);
  if (!l->h) {
    l->h = n;
    l->len = 1;
  }
  if (!l->t) {
    l->t = n;
    l->len = 1;
    return;
  }
  l->t->next = n;
  l->t = n;
  l->len++;
}

void *dpop(dlist *l) {
  if (l->len == 0)
    return NULL;
}
