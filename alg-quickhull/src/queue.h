#ifndef _queue_h_
#define _queue_h_

typedef struct queueCDT *queueADT;

queueADT NewQueue(int size);
void FreeQueue(queueADT queue);

void Enqueue(queueADT queue, void *value);
void *Dequeue(queueADT queue);

bool QueueIsEmpty(queueADT queue);
bool QueueIsFull(queueADT queue);

int QueueLength(queueADT queue);

#endif
