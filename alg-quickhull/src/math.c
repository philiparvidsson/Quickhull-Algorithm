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

#include "array.h"
#include "benchmark.h"
#include "common.h"
#include "debug.h"
#include "math.h"

#include <float.h>
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
 *   sökning. Returnerar det totala antalet kritiska operationer som utfördes.
 *------------------------------------*/
int BruteforceHull(pointsetT ps, hullT *hull) {
    int numCritOps = 0;

    // For-looparna med i och j används för att konstruera alla tänkbara
    // kombinationer av par bland punkterna. Vi konstruerar dem åt båda håll,
    // d.v.s. både (a,b) och (b,a).

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
                //
                // d<0.0 : Punkten är utanför.
                // d=0.0 : Punkten ligger på linjen.
                // d>0.0 : Punkten är innanför.
                //
                // Dessutom har jag låtit detta utgöra algoritmens kritiska
                // operation, vilket jag hoppas är adekvat.
                numCritOps++;
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
                if (hull->numLines >= hull->maxLines) {
                    // Segmentet får inte plats i höljets datastruktur. Om det
                    // här sker så har vi antingen allokerat ett för litet hölje
                    // eller, om maxLines är detsamma som ps.numPoints, så vet vi
                    // att höljet egentligen borde få plats. Det innebär att vi
                    // håller på att lägga in en dubblett. Det innebär att vi
                    // ändå är klara, så vi kan returnera här.
                    return numCritOps;
                }

                hull->lines[hull->numLines].a = a;
                hull->lines[hull->numLines].b = b;
                hull->numLines++;
                break;
            }
        }
    }

    return numCritOps;
}

int Quickhull2(arrayT* hull, pointT *a, pointT *b, arrayT *subset) {

    int n = ArrayLength(subset);

    if (n == 0)
        return 0;

    if (n == 1) {
        ArrayAdd(hull, ArrayGet(subset, 0));
        return 1;
        for (int i = 0; i < ArrayLength(hull); i++) {
            if (*(pointT **)ArrayGet(hull, i) == a) {
                ArrayInsert(hull, i, ArrayGet(subset, 0));
                return i;
            }
        }

        Fail();
    }

    int numOps = 0;

    int   index = 0;
    float max   = -FLT_MAX;

    for (int i = 0; i < n; i++) {
        pointT *p = *(pointT **)ArrayGet(subset, i);

        float d = (b->x - a->x) * (p->y - a->y)
                - (b->y - a->y) * (p->x - a->x);

        if (d < 0.0f)
            d = -d;

        if (d > max) {
            index = i;
            max   = d;
        }

        numOps++;
    }

    pointT *p = *(pointT **)ArrayGet(subset, index);

    arrayT subsetA, subsetB;
    InitArray(&subsetA, sizeof(pointT *));
    InitArray(&subsetB, sizeof(pointT *));

    for (int i = 0; i < n; i++) {
        pointT *q = *(pointT **)ArrayGet(subset, i);

        float d1 = (p->x - a->x) * (q->y - a->y)
                 - (p->y - a->y) * (q->x - a->x);

        if (d1 > 0.0f) {
            ArrayAdd(&subsetA, &q);
        }

        float d2 = (b->x - p->x) * (q->y - p->y)
                 - (b->y - p->y) * (q->x - p->x);

        if (d2 > 0.0f) {
            ArrayAdd(&subsetA, &q);
        }

        numOps++;
    }

    numOps += Quickhull2(hull, a, p, &subsetA)
           +  Quickhull2(hull, p, b, &subsetB);

    FreeArray(&subsetA);
    FreeArray(&subsetB);

    return numOps;
}

/*--------------------------------------
 * Function: Quickhull()
 * Parameters:
 *   ps    Punktuppsättningen för vilken ett hölje ska genereras.
 *   hull  En pekare till höljet.
 *
 * Description:
 *   Genererar att konvext hölje för punktuppsättningen med hjälp av algoritmen
 *   quickhull. Returnerar det totala antalet kritiska operationer som utfördes.
 *------------------------------------*/
int Quickhull(pointsetT ps, hullT *hull) {
    /*--------------------------------------------------------------------------
     * 1. INITIERING AV VARIABLER OCH DATA
     *
     *   Här allokerar vi minne och arrayer/vektorer som vi behöver för att
     *   lagra information om höljet och de 'subset' punkter (två "halvor" av
     *   höljet) som vi ska jobba med.
     *
     *------------------------------------------------------------------------*/

    // Vi initierar både lp (left point) och rp (right point) till den första
    // punkten i uppsättningen, bara för att inte behöva hantera null-pekare.
    pointT *lp = &ps.points[0],
           *rp = lp;

    // Efter att algoritmen är klar kommer hullPoints innehålla alla punkter i
    // höljer i medurs ordning.
    arrayT hullPoints, subsetA, subsetB;
    InitArray(&hullPoints, sizeof(pointT *));
    InitArray(&subsetA   , sizeof(pointT *));
    InitArray(&subsetB   , sizeof(pointT *));

    /*--------------------------------------------------------------------------
     * 2. SÖKNING EFTER EXTREMPUNKTER
     *
     *   Vi letar upp vänstra och högra extrempunkterna. Dessa är garanterat en
     *   del av det konvexa höljet, och läggs därför in i punktlistan omgående.
     *
     *------------------------------------------------------------------------*/

    for (int i = 1; i < ps.numPoints; i++) {
        pointT *p = &ps.points[i];

        if (p->x < lp->x) lp = p;
        if (p->x > rp->x) rp = p;
    }

    ArrayAdd(&hullPoints, &lp);
    ArrayAdd(&hullPoints, &rp);

    /*--------------------------------------------------------------------------
     * 3. UPPDELNING I TVÅ HALVOR
     *
     *   Vi drar en linje mellan extrempunkterna. Punkterna på de två sidorna om
     *   linjen hamnar i varsina punktsamlingar; subsetA och subsetB.
     *
     *------------------------------------------------------------------------*/

    for (int i = 0; i < ps.numPoints; i++) {
        pointT *p = &ps.points[i];

        float d = (rp->x - lp->x) * (p->y - lp->y)
                - (rp->y - lp->y) * (p->x - lp->x);

        if (d < 0.0f) ArrayAdd(&subsetA, &p); // Övre "halva."
        else          ArrayAdd(&subsetB, &p); // Nedra "halva."
    }

    /*--------------------------------------------------------------------------
     * 4. REKURSION
     *
     *   Här hanterar vi de två punktsamlingarna rekursivt, vilket är quickhull-
     *   algoritmens kärna.
     *
     *------------------------------------------------------------------------*/

    int numOps = ps.numPoints;
               + Quickhull2(&hullPoints, lp, rp, &subsetA)
               + Quickhull2(&hullPoints, rp, lp, &subsetB);

    /*--------------------------------------------------------------------------
     * 5. AVALLOKERING
     *
     *   Här rensar vi upp minnet efter oss.
     *
     *------------------------------------------------------------------------*/

    hull->numLines = 0;
    for (int i = 0; i < ArrayLength(&hullPoints); i++) {
        if (hull->numLines >= hull->maxLines)
            break;

        hull->lines[i].a = *(pointT **)ArrayGet(&hullPoints, i);
        hull->lines[i].b = *(pointT **)ArrayGet(&hullPoints, (i+1) % ArrayLength(&hullPoints));

        hull->numLines++;
    }

    FreeArray(&hullPoints);
    FreeArray(&subsetA);
    FreeArray(&subsetB);

    return 0;
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
