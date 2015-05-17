/*------------------------------------------------------------------------------
 * File: benchmark.h
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

#ifndef Benchmark_h
#define Benchmark_h

typedef struct {
    int   minCritOps;
    int   maxCritOps;
    float avgCritOps;

    int   minTime;
    int   maxTime;
    float avgTime;
} benchmarkdataT;

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
void RunBenchmark(int numPoints);

#endif // Benchmark_h
