/*------------------------------------------------------------------------------
 * File: queue.h
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

#ifndef _queue_h_
#define _queue_h_

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: queueADT
 *
 * Description:
 *   Representerar en kö med objekt i.
 *------------------------------------*/
typedef struct queueCDT *queueADT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: NewQueue()
 * Parameters:
 *   size  Det maximala antalet objekt som kön kan innehålla.
 *
 * Description:
 *   Sakapar en ny kö av den givna storleken och returnerar den.
 *------------------------------------*/
queueADT NewQueue(int size);

/*--------------------------------------
 * Function: FreeQueue()
 * Parameters:
 *   queue  Kön som ska frigöras från minnet.
 *
 * Description:
 *   Frigör den specificerade kön från minnet.
 *------------------------------------*/
void FreeQueue(queueADT queue);

void Enqueue(queueADT queue, void *value);
void *Dequeue(queueADT queue);

bool QueueIsEmpty(queueADT queue);
bool QueueIsFull(queueADT queue);

#endif // _queue_h_
