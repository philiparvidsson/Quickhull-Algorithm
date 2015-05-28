/*------------------------------------------------------------------------------
 * File: benchmark.h
 * Created: May 16, 2015
 * Last changed: May 21, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Benchmark-funktioner för algoritmerna.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

#ifndef _benchmark_h_
#define _benchmark_h_

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

#endif // _benchmark_h_
