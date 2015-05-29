/*------------------------------------------------------------------------------
 * File: stopwatch.c
 * Created: May 28, 2015
 * Last changed: May 28, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Erbjuder funktioner för tidsmätning.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "stopwatch.h"

/*------------------------------------------------
 * EXTERNALS
 *----------------------------------------------*/

extern void   ResetStopwatchImpl(int id);
extern int  StopwatchElapsedImpl(int id);

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: MicrosecsPerSec
 *
 * Description:
 *   Antal mikrosekunder per sekund.
 *------------------------------------*/
#define MicrosecsPerSec 1000000

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: ResetStopwatch()
 * Parameters:
 *   id  "Namnet" på tidtagaruret. Används för att köra flera, parallella ur.
 *
 * Description:
 *   Nollställer det specificerade tidtagaruret.
 *------------------------------------*/
void ResetStopwatch(int id) {
    ResetStopwatchImpl(id);
}

/*--------------------------------------
 * Function: StopwatchElapsed()
 * Parameters:
 *   id  "Namnet" på tidtagaruret. Används för att köra flera, parallella ur.
 *
 * Description:
 *   Returnerar det antal mikrosekunder som passerat sedan StopwatchReset()-
 *   funktionen anropades för samma tidtagarur.
 *------------------------------------*/
int StopwatchElapsed(int id) {
    return StopwatchElapsedImpl(id);
}

/*--------------------------------------
 * Function: SecsToMicrosecs()
 * Parameters:
 *   secs  Antal sekunder.
 *
 * Description:
 *   Konverterar från sekunder till mikrosekunder.
 *------------------------------------*/
int SecsToMicrosecs(float secs) {
    return (int)(secs * MicrosecsPerSec);
}

/*--------------------------------------
 * Function: MicrosecsToSecs()
 * Parameters:
 *   microsecs  Antal mikrosekunder.
 *
 * Description:
 *   Konverterar från mikrosekunder till sekunder.
 *------------------------------------*/
float MicrosecsToSecs(int microsecs) {
    return (float)microsecs / MicrosecsPerSec;
}
