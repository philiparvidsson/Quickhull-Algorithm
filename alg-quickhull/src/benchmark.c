/*------------------------------------------------------------------------------
 * File: benchmark.c
 * Created: May 16, 2015
 * Last changed: May 16, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Benchmark-funktioner för quickhull.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "benchmark.h"
#include "math.h"

#include <stdio.h>

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: BenchmarkNumIterations
 *
 * Description:
 *   Antal iterationer som ska köras i benchmarkläge.
 *------------------------------------*/
#define BenchmarkNumIterations 1000

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: RunBenchmark()
 * Parameters:
 *   numPoints  Antal punkter att använda vid uträkning av det konvexa
 *              höljet.
 *
 * Description:
 *   Kör programmet i benchmark-läge.
 *------------------------------------*/
void RunBenchmark(int numPoints) {
    printf("Creating random points...");
    pointsetT ps = CreatePoints(numPoints);
    printf(" done.\n");

    printf("Initializing hull...");
    hullT hull = InitHull(ps);
    printf(" done.\n");

    printf("Benchmark will now run.\n");
    printf("Benchmarking...\n");

    for (int i = 1; i <= BenchmarkNumIterations; i++) {
        RandomizePoints(ps);

        // Här räknar vi ut det konvexa höljet.
        BruteforceHull(ps, &hull);

        if ((i % 10)==0)
            printf("%d/%d iterations done...\n", i, BenchmarkNumIterations);
    }

    FreeHull  (hull);
    FreePoints(ps);
}
