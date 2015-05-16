/*------------------------------------------------------------------------------
 * File: alg-quickhull.c
 * Created: May 15, 2015
 * Last changed: May 15, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Huvudprogram för lösning av det konvexa hölja med hjälpa av algoritmen
 *   quickhull.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "benchmark.h"
#include "common.h"
#include "demo.h"
#include "io.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    printf("Quickhull Demo v%s by %s\n"
           "\n"
           " - The most awesome Quickhull demo ever made!\n\n",
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
    if (numPoints < 1)    numPoints = 1;
    if (numPoints > 1000) numPoints = 1000;

    printf("Do you want to run the benchmark? (y/N) ");
    bool benchmark = GetBoolFromUser(FALSE);

    printf("\n");

#ifndef _DEBUG
    // Vi slumpar inte "på riktigt" i debugläge.
    srand((unsigned int)time(NULL));
#endif

    if (benchmark) RunBenchmark(numPoints);
    else           RunDemo     (numPoints);
}
