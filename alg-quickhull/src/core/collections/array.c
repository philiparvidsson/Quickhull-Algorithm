/*------------------------------------------------------------------------------
 * File: array.c
 * Created: January 2, 2015
 * Last changed: January 9, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Erbjuder en dynamisk array som v�xer automatiskt med antalet objekt som
 *   l�ggs in.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "array.h"

#include "core/debug.h"

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
 *   Det initiala antalet element som minne ska allokeras f�r i NewArray().
 *------------------------------------*/
#define InitialMaxElems 8 // �tta �r nog lagom.

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: DoubleArrayCapacity()
 * Parameters:
 *   a  Arrayen vars kapacitet ska dubblas.
 *
 * Description:
 *   Dubblar en arrays kapacitet genom att omallokera den och kopiera �ver
 *   de gamla elementen.
 *------------------------------------*/
static void DoubleArrayCapacity(arrayADT a) {
    // Vi dubblar kapaciteten och kopierar �ver de gamla elementen till den nya
    // minnesplatsen, sen sl�pper vi den gamla arrayen ur minnet.

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
 *   a      Den array till vilken vi ska l�gga ett element.
 *   value  Elementet som ska l�ggas till i arrayen.
 *
 * Description:
 *   L�gger till ett element i en array. Returnerar minnesadressen d�r noden
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
 *   a  Arrayen vars minnesanv�ndning ska r�knas ut.
 *
 * Description:
 *   Returnerar den specificerade arrayens minnesanv�ndning, i antal bytes.
 *------------------------------------*/
int ArrayBytes(arrayADT a) {
    return sizeof(arrayCDT) + a->maxElems*a->elemSize;
}

/*--------------------------------------
 * Function: ArrayGet()
 * Parameters:
 *   a      Den array fr�n vilken vi ska l�sa ett element.
 *   i      Det index i arrayen fr�n vilket vi ska l�sa elementet.
 *
 * Description:
 *   L�ser ut och returnerar en pekare till det specificerade elementet i
 *   arrayen.
 *------------------------------------*/
void *ArrayGet(arrayADT a, int index) {
    Assert(0 <= index && index < a->numElems);
    return (char *)a->data + (index*a->elemSize);
}

/*--------------------------------------
 * Function: ArrayInsert()
 * Parameters:
 *   a      Den array till vilken vi ska l�gga ett element.
 *   index  Det index i arrayen d�r elementet ska s�ttas in.
 *   value  Elementet som ska l�ggas till i arrayen.
 *
 * Description:
 *   L�gger in ett element i en array vid det specificerade indexet. Returnerar
 *   minnesadressen d�r noden lades in.
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
 *   a  Arrayen vars l�ngd ska l�sas ut.
 *
 * Description:
 *   Returnerar den specificerade arrayens l�ngd.
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
 *   Sl�pper en array ur minnet.
 *------------------------------------*/
void FreeArray(arrayADT a) {
    Assert(a->data != NULL);

    free(a->data);
    free(a);
}

/*--------------------------------------
 * Function: NewArray()
 * Parameters:
 *   elemSize  Storleken, i bytes, p� arrayens element.
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
 *   a  Arrayen som ska nollst�llas.
 *
 * Description:
 *   Nollst�ller arrayens elementantal till noll. OBS: Nollar inte minnet.
 *------------------------------------*/
void ResetArray(arrayADT array) {
    array->numElems = 0;
}
