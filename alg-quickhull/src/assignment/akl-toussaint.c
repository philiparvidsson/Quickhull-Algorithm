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

#include "core/debug.h"
#include "core/math.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: AklToussaintHeuristic()
 * Parameters:
 *   ps  Punktuppsättningen som Akl-Toussaint-heuristiken ska tillämpas på.
 *
 * Description:
 *   Tillämpar Akl-Toussaint-heuristik på den specificerade punktuppsättningen
 *   och returnerar den som en ny punkuppsättning. Glöm inte att anropa
 *   FreePoints()-funktionen och deallokera den nya punktuppsättningen.
 *------------------------------------*/
pointsetT AklToussaintHeuristic(pointsetT ps) {
    /* The following simple heuristic is often used as the first step in
     * implementations of convex hull algorithms to improve their performance.
     * It is based on the efficient convex hull algorithm by Selim Akl and
     * G.T.Toussaint, 1978. The idea is to quickly exclude many points that
     * would not be part of the convex hull anyway.This method is based on the
     * following idea.Find the two points with the lowest and highest
     * x-coordinates, and the two points with the lowest and highest
     * y-coordinates. (Each of these operations takes O(n).) These four points
     * form a convex quadrilateral, and all points that lie in this
     * quadrilateral(except for the four initially chosen vertices) are not part
     * of the convex hull.Finding all of these points that lie in this
     * quadrilateral is also O(n), and thus, the entire operation is O(n).
     * Optionally, the points with smallest and largest sums of x- and
     * y-coordinates as well as those with smallest and largest differences of
     * x- and y- coordinates can also be added to the quadrilateral, thus
     * forming an irregular convex octagon, whose insides can be safely
     * discarded.If the points are random variables, then for a wide class of
     * probability density functions, this throw - away pre - processing step
     * will make a convex hull algorithm run in linear expected time, even if
     * the worst - case complexity of the convex hull algorithm is quadratic in
     * n.
     */

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

    pointsetT ps2 = CreatePoints(ps.numPoints);

    ps2.numPoints = 0;

    ps2.points[ps2.numPoints++] =    *topPoint;
    ps2.points[ps2.numPoints++] = *bottomPoint;

    if (leftPoint!=topPoint && leftPoint!=bottomPoint)
        ps2.points[ps2.numPoints++] = *leftPoint;

    if (rightPoint!=topPoint && rightPoint!=bottomPoint)
        ps2.points[ps2.numPoints++] = *rightPoint;

    for (int i = 0; i < ps.numPoints; i++) {
        pointT *p = &ps.points[i];

        if (p ==    topPoint) continue;
        if (p == bottomPoint) continue;
        if (p ==  rightPoint) continue;
        if (p ==   leftPoint) continue;

        float d = (topPoint->x - leftPoint->x) * (p->y - leftPoint->y)
                - (topPoint->y - leftPoint->y) * (p->x - leftPoint->x);
        if (d >= 0.0f) {
            ps2.points[ps2.numPoints++] = *p;
            continue;
        }

        d = (rightPoint->x - topPoint->x) * (p->y - topPoint->y)
          - (rightPoint->y - topPoint->y) * (p->x - topPoint->x);
        if (d >= 0.0f) {
            ps2.points[ps2.numPoints++] = *p;
            continue;
        }

        d = (bottomPoint->x - rightPoint->x) * (p->y - rightPoint->y)
          - (bottomPoint->y - rightPoint->y) * (p->x - rightPoint->x);
        if (d >= 0.0f) {
            ps2.points[ps2.numPoints++] = *p;
            continue;
        }

        d = (leftPoint->x - bottomPoint->x) * (p->y - bottomPoint->y)
          - (leftPoint->y - bottomPoint->y) * (p->x - bottomPoint->x);
        if (d >= 0.0f) {
            ps2.points[ps2.numPoints++] = *p;
            continue;
        }
    }

    return ps2;
}
