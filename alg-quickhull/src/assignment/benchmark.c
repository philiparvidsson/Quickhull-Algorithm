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

#include "extra/akl-toussaint.h"

#include <stdio.h>
#include <stdlib.h>

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
#define NumSeconds 30

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
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: InitBenchmarkData()
 * Parameters:
 *
 * Description:
 *   Initierar ny benchmark-data.
 *------------------------------------*/
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

/*--------------------------------------
 * Function: BenchmarkAlgo()
 * Parameters:
 *   ps    Punktupps�ttningen.
 *   hull  H�ljet.
 *   bmd   Benchmark-datastruktur.
 *   fn    Algoritmens funktion.
 *
 * Description:
 *   K�r den specificerade algoritmen f�r att r�kna ut det kovnexa h�ljet och
 *   m�ter k�rningen. K�rningsdatan sparas i parametern bmd.
 *------------------------------------*/
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
 * Function: PrintStatistics()
 * Parameters:
 *   s    Namnet p� algoritmen.
 *   bmd  Data fr�n benchmark-k�rning.
 *
 * Description:
 *   Skriver ut benchmark-statistik.
 *------------------------------------*/
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

/*--------------------------------------
 * Function: BruteforceAklToussaint()
 * Parameters:
 *   ps    Punktupps�ttningen f�r vilken ett h�lje ska genereras.
 *   hull  En pekare till h�ljet.
 *
 * Description:
 *   Genererar att konvext h�lje f�r punktupps�ttningen genom utt�mmande
 *   s�kning. Returnerar information om algoritmens arbete.
 *------------------------------------*/
algorithmDataT BruteforceAklToussaint(pointsetT ps, hullT *hull) {
    pointsetT ps2 = AklToussaintHeuristic(ps);
    algorithmDataT algo = BruteforceHull(ps2, hull);
    FreePoints(ps2);
    return algo;
}

/*--------------------------------------
 * Function: RunBenchmark()
 * Parameters:
 *   numPoints  Antal punkter att anv�nda vid utr�kning av det konvexa h�ljet.
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

    benchmarkDataT bf   = InitBenchmarkData(),
                   qh   = InitBenchmarkData(),
                   bfat = InitBenchmarkData();

    int numSecs       = 0;
    int numIterations = 0;

    ResetStopwatch(BenchmarkStopwatchID);
    ResetStopwatch(ProgressStopwatchID);
    while (StopwatchElapsed(BenchmarkStopwatchID) < SecsToMicrosecs(NumSeconds))
    {
        RandomizePoints(ps);

        BenchmarkAlgo(ps,&hull,&bf  , BruteforceHull         );
        BenchmarkAlgo(ps,&hull,&bfat, BruteforceAklToussaint );
        BenchmarkAlgo(ps,&hull,&qh  , Quickhull              );

        numIterations++;

        // H�r ser vi till att skriva ut hur l�ngt i benchmarket vi kommit,
        // procentuellt sett, en g�ng varje sekund. S� att anv�ndaren inte tror
        // att programmet h�ngt sig.
        if (StopwatchElapsed(ProgressStopwatchID) >= SecsToMicrosecs(1.0f)) {
            float p = (float)StopwatchElapsed(BenchmarkStopwatchID)
                    /        SecsToMicrosecs (NumSeconds);

            if (p < 1.0f) {
                printf("\x08\x08\x08\x08\x08\x08\x08\x08%2.1f%%...", 100.0f*p);
                ResetStopwatch(ProgressStopwatchID);
            }
        }
    }

    FreeHull  (hull);
    FreePoints(ps);

    bf.avgOps    /= numIterations;
    bf.avgAllocs /= numIterations;
    bf.avgBytes  /= numIterations;
    bf.avgTime   /= numIterations;

    qh.avgOps    /= numIterations;
    qh.avgAllocs /= numIterations;
    qh.avgBytes  /= numIterations;
    qh.avgTime   /= numIterations;

    bfat.avgOps    /= numIterations;
    bfat.avgAllocs /= numIterations;
    bfat.avgBytes  /= numIterations;
    bfat.avgTime   /= numIterations;

    printf("\x08\x08\x08\x08\x08\x08\x08\x08 100.0%%. Done!\n\n");

    PrintStatistics("Bruteforce"                , &bf);
    PrintStatistics("Bruteforce + Akl-Toussaint", &bfat);
    PrintStatistics("Quickhull"                 , &qh);

    printf("\nPress ENTER to exit...");
    getchar();
}
