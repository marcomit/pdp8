

#include <stddef.h>
typedef struct dnode {
  void *data;
  size_t len;
  struct dnode *next;
  struct dnode *prev;
} dnode;

typedef struct dlist {
  dnode *h;
  dnode *t;
  size_t len;
} dlist;

dlist *newlist();
void dpush(dlist *, void *);
void dunshift(dlist *, void *);

void *dpop(dlist *);
void *dshift(dlist *);
