/*------------------------------------------------------------------------------
 * File: debug.c
 * Created: January 3, 2015
 * Last changed: September 30, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Makron och funktioner för debugging.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "debug.h"

#include <stdio.h>
#include <stdlib.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: AssertFail()
 * Parameters:
 *   expr      Uttrycket som resulterade i falskt.
 *   funcName  Namnet på den funktion som programmet kraschade i.
 *   line      Den rad i koden som programmet kraschade på.
 *
 * Description:
 *   Kraschar programmet med ett meddelande.
 *------------------------------------*/
void AssertFail(string expr, string funcName, int line) {
    printf("\nERROR: The assertion '%s' failed in %s() on line %d.\n\n"
           "This program will now crash.\n"
           "Press ENTER to continue...", expr, funcName, line);
    getchar();
#ifdef _MSC_VER
    __debugbreak();
#endif // _MSC_VER
    exit(EXIT_FAILURE);
}

/*--------------------------------------
 * Function: Crash()
 * Parameters:
 *   msg       Meddelandet som ska visas.
 *   funcName  Namnet på den funktion som programmet kraschade i.
 *   line      Den rad i koden som programmet kraschade på.
 *
 * Description:
 *   Kraschar programmet med ett meddelande.
 *------------------------------------*/
void Crash(string msg, string funcName, int line) {
    printf("\nERROR: %s in %s() on line %d.\n\n"
           "This program will now crash.\n"
           "Press ENTER to continue...", msg, funcName, line);
    getchar();
#ifdef _MSC_VER
    __debugbreak();
#endif // _MSC_VER
    exit(EXIT_FAILURE);
}
