/*------------------------------------------------------------------------------
 * File: benchmark.c
 * Created: May 16, 2015
 * Last changed: May 28, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Benchmark-funktioner f�r algoritmerna.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "benchmark.h"

#include "core/common.h"
#include "core/math.h"
#include "core/stopwatch.h"

#include "assignment/bruteforce.h"
#include "assignment/quickhull.h"

#include <stdio.h>
#include <stdlib.h>

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: AlgorithmStopwatchID
 *
 * Description:
 *   Tidtagaruret som anv�nds f�r att m�ta hur snabbt algoritmer exekveras.
 *------------------------------------*/
#define AlgorithmStopwatchID 61

/*--------------------------------------
 * Constant: BenchmarkStopwatchID
 *
 * Description:
 *   Tidtagaruret som anv�nds f�r att m�ta hur l�ng tid benchmark-l�get k�rts.
 *------------------------------------*/
#define BenchmarkStopwatchID 14

/*--------------------------------------
 * Constant: ProgressStopwatchID
 *
 * Description:
 *   Tidtagaruret som anv�nds f�r att skriva ut hur procent f�r benchmark-l�get
 *   med j�mna mellanrum.
 *------------------------------------*/
#define ProgressStopwatchID 79

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*
 * Type: benchmarkdataT
 *
 * Description:
 *   Inneh�ller data om benchmark-k�rning av en funktion.
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
} benchmarkDataT;

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: NumSeconds
 *
 * Description:
 *   Antal sekunder som ska benchmark ska k�ras.
 *------------------------------------*/
#define NumSeconds 10

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

static benchmarkDataT InitBenchmarkData() {
    benchmarkDataT bmd = { 0 };

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

static void PrintStatistics(string s, benchmarkDataT *bmd) {
    printf("STATISTICS (%s)\n"
           "-----------------------------------------------------------------\n"
           "                         Min.          Max.          Avg.\n"
           " Critical Operations     %-10d    %-10d    %-10d\n"
           " Number of Allocations   %-10d    %-10d    %-10d\n"
           " Memory Used (bytes)     %-10d    %-10d    %-10d\n"
           " Execution Time (\xe6s)     %-10d    %-10d    %-10d\n"
           "-----------------------------------------------------------------\n"
           "\n",
           s, bmd->minOps   , bmd->maxOps   , (int)bmd->avgOps   ,
              bmd->minAllocs, bmd->maxAllocs, (int)bmd->avgAllocs,
              bmd->minBytes , bmd->maxBytes , (int)bmd->avgBytes ,
              bmd->minTime  , bmd->maxTime  , (int)bmd->avgTime  );
}



static void BenchmarkAlgo(pointsetT ps, hullT *hull, benchmarkDataT *bmd,
                          algorithmDataT (*fn)(pointsetT ps, hullT *hull))
{
    ResetStopwatch(AlgorithmStopwatchID);
    algorithmDataT algo = fn(ps, hull);
    int microSecs = StopwatchElapsed(AlgorithmStopwatchID);

    if (algo.numOps    < bmd->minOps   ) bmd->minOps    = algo.numOps   ;
    if (algo.numOps    > bmd->maxOps   ) bmd->maxOps    = algo.numOps   ;
    if (algo.numAllocs < bmd->minAllocs) bmd->minAllocs = algo.numAllocs;
    if (algo.numAllocs > bmd->maxAllocs) bmd->maxAllocs = algo.numAllocs;
    if (algo.numBytes  < bmd->minBytes ) bmd->minBytes  = algo.numBytes ;
    if (algo.numBytes  > bmd->maxBytes ) bmd->maxBytes  = algo.numBytes ;
    if (microSecs      < bmd->minTime  ) bmd->minTime   = microSecs     ;
    if (microSecs      > bmd->maxTime  ) bmd->maxTime   = microSecs     ;

    bmd->avgOps    += algo.numOps   ;
    bmd->avgAllocs += algo.numAllocs;
    bmd->avgBytes  += algo.numBytes ;
    bmd->avgTime   += microSecs     ;
}

/*--------------------------------------
 * Function: RunBenchmark()
 * Parameters:
 *   numPoints  Antal punkter att anv�nda vid utr�kning av det konvexa
 *              h�ljet.
 *
 * Description:
 *   K�r programmet i benchmark-l�ge.
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

    benchmarkDataT bmdbf = InitBenchmarkData(),
                   bmdqh = InitBenchmarkData();

    int numSecs       = 0;
    int numIterations = 0;

    ResetStopwatch(BenchmarkStopwatchID);
    ResetStopwatch(ProgressStopwatchID);
    while (StopwatchElapsed(BenchmarkStopwatchID) < SecsToMicrosecs(NumSeconds)) {
        RandomizePoints(ps);

        BenchmarkAlgo(ps,&hull,&bmdbf, BruteforceHull );
        BenchmarkAlgo(ps,&hull,&bmdqh, Quickhull      );

        numIterations++;

        // H�r ser vi till att skriva ut hur l�ngt i benchmarket vi kommit,
        // procentuellt sett, en g�ng varje sekund. S� att anv�ndaren inte tror
        // att programmet h�ngt sig.
        if (StopwatchElapsed(ProgressStopwatchID) >= SecsToMicrosecs(1.0f)) {
            float p = (float)StopwatchElapsed(BenchmarkStopwatchID)
                    /        SecsToMicrosecs (NumSeconds);

            if (p < 1.0f) {
                printf("%2.1f%%...\n", 100.0f*p);
                ResetStopwatch(ProgressStopwatchID);
            }
        }
    }

    FreeHull  (hull);
    FreePoints(ps);

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
    PrintStatistics("Quickhull" , &bmdqh);

    printf("\nPress ENTER to exit...");
    getchar();
}
