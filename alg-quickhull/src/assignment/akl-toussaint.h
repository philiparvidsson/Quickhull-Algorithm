/*------------------------------------------------------------------------------
 * File: akl-toussaint.h
 * Created: May 29, 2015
 * Last changed: May 29, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Funktioner för att tillämpa Akl-Toussaint-heuristik på en uppsättning
 *   punkter. Akl-Toussaint innebär att vi bildar en fyrsiding runt extrem-
 *   punkterna längs x- och y-axlarna och avfärdar de punkter som är inuti den.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/


#ifndef _akl_toussaint_h_
#define _akl_toussaint_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/math.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: AklToussaintHeuristic()
 * Parameters:
 *   ps  Punktuppsättningen som Akl-Toussaint-heuristiken ska tillämpas på.
 *
 * Description:
 *   Tillämpar Akl-Toussaint-heuristik på den specificerade punktuppsättningen
 *   och returnerar den som en ny punkuppsättning. Glöm inte att anropa
 *   FreePoints()-funktionen och deallokera den nya punktuppsättningen.
 *------------------------------------*/
pointsetT AklToussaintHeuristic(pointsetT ps);

#endif
