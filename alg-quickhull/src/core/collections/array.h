/*------------------------------------------------------------------------------
 * File: array.h
 * Created: January 2, 2015
 * Last changed: February 19, 2015
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

#ifndef _array_h_
#define _array_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"
#include "core/debug.h"

#include <stdlib.h>

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: arrayADT
 *
 * Description:
 *   Representerar en dynamisk array med objekt i.
 *------------------------------------*/
typedef struct arrayCDT *arrayADT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

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
void *ArrayAdd(arrayADT a, const void *value);

/*--------------------------------------
 * Function: ArrayBytes()
 * Parameters:
 *   a  Arrayen vars minnesanv�ndning ska r�knas ut.
 *
 * Description:
 *   Returnerar den specificerade arrayens minnesanv�ndning, i antal bytes.
 *------------------------------------*/
int ArrayBytes(arrayADT a);

/*--------------------------------------
 * Function: ArrayGet()
 * Parameters:
 *   a      Den array fr�n vilken vi ska l�sa ett element.
 *   index  Det index i arrayen fr�n vilket vi ska l�sa elementet.
 *
 * Description:
 *   L�ser ut och returnerar en pekare till det specificerade elementet i
 *   arrayen.
 *------------------------------------*/
void *ArrayGet(arrayADT a, int index);

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
void *ArrayInsert(arrayADT a, int i, const void *value);

/*--------------------------------------
 * Function: ArrayLength()
 * Parameters:
 *   a  Arrayen vars l�ngd ska l�sas ut.
 *
 * Description:
 *   Returnerar den specificerade arrayens l�ngd.
 *------------------------------------*/
int ArrayLength(arrayADT a);

/*--------------------------------------
 * Function: FreeArray()
 * Parameters:
 *   a  Den array som ska avallokeras.
 *
 * Description:
 *   Sl�pper en array ur minnet.
 *------------------------------------*/
void FreeArray(arrayADT a);

/*--------------------------------------
 * Function: NewArray()
 * Parameters:
 *   elementSize  Storleken, i bytes, p� arrayens element.
 *
 * Description:
 *   Initierar och allokerar en array.
 *------------------------------------*/
arrayADT NewArray(size_t elementSize);

/*--------------------------------------
 * Function: ResetArray()
 * Parameters:
 *   a  Arrayen som ska nollst�llas.
 *
 * Description:
 *   Nollst�ller arrayens elementantal till noll. OBS: Nollar inte minnet.
 *------------------------------------*/
void ResetArray(arrayADT a);

#endif // _array_h_
