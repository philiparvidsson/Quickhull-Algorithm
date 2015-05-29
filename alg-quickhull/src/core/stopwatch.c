/*------------------------------------------------------------------------------
 * File: stopwatch.c
 * Created: May 28, 2015
 * Last changed: May 28, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Erbjuder funktioner f�r tidsm�tning.
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
 *   id  "Namnet" p� tidtagaruret. Anv�nds f�r att k�ra flera, parallella ur.
 *
 * Description:
 *   Nollst�ller det specificerade tidtagaruret.
 *------------------------------------*/
void ResetStopwatch(int id) {
    ResetStopwatchImpl(id);
}

/*--------------------------------------
 * Function: StopwatchElapsed()
 * Parameters:
 *   id  "Namnet" p� tidtagaruret. Anv�nds f�r att k�ra flera, parallella ur.
 *
 * Description:
 *   Returnerar det antal mikrosekunder som passerat sedan StopwatchReset()-
 *   funktionen anropades f�r samma tidtagarur.
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
 *   Konverterar fr�n sekunder till mikrosekunder.
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
 *   Konverterar fr�n mikrosekunder till sekunder.
 *------------------------------------*/
float MicrosecsToSecs(int microsecs) {
    return (float)microsecs / MicrosecsPerSec;
}
