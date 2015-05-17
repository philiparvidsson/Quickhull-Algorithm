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
#include <time.h>

#include <Windows.h>

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: NumIterations
 *
 * Description:
 *   Antal iterationer som ska köras i benchmarkläge.
 *------------------------------------*/
#define NumIterations 1000

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

LARGE_INTEGER stopwatch;

static void StopwatchStart() {
    QueryPerformanceCounter(&stopwatch);
}

static int StopwatchStop() {
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);

    LARGE_INTEGER pc;
    QueryPerformanceCounter(&pc);

    pc.QuadPart -= stopwatch.QuadPart;
    pc.QuadPart *= 1000000;
    pc.QuadPart /= freq.QuadPart;

    return (int)pc.QuadPart;
}

static benchmarkdataT InitBenchmarkData() {
    benchmarkdataT bmd = { 0 };

    bmd.minCritOps = INT_MAX;
    bmd.maxCritOps = INT_MIN;
    bmd.minTime    = INT_MAX;
    bmd.maxTime    = INT_MIN;

    return bmd;
}

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

    benchmarkdataT bmdbf = InitBenchmarkData();

    clock_t start = clock();
    for (int i = 1; i <= NumIterations; i++) {
        RandomizePoints(ps);

        StopwatchStart();
        int numCritOps = BruteforceHull(ps, &hull);
        int microSecs  = StopwatchStop();

        if (numCritOps < bmdbf.minCritOps) bmdbf.minCritOps = numCritOps;
        if (numCritOps > bmdbf.maxCritOps) bmdbf.maxCritOps = numCritOps;

        if (microSecs < bmdbf.minTime) bmdbf.minTime = microSecs;
        if (microSecs > bmdbf.maxTime) bmdbf.maxTime = microSecs;

        bmdbf.avgCritOps += (float)numCritOps / NumIterations;
        bmdbf.avgTime    += (float)microSecs  / NumIterations;

        int benchmarkTime = 1000 * (clock() - start) / CLOCKS_PER_SEC;
        if (benchmarkTime >= 1000) {
            printf("%2.1f%%...\n", 100.0f * (float)i / NumIterations);
            start = clock();
        }
    }

    printf("100.0%%. Done!\n\n");

    printf("%f ops, %f usecs\n", bmdbf.avgCritOps, bmdbf.avgTime);
    FreeHull(hull);
    FreePoints(ps);

    printf("Press ENTER to exit...");
    getchar();
}
