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
 *
 *----------------------------------------------------------------------------*/

#ifndef _debug_h_
#define _debug_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"

#include <stdio.h>

/*------------------------------------------------
 * MACROS
 *----------------------------------------------*/

#define Assert(expr) if (!(expr)) {                                 \
                         AssertFail(#expr, __FUNCTION__, __LINE__); \
                     }

#define Error(msg) Crash(msg, __FUNCTION__, __LINE__)


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
 * Function: Crash()
 * Parameters:
 *   msg  Meddelandet som ska visas.
 *   funcName  Namnet på den funktion som programmet kraschade i.
 *   line      Den rad i koden som programmet kraschade på.
 *
 * Description:
 *   Kraschar programmet med ett meddelande.
 *------------------------------------*/
void Crash(string msg, string funcName, int line);

#endif // _debug_h_
