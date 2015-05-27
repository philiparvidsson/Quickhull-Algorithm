/*------------------------------------------------------------------------------
 * File: input.h
 * Created: May 15, 2015
 * Last changed: May 15, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Input-funktioner för att läsa in data från användaren.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

#ifndef _input_h_
#define _input_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: GetBoolFromUser()
 * Parameters:
 *   defaultVal  Värdet som ska returneras om användaren inte svarar.
 *
 * Description:
 *   Låter användaren skriva in ja eller nej.
 *------------------------------------*/
bool GetBoolFromUser(bool defaultVal);

/*--------------------------------------
 * Function: GetIntFromUser()
 * Parameters:
 *
 * Description:
 *   Låter användaren skriva in ett heltal.
 *------------------------------------*/
int GetIntFromUser();

/*--------------------------------------
 * Function: GetStringFromUser()
 * Parameters:
 *
 * Description:
 *   Låter användaren skriva in en sträng. Glöm inte anropa free() efteråt för
 *   att förhindra minnesläckage.
 *------------------------------------*/
string GetStringFromUser();

#endif // _input_h_
