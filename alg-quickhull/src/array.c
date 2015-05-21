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
 * Type: arrayCDT
 *
 * Description:
 *   Representerar en dynamisk array med objekt i.
 *------------------------------------*/
typedef struct arrayCDT {
    void  *elements;
    int    numElements;
    int    maxElements;
    size_t elementSize;
} arrayCDT;

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

static void DoubleArrayCapacity(arrayADT array) {
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
void *ArrayAdd(arrayADT array, const void *value) {
    if (array->numElements >= array->maxElements)
        DoubleArrayCapacity(array);

    void *dest = (char *)array->elements + (array->numElements * array->elementSize);
    memcpy(dest, value, array->elementSize);

    array->numElements++;
    return dest;
}

/*--------------------------------------
 * Function: ArrayBytes()
 * Parameters:
 *   array  Arrayen vars minnesanvändning ska räknas ut.
 *
 * Description:
 *   Returnerar den specificerade arrayens minnesanvändning, i antal bytes.
 *------------------------------------*/
int ArrayBytes(arrayADT array) {
    return sizeof(arrayCDT) + array->maxElements*array->elementSize;
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
void *ArrayGet(arrayADT array, int index) {
    Assert(0 <= index && index < array->numElements);
    return (char *)array->elements + (index * array->elementSize);
}

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
void *ArrayInsert(arrayADT array, int index, const void *value) {
    Assert(0 <= index && index <= array->numElements);

    if (index == array->numElements)
        return ArrayAdd(array, value);

    if (array->numElements >= array->maxElements)
        DoubleArrayCapacity(array);

    int elementSize = array->elementSize;

    char *base = (char *)array->elements;
    for (int i = array->numElements; i > index; i--) {
        void *src  = base + ((i-1) * elementSize);
        void *dest = base + ( i    * elementSize);
        memcpy(dest, src, elementSize);
    }

    void *dest = base + (index * elementSize);
    memcpy(dest, value, elementSize);

    array->numElements++;
    return dest;
}

/*--------------------------------------
 * Function: ArrayLength()
 * Parameters:
 *   array  Arrayen vars längd ska läsas ut.
 *
 * Description:
 *   Returnerar den specificerade arrayens längd.
 *------------------------------------*/
int ArrayLength(arrayADT array) {
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
void FreeArray(arrayADT array) {
    Assert(array->elements != NULL);

    free(array->elements);
    free(array);
}

/*--------------------------------------
 * Function: NewArray()
 * Parameters:
 *   elementSize  Storleken, i bytes, på arrayens element.
 *
 * Description:
 *   Initierar och allokerar en array.
 *------------------------------------*/
arrayADT NewArray(size_t elementSize) {
    arrayADT array = malloc(sizeof(arrayCDT));

    array->numElements = 0;
    array->maxElements = InitialMaxElements;
    array->elementSize = elementSize;

    array->elements = malloc(array->maxElements * array->elementSize);

    return array;
}
