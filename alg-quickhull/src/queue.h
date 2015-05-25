/*------------------------------------------------------------------------------
 * File: queue.h
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
 *   Skapar en ny kö av den givna storleken och returnerar den.
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

/*--------------------------------------
 * Function: Enqueue()
 * Parameters:
 *   queue  Kön till vilket ett värde ska läggas.
 *   value  Värdet som ska läggas till i kön.
 *
 * Description:
 *   Lägger till ett värde i en kö.
 *------------------------------------*/
void Enqueue(queueADT queue, void *value);

/*--------------------------------------
 * Function: Dequeue()
 * Parameters:
 *   queue  Kön från vilket ett värde ska tas.
 *
 * Description:
 *   Tar ut det första värdet i kön och returnerar det.
 *------------------------------------*/
void *Dequeue(queueADT queue);

/*--------------------------------------
 * Function: QueueIsEmpty()
 * Parameters:
 *   queue  Kön som ska kontrolleras.
 *
 * Description:
 *   Returnerar sant om den specificerade kön är tom.
 *------------------------------------*/
bool QueueIsEmpty(queueADT queue);

/*--------------------------------------
 * Function: QueueIsFull()
 * Parameters:
 *   queue  Kön som ska kontrolleras.
 *
 * Description:
 *   Returnerar sant om den specificerade kön är full.
 *------------------------------------*/
bool QueueIsFull(queueADT queue);

#endif // _queue_h_
