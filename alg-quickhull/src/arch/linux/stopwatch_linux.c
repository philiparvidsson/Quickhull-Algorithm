/*------------------------------------------------------------------------------
 * File: stopwatch_linux.c
 * Created: September 30, 2015
 * Last changed: September 30, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Linux-implementation av tidtagarur.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

#ifdef __linux__

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/stopwatch.h"

#include <stdlib.h>

#include <sys/time.h>

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: NumStopwatches
 *
 * Description:
 *   Antalet tidtagarur.
 *------------------------------------*/
#define NumStopwatches 4096

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

/*--------------------------------------
 * Variable: stopwatches
 *
 * Description:
 *   Tidtagaruren.
 *------------------------------------*/
unsigned long long stopwatches[NumStopwatches];

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: ResetStopwatchImpl()
 * Parameters:
 *   id  "Namnet" på tidtagaruret. Används för att köra flera, parallella ur.
 *
 * Description:
 *   Nollställer det specificerade tidtagaruret.
 *------------------------------------*/
void ResetStopwatchImpl(int id) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    stopwatches[id] = (tv.tv_sec*1000000) + tv.tv_usec;
}

/*--------------------------------------
 * Function: StopwatchElapsedImpl()
 * Parameters:
 *   id  "Namnet" på tidtagaruret. Används för att köra flera, parallella ur.
 *
 * Description:
 *   Returnerar det antal mikrosekunder som passerat sedan StopwatchReset()-
 *   funktionen anropades för samma tidtagarur.
 *------------------------------------*/
int StopwatchElapsedImpl(int id) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned long long ts = (tv.tv_sec*1000000) + tv.tv_usec;
    ts -= stopwatches[id];

    return (ts);
}

#endif // __linux__
