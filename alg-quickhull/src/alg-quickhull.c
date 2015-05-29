/*------------------------------------------------------------------------------
 * File: alg-quickhull.c
 * Created: May 15, 2015
 * Last changed: May 26, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Huvudprogram f�r l�sning av det konvexa h�lja med hj�lpa av algoritmen
 *   quickhull.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"
#include "core/io/input.h"

#include "assignment/benchmark.h"

#include "extra/sandbox.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: MaxPoints
 *
 * Description:
 *   Maximalt antal punkter.
 *------------------------------------*/
#define MaxPoints 10000

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: PrintIntroMessage()
 * Parameters:
 *
 * Description:
 *   Skriver ut introduktionsmeddelandet.
 *------------------------------------*/
static void PrintIntroMessage() {
    printf("Quickhull Sandbox v%s by %s\n"
           "\n"
           " - The most awesome Quickhull sandbox/benchmark ever made!\n\n",
           ProgramVersion, ProgramAuthors);
}

/*--------------------------------------
 * Function: main()
 * Parameters:
 *
 * Description:
 *   Programmets huvudfunktion.
 *------------------------------------*/
main() {
    PrintIntroMessage();

    printf("Number of points to use? ");
    int numPoints = GetIntFromUser();
    if (numPoints < 3        ) numPoints = 3;
    if (numPoints > MaxPoints) numPoints = MaxPoints;

    printf("Do you want to run the benchmark? (y/N) ");
    bool benchmark = GetBoolFromUser(FALSE);

    printf("\n");

#ifndef _DEBUG
    // Vi slumpar inte "p� riktigt" i debugl�ge.
    srand((unsigned int)time(NULL));
#endif

    if (benchmark) RunBenchmark(numPoints);
    else           RunSandbox  (numPoints);
}
