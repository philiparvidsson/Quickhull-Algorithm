#include "common.h"
#include "debug.h"
#include "queue.h"

#include <stdlib.h>

typedef struct queueCDT {
    void **data;
    int first, last;
    int size;
} queueCDT;

queueADT NewQueue(int size) {
    queueADT queue = malloc(sizeof(queueCDT));

    queue->data = malloc(sizeof(void *) * size);
    queue->size = size;
    queue->first = 0;
    queue->last = 0;

    return queue;
}

void FreeQueue(queueADT queue) {
    while (!QueueIsEmpty(queue))
        Dequeue(queue);

    free(queue);
}

void Enqueue(queueADT queue, const void *value) {
    queue->last++;
    if (queue->last >= queue->size)
        queue->last = 0;

    queue->data[queue->last] = value;

}

void *Dequeue(queueADT queue) {
    Assert(queue->first != queue->last);

    queue->first++;
    if (queue->first >= queue->size)
        queue->first = 0;

    void *value = queue->data[queue->first];

    return value;
}

bool QueueIsEmpty(queueADT queue) {
    return queue->first == queue->last;
}

bool QueueIsFull(queueADT queue) {
    return (queue->first == (queue->last+1)%queue->size);
}
