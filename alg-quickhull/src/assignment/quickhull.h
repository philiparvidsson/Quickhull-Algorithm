/*------------------------------------------------------------------------------
 * File: quickhull.h
 * Created: May 21, 2015
 * Last changed: May 21, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Innehåller algoritmen quickhull för att lösa det konvexa höljet.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

#ifndef _quickhull_h_
#define _quickhull_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/math.h"

#include "assignment/algorithmdata.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: Quickhull()
 * Parameters:
 *   ps    Punktuppsättningen för vilken ett hölje ska genereras.
 *   hull  En pekare till höljet.
 *
 * Description:
 *   Genererar att konvext hölje för punktuppsättningen med hjälp av algoritmen
 *   quickhull. Returnerar data om algoritmens arbete.
 *------------------------------------*/
algorithmDataT Quickhull(pointsetT ps, hullT *hull);

#endif // _quickhull_h_
