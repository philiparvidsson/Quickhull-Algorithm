/*------------------------------------------------------------------------------
 * File: akl-toussaint.c
 * Created: May 29, 2015
 * Last changed: May 29, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Funktioner för att tillämpa Akl-Toussaint-heuristik på en uppsättning
 *   punkter. Akl-Toussaint innebär att vi bildar en fyrsiding runt extrem-
 *   punkterna längs x- och y-axlarna och avfärdar de punkter som är inuti den.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "akl-toussaint.h"

#include "core/math.h"
#include "core/collections/array.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

pointsetT AklToussaintHeuristic(pointsetT ps) {
    pointT *   topPoint = &ps.points[0],
           *bottomPoint = topPoint,
           *  leftPoint = topPoint,
           * rightPoint = topPoint;

    for (int i = 0; i < ps.numPoints; i++) {
        pointT *p = &ps.points[i];

        if (p->x <   leftPoint->x)   leftPoint = p;
        if (p->x >  rightPoint->x)  rightPoint = p;
        if (p->y >    topPoint->y)    topPoint = p;
        if (p->y < bottomPoint->y) bottomPoint = p;
    }

    arrayADT points = NewArray(sizeof(pointT *));

    ArrayAdd(points, &topPoint);
    ArrayAdd(points, &bottomPoint);
    ArrayAdd(points, &leftPoint);
    ArrayAdd(points, &rightPoint);

    for (int i = 0; i < ps.numPoints; i++) {
        pointT *p = &ps.points[i];

        if (p ==    topPoint) continue;
        if (p == bottomPoint) continue;
        if (p ==  rightPoint) continue;
        if (p ==   leftPoint) continue;

        float d = (topPoint->x - leftPoint->x) * (p->y - leftPoint->y)
                - (topPoint->y - leftPoint->y) * (p->x - leftPoint->x);
        if (d > 0.0f) {
            ArrayAdd(points, &p);
            continue;
        }

        d = (rightPoint->x - topPoint->x) * (p->y - topPoint->y)
          - (rightPoint->y - topPoint->y) * (p->x - topPoint->x);
        if (d > 0.0f) {
            ArrayAdd(points, &p);
            continue;
        }

        d = (bottomPoint->x - rightPoint->x) * (p->y - rightPoint->y)
          - (bottomPoint->y - rightPoint->y) * (p->x - rightPoint->x);
        if (d > 0.0f) {
            ArrayAdd(points, &p);
            continue;
        }

        d = (leftPoint->x - bottomPoint->x) * (p->y - bottomPoint->y)
          - (leftPoint->y - bottomPoint->y) * (p->x - bottomPoint->x);
        if (d > 0.0f) {
            ArrayAdd(points, &p);
            continue;
        }
    }

    pointsetT athps = CreatePoints(ArrayLength(points));

    for (int i = 0; i < athps.numPoints; i++)
        athps.points[i] = **(pointT **)ArrayGet(points, i);

    FreeArray(points);

    return athps;
}
