/*------------------------------------------------------------------------------
 * File: benchmark.c
 * Created: May 16, 2015
 * Last changed: May 21, 2015
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

#include "algorithms.h"
#include "benchmark.h"
#include "common.h"
#include "math.h"

#include <stdio.h>
#include <time.h>

#include <Windows.h>

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*
 * Type: benchmarkdataT
 *
 * Description:
 *   Innehåller data om benchmark-körning av en funktion.
 */
typedef struct {
    int   minOps;
    int   maxOps;
    float avgOps;

    int   minAllocs;
    int   maxAllocs;
    float avgAllocs;

    int   minBytes;
    int   maxBytes;
    float avgBytes;

    int   minTime;
    int   maxTime;
    float avgTime;
} benchmarkdataT;

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: NumSeconds
 *
 * Description:
 *   Antal sekunder som ska benchmark ska köras.
 *------------------------------------*/
#define NumSeconds 10

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

LARGE_INTEGER stopwatch;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static benchmarkdataT InitBenchmarkData() {
    benchmarkdataT bmd = { 0 };

    bmd.minOps    = INT_MAX;
    bmd.maxOps    = INT_MIN;
    bmd.minAllocs = INT_MAX;
    bmd.maxAllocs = INT_MIN;
    bmd.minBytes  = INT_MAX;
    bmd.maxBytes  = INT_MIN;
    bmd.minTime   = INT_MAX;
    bmd.maxTime   = INT_MIN;

    return bmd;
}

static void PrintStatistics(string s, benchmarkdataT *bmd) {
    printf("STATISTICS (%s)\n"
           "-----------------------------------------------------------------\n"
           "                         Min.          Max.          Avg.\n"
           " Critical Operations     %-10d    %-10d    %-10d\n"
           " Number of Allocations   %-4d          %-4d          %-4d\n"
           " Memory Used (bytes)     %-7d       %-7d       %-7d\n"
           " Execution Time (usecs)  %-7d       %-7d       %-7d\n"
           "-----------------------------------------------------------------\n"
           "\n",
           s,
           bmd->minOps   , bmd->maxOps   , (int)bmd->avgOps,
           bmd->minAllocs, bmd->maxAllocs, (int)bmd->avgAllocs,
           bmd->minBytes , bmd->maxBytes , (int)bmd->avgBytes,
           bmd->minTime  , bmd->maxTime  , (int)bmd->avgTime);
}

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

    benchmarkdataT bmdbf = InitBenchmarkData(),
                   bmdqh = InitBenchmarkData();

    int numSecs = 0;
    int numIterations = 0;

    clock_t start = clock(), lastUpdate = start;
    while ((clock() - start) < NumSeconds * CLOCKS_PER_SEC) {
        numIterations++;

        RandomizePoints(ps);

        /*--------------------------------------------------------------------*/
        /* Bruteforce                                                         */
        /*--------------------------------------------------------------------*/

        StopwatchStart();
        algorithmdataT bf = BruteforceHull(ps, &hull);
        int microSecs = StopwatchStop();

        if (bf.numOps    < bmdbf.minOps   ) bmdbf.minOps    = bf.numOps   ;
        if (bf.numOps    > bmdbf.maxOps   ) bmdbf.maxOps    = bf.numOps   ;
        if (bf.numAllocs < bmdbf.minAllocs) bmdbf.minAllocs = bf.numAllocs;
        if (bf.numAllocs > bmdbf.maxAllocs) bmdbf.maxAllocs = bf.numAllocs;
        if (bf.numBytes  < bmdbf.minBytes ) bmdbf.minBytes  = bf.numBytes ;
        if (bf.numBytes  > bmdbf.maxBytes ) bmdbf.maxBytes  = bf.numBytes ;
        if (microSecs    < bmdbf.minTime  ) bmdbf.minTime   = microSecs   ;
        if (microSecs    > bmdbf.maxTime  ) bmdbf.maxTime   = microSecs   ;

        bmdbf.avgOps    += bf.numOps   ;
        bmdbf.avgAllocs += bf.numAllocs;
        bmdbf.avgBytes  += bf.numBytes ;
        bmdbf.avgTime   += microSecs   ;

        /*--------------------------------------------------------------------*/
        /* Quickhull                                                          */
        /*--------------------------------------------------------------------*/

        StopwatchStart();
        algorithmdataT qh = Quickhull(ps, &hull);
        microSecs = StopwatchStop();

        if (qh.numOps    < bmdqh.minOps   ) bmdqh.minOps    = qh.numOps   ;
        if (qh.numOps    > bmdqh.maxOps   ) bmdqh.maxOps    = qh.numOps   ;
        if (qh.numAllocs < bmdqh.minAllocs) bmdqh.minAllocs = qh.numAllocs;
        if (qh.numAllocs > bmdqh.maxAllocs) bmdqh.maxAllocs = qh.numAllocs;
        if (qh.numBytes  < bmdqh.minBytes ) bmdqh.minBytes  = qh.numBytes ;
        if (qh.numBytes  > bmdqh.maxBytes ) bmdqh.maxBytes  = qh.numBytes ;
        if (microSecs    < bmdqh.minTime  ) bmdqh.minTime   = microSecs   ;
        if (microSecs    > bmdqh.maxTime  ) bmdqh.maxTime   = microSecs   ;

        bmdqh.avgOps    += qh.numOps   ;
        bmdqh.avgAllocs += qh.numAllocs;
        bmdqh.avgBytes  += qh.numBytes ;
        bmdqh.avgTime   += microSecs   ;

        // Här ser vi till att skriva ut hur långt i benchmarket vi kommit,
        // procentuellt sett, en gång varje sekund. Så att användaren inte tror
        // att programmet hängt sig.
        int benchmarkTime = 1000 * (clock() - lastUpdate) / CLOCKS_PER_SEC;
        if (benchmarkTime >= 1000) {
            printf("%2.1f%%...\n", 100.0f * (float)(clock()-start) / NumSeconds);
            lastUpdate = clock();
        }
    }

    bmdbf.avgOps    /= numIterations;
    bmdbf.avgAllocs /= numIterations;
    bmdbf.avgBytes  /= numIterations;
    bmdbf.avgTime   /= numIterations;

    bmdqh.avgOps    /= numIterations;
    bmdqh.avgAllocs /= numIterations;
    bmdqh.avgBytes  /= numIterations;
    bmdqh.avgTime   /= numIterations;

    printf("100.0%%. Done!\n\n");

    PrintStatistics("Bruteforce", &bmdbf);
    PrintStatistics("Quickhull", &bmdqh);

    FreeHull(hull);
    FreePoints(ps);

    printf("\nPress ENTER to exit...");
    getchar();
}
