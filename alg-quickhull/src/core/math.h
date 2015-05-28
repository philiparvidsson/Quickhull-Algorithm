/*------------------------------------------------------------------------------
 * File: math.h
 * Created: May 15, 2015
 * Last changed: May 21, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Matematikbibliotek för lösning av olika matematiska problem.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

#ifndef _math_h_
#define _math_h_

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*
 * Type: pointT
 *
 * Description:
 *   Representerar en punkt i euklidisk 2-dimensionell rymd.
 */
typedef struct {
    float x, y;
} pointT;

/*
 * Type: pointsetT
 *
 * Description:
 *   Representerar en uppsättning punkter.
 */
typedef struct {
    pointT* points;
    int     numPoints;
} pointsetT;

/*
 * Type: lineT
 *
 * Description:
 *   Representerar en linje mellan två punkter.
 */
typedef struct {
    pointT *a, *b;
} lineT;

/*
 * Type: hullT
 *
 * Description:
 *   Representerar ett konvext hölje.
 */
typedef struct {
    lineT *lines;
    int    numLines;
    int    maxLines;
} hullT;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: CreatePoints()
 * Parameters:
 *   n  Antalet punkter att skapa i uppsättningen.
 *
 * Description:
 *   Skapar en uppsättning punkter.
 *------------------------------------*/
pointsetT CreatePoints(int n);

/*--------------------------------------
 * Function: FreePoints()
 * Parameters:
 *   ps  Den uppsättning punkter som ska avallokeras.
 *
 * Description:
 *   Avallokerar en uppsättning punkter,
 *------------------------------------*/
void FreePoints(pointsetT ps);

/*--------------------------------------
 * Function: InitHull()
 * Parameters:
 *   ps  Den uppsättning punkter som ska användas till att initiera höljet.
 *
 * Description:
 *   Initierar ett nytt hölje.
 *------------------------------------*/
hullT InitHull(pointsetT ps);

/*--------------------------------------
 * Function: FreeHull()
 * Parameters:
 *   hull Det hölje som ska avallokeras.
 *
 * Description:
 *   Avallokerar ett hölje.
 *------------------------------------*/
void FreeHull(hullT hull);

/*--------------------------------------
 * Function: RandomizePoints()
 * Parameters:
 *   ps    Punktuppsättningen vars positioner ska slumpas.
 *
 * Description:
 *   Slumpar positionerna för punkterna i den specificerade punktuppsättningen.
 *------------------------------------*/
void RandomizePoints(pointsetT ps);

/*--------------------------------------
 * Function: Reflect()
 * Parameters:
 *   d  Riktningsvektorn.
 *   n  Normalvektorn som riktningsvektorn ska reflekteras mot.
 *
 * Description:
 *   Reflekterar en riktningsvektor mot en normalvektor och returnerar
 *   resultatet.
 *------------------------------------*/
pointT Reflect(pointT d, pointT n);

#endif // _math_h_
