#ifndef _algorithms_h_
#define _algorithms_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "math.h"

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*
 * Type: algorithmdataT
 *
 * Description:
 *   Innehåller data om en algoritms arbete.
 */
typedef struct {
    int numOps;
    int numAllocs;
    int numBytes;
} algorithmdataT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: BruteforceHull()
 * Parameters:
 *   ps    Punktuppsättningen för vilken ett hölje ska genereras.
 *   hull  En pekare till höljet.
 *
 * Description:
 *   Genererar att konvext hölje för punktuppsättningen genom uttömmande
 *   sökning. Returnerar information om algoritmens arbete.
 *------------------------------------*/
algorithmdataT BruteforceHull(pointsetT ps, hullT *hull);

/*--------------------------------------
 * Function: Quickhull()
 * Parameters:
 *   pointset  Punktuppsättningen för vilken ett hölje ska genereras.
 *   hull      En pekare till höljet.
 *
 * Description:
 *   Genererar att konvext hölje för punktuppsättningen med hjälp av algoritmen
 *   quickhull. Returnerar data om algoritmens arbete.
 *------------------------------------*/
algorithmdataT Quickhull(pointsetT pointset, hullT *hull);

#endif // _algorithms_h_
