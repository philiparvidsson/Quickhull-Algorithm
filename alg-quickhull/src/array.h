/*------------------------------------------------------------------------------
 * File: array.h
 * Created: January 2, 2015
 * Last changed: February 19, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Erbjuder en dynamisk array som växer automatiskt med antalet objekt som
 *   läggs in.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

#ifndef _array_h_
#define _array_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "common.h"
#include "debug.h"

#include <stdlib.h>

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: Array
 *
 * Description:
 *   Representerar en dynamisk array med objekt i.
 *------------------------------------*/
typedef struct {
    void  *elements;
    int    numElements;
    int    maxElements;
    size_t elementSize;
} arrayT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: ArrayAdd()
 * Parameters:
 *   array  Den array till vilken vi ska lägga ett element.
 *   value  Elementet som ska läggas till i arrayen.
 *
 * Description:
 *   Lägger till ett element i en array. Returnerar minnesadressen där noden
 *   lades in.
 *------------------------------------*/
void *ArrayAdd(arrayT *array, const void *value);

/*--------------------------------------
 * Function: ArrayBytes()
 * Parameters:
 *   array  Arrayen vars minnesanvändning ska räknas ut.
 *
 * Description:
 *   Returnerar den specificerade arrayens minnesanvändning, i antal bytes.
 *------------------------------------*/
int ArrayBytes(const arrayT *array);

/*--------------------------------------
 * Function: ArrayGet()
 * Parameters:
 *   array  Den array från vilken vi ska läsa ett element.
 *   index  Det index i arrayen från vilket vi ska läsa elementet.
 *
 * Description:
 *   Läser ut och returnerar en pekare till det specificerade elementet i
 *   arrayen.
 *------------------------------------*/
void *ArrayGet(const arrayT *array, int index);

/*--------------------------------------
 * Function: ArrayInsert()
 * Parameters:
 *   array  Den array till vilken vi ska lägga ett element.
 *   index  Det index i arrayen där elementet ska sättas in.
 *   value  Elementet som ska läggas till i arrayen.
 *
 * Description:
 *   Lägger in ett element i en array vid det specificerade indexet. Returnerar
 *   minnesadressen där noden lades in.
 *------------------------------------*/
void *ArrayInsert(arrayT *array, int i, const void *value);

/*--------------------------------------
 * Function: ArrayLength()
 * Parameters:
 *   array  Arrayen vars längd ska läsas ut.
 *
 * Description:
 *   Returnerar den specificerade arrayens längd.
 *------------------------------------*/
int ArrayLength(const arrayT *array);

/*--------------------------------------
 * Function: FreeArray()
 * Parameters:
 *   array  Den array som ska avallokeras.
 *
 * Description:
 *   Släpper en array ur minnet.
 *------------------------------------*/
void FreeArray(arrayT *array);

/*--------------------------------------
 * Function: InitArray()
 * Parameters:
 *   array  Den array som ska initieras.
 *
 * Description:
 *   Initialiserar och allokerar en array.
 *------------------------------------*/
void InitArray(arrayT *array, size_t elementSize);

#endif // _array_h_
