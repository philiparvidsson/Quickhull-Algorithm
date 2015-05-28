/*------------------------------------------------------------------------------
 * File: stopwatch.h
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

#ifndef _stopwatch_h_
#define _stopwatch_h_

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
void ResetStopwatch(int id);

/*--------------------------------------
 * Function: StopwatchElapsed()
 * Parameters:
 *   id  "Namnet" på tidtagaruret. Används för att köra flera, parallella ur.
 *
 * Description:
 *   Returnerar det antal mikrosekunder som passerat sedan StopwatchReset()-
 *   funktionen anropades för samma tidtagarur.
 *------------------------------------*/
int StopwatchElapsed(int id);

/*--------------------------------------
 * Function: MicrosecsToSecs()
 * Parameters:
 *   microsecs  Antal mikrosekunder.
 *
 * Description:
 *   Konverterar från mikrosekunder till sekunder.
 *------------------------------------*/
float MicrosecsToSecs(int microsecs);

/*--------------------------------------
 * Function: SecsToMicrosecs()
 * Parameters:
 *   secs  Antal sekunder.
 *
 * Description:
 *   Konverterar från sekunder till mikrosekunder.
 *------------------------------------*/
int SecsToMicrosecs(float secs);

#endif
