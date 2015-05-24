/*------------------------------------------------------------------------------
 * File: queue.c
 * Created: May 23, 2015
 * Last changed: May 24, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Erbjuder en kö-struktur. Implementationen använder sig av en cirkulär
 *   buffer för att slippa omallokeringar.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "common.h"
#include "debug.h"
#include "queue.h"

#include <stdlib.h>

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: queueCDT
 *
 * Description:
 *   Representerar en kö med objekt i.
 *------------------------------------*/
typedef struct queueCDT {
    void **data;
    int    first, last;
    int    size;
} queueCDT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: NewQueue()
 * Parameters:
 *   size  Det maximala antalet objekt som kön kan innehålla.
 *
 * Description:
 *   Sakapar en ny kö av den givna storleken.
 *------------------------------------*/
queueADT NewQueue(int size) {
    Assert(size > 0);

    // Vi ökar storleken med ett eftersom vi behöver ett tomt element i vektorn.
    size++;

    queueADT queue = malloc(sizeof(queueCDT));

    queue->data  = malloc(sizeof(void *) * (size));
    queue->first = 0;
    queue->last  = 0;
    queue->size  = size;

    return queue;
}

/*--------------------------------------
 * Function: FreeQueue()
 * Parameters:
 *   queue  Kön som ska frigöras från minnet.
 *
 * Description:
 *   Frigör den specificerade kön från minnet.
 *------------------------------------*/
void FreeQueue(queueADT queue) {
    while (!QueueIsEmpty(queue))
        Dequeue(queue);

    free(queue);
}

void Enqueue(queueADT queue, const void *value) {
    Assert(!QueueIsFull(queue));

    queue->data[queue->last] = value;

    queue->last++;
    if (queue->last >= queue->size)
        queue->last = 0;
}

void *Dequeue(queueADT queue) {
    Assert(!QueueIsEmpty(queue));

    void *value = queue->data[queue->first];

    queue->first++;
    if (queue->first >= queue->size)
        queue->first = 0;

    return value;
}

bool QueueIsEmpty(queueADT queue) {
    return queue->first==queue->last;
}

bool QueueIsFull(queueADT queue) {
    return queue->first==((queue->last+1) % queue->size);
}
