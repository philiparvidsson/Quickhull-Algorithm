/*------------------------------------------------------------------------------
 * File: queue.c
 * Created: May 23, 2015
 * Last changed: May 24, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Erbjuder en kö-struktur. Implementationen använder sig av en cirkulär
 *   vektor för att slippa omallokeringar.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "queue.h"

#include "core/common.h"
#include "core/debug.h"

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
    int    head, tail;
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
 *   Skapar en ny kö av den givna storleken.
 *------------------------------------*/
queueADT NewQueue(int size) {
    Assert(size > 0);

    // Vi ökar storleken med ett eftersom vi behöver ett tomt element i vektorn.
    // Anledningen är att vi har en cirkelbuffert. Utan ett extra, tomt element
    // kan vi inte skilja på full och tom kö.
    size++;

    queueADT queue = malloc(sizeof(queueCDT));

    queue->data = malloc(sizeof(void *) * size);
    queue->head = 0;
    queue->tail = 0;
    queue->size = size;

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

/*--------------------------------------
 * Function: Enqueue()
 * Parameters:
 *   queue  Kön till vilket ett värde ska läggas.
 *   value  Värdet som ska läggas till i kön.
 *
 * Description:
 *   Lägger till ett värde i en kö.
 *------------------------------------*/
void Enqueue(queueADT queue, const void *value) {
    Assert(!QueueIsFull(queue));

    queue->data[queue->tail] = value;

    queue->tail++;
    if (queue->tail >= queue->size)
        queue->tail = 0;
}

/*--------------------------------------
 * Function: Dequeue()
 * Parameters:
 *   queue  Kön från vilket ett värde ska tas.
 *
 * Description:
 *   Tar ut det första värdet i kön och returnerar det.
 *------------------------------------*/
void *Dequeue(queueADT queue) {
    Assert(!QueueIsEmpty(queue));

    void *value = queue->data[queue->head];

    queue->head++;
    if (queue->head >= queue->size)
        queue->head = 0;

    return value;
}


/*--------------------------------------
 * Function: QueueIsEmpty()
 * Parameters:
 *   queue  Kön som ska kontrolleras.
 *
 * Description:
 *   Returnerar sant om den specificerade kön är tom.
 *------------------------------------*/
bool QueueIsEmpty(queueADT queue) {
    return (queue->head == queue->tail);
}

/*--------------------------------------
 * Function: QueueIsFull()
 * Parameters:
 *   queue  Kön som ska kontrolleras.
 *
 * Description:
 *   Returnerar sant om den specificerade kön är full.
 *------------------------------------*/
bool QueueIsFull(queueADT queue) {
    return (queue->head == ((queue->tail+1) % queue->size));
}
