/*------------------------------------------------------------------------------
 * File: stopwatch_win32.c
 * Created: May 28, 2015
 * Last changed: May 28, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Win32-implementation av tidtagarur.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

#ifdef _WIN32

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/stopwatch.h"

#include <windows.h>

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
LARGE_INTEGER stopwatches[NumStopwatches];

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
    LARGE_INTEGER *stopwatch = &stopwatches[id % NumStopwatches];
    QueryPerformanceCounter(stopwatch);
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
    LARGE_INTEGER pc;
    QueryPerformanceCounter(&pc);

    LARGE_INTEGER *stopwatch = &stopwatches[id % NumStopwatches];
    LARGE_INTEGER  freq;

    QueryPerformanceFrequency(&freq);

    pc.QuadPart -= stopwatch->QuadPart;
    pc.QuadPart *= 1000000;
    pc.QuadPart /= freq.QuadPart;

    return (int)pc.QuadPart;
}

#endif // _WIN32
