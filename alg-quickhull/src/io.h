#ifndef IO_H_
#define IO_H_

//------------------------------------------------
// INCLUDES
//------------------------------------------------

#include "common.h"

//------------------------------------------------
// FUNCTIONS
//------------------------------------------------

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

#endif // IO_H_
