#ifndef STD_H
#define STD_H

#include <stddef.h>
typedef struct dnode {
  void *data;
  struct dnode *prev;
  struct dnode *next;
} dnode;
typedef struct dlist {
  struct dnode *head;
  struct dnode *tail;
  size_t len;
} dlist;

typedef struct hashset {
} hashset;

typedef struct hashmap {
} hashmap;

dlist *newdlist();
void dpush(dlist *, void *);
void dunshift(dlist *, void *);
dnode *dpop(dlist *);
dnode *dshift(dlist *);
#endif // !STD_H
