/*------------------------------------------------------------------------------
 * File: array.c
 * Created: January 2, 2015
 * Last changed: January 9, 2015
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

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "array.h"
#include "debug.h"

#include <stdlib.h>
#include <string.h> // memcpy()

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: InitialMaxElements
 *
 * Description:
 *   Det initiala antalet element som minne ska allokeras för i Array_Init().
 *------------------------------------*/
#define InitialMaxElements 8 // 8 är nog lagom.

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
void *ArrayAdd(arrayT *array, const void *value) {
    if (array->numElements >= array->maxElements) {
        // Arrayen är full, så vi dubblar kapaciteten och kopierar över de gamla
        // elementen till den nya minnesplatsen, sen släpper vi den gamla
        // arrayen ur minnet.

        void *oldElements    = array->elements;
        int   oldMaxElements = array->maxElements;

        array->maxElements *= 2;
        array->elements = malloc(array->maxElements * array->elementSize);
        memcpy(array->elements, oldElements, oldMaxElements * array->elementSize);
        free(oldElements);
    }

    void *dest = (char *)array->elements + (array->numElements * array->elementSize);
    memcpy(dest, value, array->elementSize);

    array->numElements++;
    return dest;
}

/*--------------------------------------
 * Function: ArrayGet()
 * Parameters:
 *   array  Den array från vilken vi ska läsa ett element.
 *   i      Det index i arrayen från vilket vi ska läsa elementet.
 *
 * Description:
 *   Läser ut och returnerar en pekare till det specificerade elementet i
 *   arrayen.
 *------------------------------------*/
void *ArrayGet(const arrayT *array, int i) {
    Assert(0 <= i && i < array->numElements);
    return (char *)array->elements + (i * array->elementSize);
}

/*--------------------------------------
 * Function: ArrayLength()
 * Parameters:
 *   array  Arrayen vars längd ska läsas ut.
 *
 * Description:
 *   Returnerar den specificerade arrayens längd.
 *------------------------------------*/
int ArrayLength(const arrayT *array) {
    return array->numElements;
}

/*--------------------------------------
 * Function: FreeArray()
 * Parameters:
 *   array  Den array som ska avallokeras.
 *
 * Description:
 *   Släpper en array ur minnet.
 *------------------------------------*/
void FreeArray(arrayT *array) {
    Assert(array->elements != NULL);

    free(array->elements);

    array->elements    = NULL;
    array->numElements = 0;
    array->maxElements = 0;
    array->elementSize = 0;
}

/*--------------------------------------
 * Function: InitArray()
 * Parameters:
 *   array  Den array som ska initieras.
 *
 * Description:
 *   Initialiserar och allokerar en array.
 *------------------------------------*/
void InitArray(arrayT *array, size_t elementSize) {
    array->numElements = 0;
    array->maxElements = InitialMaxElements;
    array->elementSize = elementSize;

    array->elements = malloc(array->maxElements * array->elementSize);
}
