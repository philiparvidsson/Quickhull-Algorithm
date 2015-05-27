/*------------------------------------------------------------------------------
 * File: bruteforce.h
 * Created: May 21, 2015
 * Last changed: May 21, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Innehåller bruteforce-algoritmen för att lösa det konvexa höljet.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

#ifndef _bruteforce_h_
#define _bruteforce_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/math.h"

#include "assignment/algorithmdata.h"

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
algorithmDataT BruteforceHull(pointsetT ps, hullT *hull);

#endif // _bruteforce_h_
