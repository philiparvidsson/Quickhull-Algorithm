/*------------------------------------------------------------------------------
 * File: debug.c
 * Created: January 3, 2015
 * Last changed: January 11, 2015
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
    printf("The assertion '%s' failed in %s() on line %d.\n"
           "This program will now crash.\n"
           "Press ENTER to continue...", expr, funcName, line);
    getchar();
    exit(EXIT_FAILURE);
}

/*--------------------------------------
 * Function: Error()
 * Parameters:
 *   msg  Meddelandet som ska visas.
 *
 * Description:
 *   Kraschar programmet med ett meddelande.
 *------------------------------------*/
void Error(string msg) {
    printf("Error: %s\n", msg);
    getchar();
    exit(EXIT_FAILURE);
}

/*--------------------------------------
 * Function: ProgramFail()
 * Parameters:
 *   funcName  Namnet på den funktion som programmet kraschade i.
 *   line      Den rad i koden som programmet kraschade på.
 *
 * Description:
 *   Kraschar programmet med ett meddelande.
 *------------------------------------*/
void ProgramFail(string funcName, int line) {
    printf("Program failed in %s() on line %d and will now crash.\n"
           "Press ENTER to continue...", funcName, line);
    getchar();
    exit(EXIT_FAILURE);
}
