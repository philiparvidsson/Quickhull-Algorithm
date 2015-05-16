/*------------------------------------------------------------------------------
 * File: debug.h
 * Created: January 2, 2015
 * Last changed: January 13, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Makron och funktioner för debugging.
 *
 * Changes:
 *   * Lade till () efter funktionsnamnet som skrivs ut i DEBUG_PRINT.
 *   * Skapade makrot ASSERT med tillhörande funktion.
 *   * Ändrade DEBUG_PRINT()-makrot så att det visar rad i programkoden.
 *----------------------------------------------------------------------------*/

#ifndef Debug_h
#define Debug_h

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "common.h"

#include <stdio.h>

/*------------------------------------------------
 * MACROS
 *----------------------------------------------*/

#define ASSERT(expr) if (!(expr)) {                                 \
                         AssertFail(#expr, __FUNCTION__, __LINE__); \
                     }

#define FAIL() Fail(__FUNCTION__, __LINE__)

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
void AssertFail(string expr, string funcName, int line);

/*--------------------------------------
 * Function: Error()
 * Parameters:
 *   msg  Meddelandet som ska visas.
 *
 * Description:
 *   Kraschar programmet med ett meddelande.
 *------------------------------------*/
void Error(string msg);

/*--------------------------------------
 * Function: Fail()
 * Parameters:
 *   funcName  Namnet på den funktion som programmet kraschade i.
 *   line      Den rad i koden som programmet kraschade på.
 *
 * Description:
 *   Kraschar programmet med ett meddelande.
 *------------------------------------*/
void Fail(string funcName, int line);

#endif // Debug_h
