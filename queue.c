#include "types.h"
#include "param.h"
#include "defs.h"
#include "spinlock.h"
#include "queue.h"

void
initnode(struct node *n)
{
  n->nxt = 0;
  n->state = NEW;
  n->data = 0;
}

void
initqueue(struct queue *q, char *n)
{
  long t = 0;
  long te = t;
  cprintf("init queue %s \n", n);

  acquire(&tickslock);
  t = ticks;
  te = t;
  release(&tickslock);

  q->name = n;

  int i;
  for(i = 0; i < NNODES; ++i)
    initnode(&(q->nodes[i]));

  q->head = 0;
  initlock(&(q->lock), n);

  acquire(&tickslock);
  te = ticks;
  t = te - t;
  release(&tickslock);

  cprintf("finished in %d ticks\n", t);
}

struct node *
addnode(struct queue *q, void *d)
{
  acquire(&(q->lock));

  struct node a;
  a.nxt = 0;
  a.state = OLD;
  a.data = d;

  int i;
  for(i = 0; i < NNODES; ++i)
    if(q->nodes[i].state == NEW || q->nodes[i].state == OLD)
      goto found;

  cprintf("queue %s is full", q->name);
  return 0;

found:
  q->nodes[i] = a;
  release(&(q->lock));

  return &q->nodes[i];
}

int
qsize(struct queue *q)
{
  acquire(&q->lock);

  struct node *h;
  int a = 0;
  for(h = q->head; h->nxt != 0; h = h->nxt)
    ++a;

  release(&q->lock);
  cprintf("queue %s has size %d\n", q->name, a);

  return a;
}

void
insert(struct queue *q, struct node *n, int i)
{
  cprintf("inserting node to %s\n", q->name);

  struct node *t = addnode(q, n->data);

  acquire(&q->lock);

  if(i == 0)
  {
    t->nxt = q->head;
    q->head = t;
    t->state = USING;

    release(&q->lock);

    return;
  }

  struct node *prev = q->head;

  int a;
  for(a = 1; a < i; ++a)
  {
    prev = prev->nxt;
  }

  t->nxt = prev->nxt;
  prev->nxt = t;
  t->state = USING;

  release(&q->lock);
}

void *
delete(struct queue *q, int i)
{
  struct node *h = q->head;

  acquire(&q->lock);

  if(i == 0)
  {
    q->head = h->nxt;
    h->state = OLD;

    release(&q->lock);

    return h->data;
  }

  int a;
  for(a = 1; a < i; ++a)
  {
    h = h->nxt;
  }

  void *dt = h->nxt->data;
  h->nxt->state = OLD;
  h->nxt = h->nxt->nxt;

  release(&q->lock);

  return dt;
}
