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
#include <string.h> // memcpy(), memmove()

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
    void  *data;
    int    numElems;
    int    maxElems;
    size_t elemSize;
} arrayCDT;

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: InitialMaxElems
 *
 * Description:
 *   Det initiala antalet element som minne ska allokeras för i Array_Init().
 *------------------------------------*/
#define InitialMaxElems 8 // Åtta är nog lagom.

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: DoubleArrayCapacity()
 * Parameters:
 *   a  Arrayen vars kapacitet ska dubblas.
 *
 * Description:
 *   Dubblar en arrays kapacitet genom att omallokera den och kopiera över
 *   de gamla elementen.
 *------------------------------------*/
static void DoubleArrayCapacity(arrayADT a) {
    // Vi dubblar kapaciteten och kopierar över de gamla elementen till den nya
    // minnesplatsen, sen släpper vi den gamla arrayen ur minnet.

    void *oldData     = a->data;
    int   oldMaxElems = a->maxElems;

    a->maxElems *= 2;
    a->data      = malloc(a->maxElems*a->elemSize);

    memcpy(a->data, oldData, oldMaxElems*a->elemSize);

    free(oldData);
}

/*--------------------------------------
 * Function: ArrayAdd()
 * Parameters:
 *   a      Den array till vilken vi ska lägga ett element.
 *   value  Elementet som ska läggas till i arrayen.
 *
 * Description:
 *   Lägger till ett element i en array. Returnerar minnesadressen där noden
 *   lades in.
 *------------------------------------*/
void *ArrayAdd(arrayADT a, const void *value) {
    if (a->numElems >= a->maxElems)
        DoubleArrayCapacity(a);

    void *dest = (char *)a->data + (a->numElems*a->elemSize);
    memcpy(dest, value, a->elemSize);

    a->numElems++;
    return dest;
}

/*--------------------------------------
 * Function: ArrayBytes()
 * Parameters:
 *   a  Arrayen vars minnesanvändning ska räknas ut.
 *
 * Description:
 *   Returnerar den specificerade arrayens minnesanvändning, i antal bytes.
 *------------------------------------*/
int ArrayBytes(arrayADT a) {
    return sizeof(arrayCDT) + a->maxElems*a->elemSize;
}

/*--------------------------------------
 * Function: ArrayGet()
 * Parameters:
 *   a      Den array från vilken vi ska läsa ett element.
 *   i      Det index i arrayen från vilket vi ska läsa elementet.
 *
 * Description:
 *   Läser ut och returnerar en pekare till det specificerade elementet i
 *   arrayen.
 *------------------------------------*/
void *ArrayGet(arrayADT a, int index) {
    Assert(0 <= index && index < a->numElems);
    return (char *)a->data + (index*a->elemSize);
}

/*--------------------------------------
 * Function: ArrayInsert()
 * Parameters:
 *   a      Den array till vilken vi ska lägga ett element.
 *   index  Det index i arrayen där elementet ska sättas in.
 *   value  Elementet som ska läggas till i arrayen.
 *
 * Description:
 *   Lägger in ett element i en array vid det specificerade indexet. Returnerar
 *   minnesadressen där noden lades in.
 *------------------------------------*/
void *ArrayInsert(arrayADT a, int index, const void *value) {
    Assert(0 <= index && index <= a->numElems);

    if (index == a->numElems)
        return ArrayAdd(a, value);

    if (a->numElems >= a->maxElems)
        DoubleArrayCapacity(a);
    
    char *basePtr  = a->data;
    int   elemSize = a->elemSize;
    char *dest     = basePtr + index*elemSize;

    memmove(dest+elemSize, dest , (a->numElems - index)*elemSize);
    memcpy (dest         , value, elemSize);

    a->numElems++;
    return dest;
}

/*--------------------------------------
 * Function: ArrayLength()
 * Parameters:
 *   a  Arrayen vars längd ska läsas ut.
 *
 * Description:
 *   Returnerar den specificerade arrayens längd.
 *------------------------------------*/
int ArrayLength(arrayADT a) {
    return a->numElems;
}

/*--------------------------------------
 * Function: FreeArray()
 * Parameters:
 *   a  Den array som ska avallokeras.
 *
 * Description:
 *   Släpper en array ur minnet.
 *------------------------------------*/
void FreeArray(arrayADT a) {
    Assert(a->data != NULL);

    free(a->data);
    free(a);
}

/*--------------------------------------
 * Function: NewArray()
 * Parameters:
 *   elemSize  Storleken, i bytes, på arrayens element.
 *
 * Description:
 *   Initierar och allokerar en array.
 *------------------------------------*/
arrayADT NewArray(size_t elemSize) {
    arrayADT array = malloc(sizeof(arrayCDT));

    array->numElems = 0;
    array->maxElems = InitialMaxElems;
    array->elemSize = elemSize;

    array->data = malloc(array->maxElems * array->elemSize);

    return array;
}

/*--------------------------------------
 * Function: ResetArray()
 * Parameters:
 *   a  Arrayen som ska nollställas.
 *
 * Description:
 *   Nollställer arrayens elementantal till noll. OBS: Nollar inte minnet.
 *------------------------------------*/
void ResetArray(arrayADT array) {
    array->numElems = 0;
}
