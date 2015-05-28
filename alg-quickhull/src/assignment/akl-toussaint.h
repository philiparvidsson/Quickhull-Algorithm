/*------------------------------------------------------------------------------
 * File: akl-toussaint.h
 * Created: May 29, 2015
 * Last changed: May 29, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Funktioner f�r att till�mpa Akl-Toussaint-heuristik p� en upps�ttning
 *   punkter. Akl-Toussaint inneb�r att vi bildar en fyrsiding runt extrem-
 *   punkterna l�ngs x- och y-axlarna och avf�rdar de punkter som �r inuti den.
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
