#ifndef MATH_H_
#define MATH_H_

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

pointsetT CreatePoints(int n);

void FreePoints(pointsetT ps);

hullT InitHull(pointsetT ps);

void FreeHull(hullT hull);

void BruteforceHull(pointsetT ps, hullT *hull);

void RandomizePoints(pointsetT ps);

#endif // MATH_H_
