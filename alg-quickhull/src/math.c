/*------------------------------------------------------------------------------
 * File: math.c
 * Created: May 15, 2015
 * Last changed: May 15, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Matematikbibliotek för lösning av olika matematiska problem.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "common.h"
#include "math.h"

#include <stdlib.h>
#include <time.h>

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
pointsetT CreatePoints(int n) {
    pointsetT ps;

    ps.points    = malloc(sizeof(pointT) * n);
    ps.numPoints = n;

    return ps;
}

/*--------------------------------------
 * Function: FreePoints()
 * Parameters:
 *   ps  Den uppsättning punkter som ska avallokeras.
 *
 * Description:
 *   Avallokerar en uppsättning punkter,
 *------------------------------------*/
void FreePoints(pointsetT ps) {
    free(ps.points);
    ps.numPoints = 0;
}

/*--------------------------------------
 * Function: InitHull()
 * Parameters:
 *   ps  Den uppsättning punkter som ska användas till att initiera höljet.
 *
 * Description:
 *   Initierar ett nytt hölje.
 *------------------------------------*/
hullT InitHull(pointsetT ps) {
    hullT hull;

    hull.lines    = malloc(sizeof(lineT) * ps.numPoints);
    hull.numLines = 0;
    hull.maxLines = ps.numPoints;

    return hull;
}

/*--------------------------------------
 * Function: FreeHull()
 * Parameters:
 *   hull Det hölje som ska avallokeras.
 *
 * Description:
 *   Avallokerar ett hölje.
 *------------------------------------*/
void FreeHull(hullT hull) {
    free(hull.lines);
    hull.maxLines = 0;
    hull.numLines = 0;
}

/*--------------------------------------
 * Function: BruteforceHull()
 * Parameters:
 *   ps    Punktuppsättningen för vilken ett hölje ska genereras.
 *   hull  En pekare till höljet.
 *
 * Description:
 *   Genererar att konvext hölje för punktuppsättningen genom uttömmande
 *   sökning.
 *------------------------------------*/
void BruteforceHull(pointsetT ps, hullT *hull) {
    // For-looparna med i och j används för att konstruera alla tänkbara
    // kombinationer av par bland punkterna.

    hull->numLines = 0;
    for (int i = 0; i < ps.numPoints; i++) {
        for (int j = (i+1); j < (i+ps.numPoints); j++) {
            pointT *a, *b, *c;

            // For-loopen med k används för att kolla att alla punkter ligger på
            // rätt sida av linjen mellan punkt i och j.

            bool outside = FALSE;
            for (int k = 0; k < ps.numPoints; k++) {
                // Linjen a---b är (potentiellt) ett segment i höljet. Vi avgör
                // det genom att se vilken sida om linjen som punkten c ligger
                // på.
                a = &ps.points[i];
                b = &ps.points[j % ps.numPoints];
                c = &ps.points[k];

                // Nedan avgör vi vilken sida om linjen punkten ligger på.
                // d<0.0 : Punkten är utanför.
                // d=0.0 : Punkten ligger på linjen.
                // d>0.0 : Punkten är innanför.
                float d = (b->x - a->x) * (c->y - a->y)
                        - (b->y - a->y) * (c->x - a->x);

                if (d < 0.0f) {
                    outside = TRUE;
                    break;
                }
            }

            // Om alla punkter visade sig vara innanför linjen, så behåller vi
            // den som ett segment i det konvexa höljet.
            if (!outside) {
                hull->lines[hull->numLines].a = a;
                hull->lines[hull->numLines].b = b;
                hull->numLines++;

                if (hull->numLines >= hull->maxLines)
                    return;
            }
        }
    }

    // -
}

/*--------------------------------------
 * Function: Quickhull()
 * Parameters:
 *   ps    Punktuppsättningen för vilken ett hölje ska genereras.
 *   hull  En pekare till höljet.
 *
 * Description:
 *   Genererar att konvext hölje för punktuppsättningen med hjälp av algoritmen
 *   Quickhull.
 *------------------------------------*/
void Quickhull(pointsetT ps, hullT *hull) {
    BruteforceHull(ps, hull);
    /*pointT *a = &ps.points[0],
           *b = a;

    for (int i = 1; i < ps.numPoints; i++) {
        pointT *p = &ps.points[i];

        if (p->x < a->x) a = p;
        if (p->x > b->x) b = p;
    }*/
}

/*--------------------------------------
 * Function: RandomizePoints()
 * Parameters:
 *   ps    Punktuppsättningen vars positioner ska slumpas.
 *
 * Description:
 *   Slumpar positionerna för punkterna i den specificerade punktuppsättningen.
 *------------------------------------*/
void RandomizePoints(pointsetT ps) {
    for (int i = 0; i < ps.numPoints; i++) {
        // Positionskomponenterna slumpas i intervallet [-0.5, 0.5).
        ps.points[i].x = (float)rand()/RAND_MAX - 0.5f;
        ps.points[i].y = (float)rand()/RAND_MAX - 0.5f;
    }
}

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
pointT Reflect(pointT d, pointT n) {
    float dp = 2.0f * (d.x*n.x + d.y*n.y);
    
    pointT p = {
        d.x - dp * n.x,
        d.y - dp * n.y
    };

    return p;
}
