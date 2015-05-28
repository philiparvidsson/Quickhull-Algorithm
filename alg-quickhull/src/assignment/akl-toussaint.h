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

pointsetT AklToussaintHeuristic(pointsetT ps);

#endif
