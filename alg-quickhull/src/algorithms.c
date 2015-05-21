/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "algorithms.h"
#include "array.h"
#include "benchmark.h"
#include "common.h"
#include "debug.h"
#include "math.h"

#include <float.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*****************************************************
 *  _                _        __                     *
 * | |__  _ __ _   _| |_ ___ / _| ___  _ __ ___ ___  *
 * | '_ \| '__| | | | __/ _ \ |_ / _ \| '__/ __/ _ \ *
 * | |_) | |  | |_| | ||  __/  _| (_) | | | (_|  __/ *
 * |_.__/|_|   \__,_|\__\___|_|  \___/|_|  \___\___| *
 *                                                   *
 *****************************************************/

/*--------------------------------------
 * Function: BruteforceHull()
 * Parameters:
 *   ps    Punktuppsättningen för vilken ett hölje ska genereras.
 *   hull  En pekare till höljet.
 *
 * Description:
 *   Genererar att konvext hölje för punktuppsättningen genom uttömmande
 *   sökning. Returnerar information om algoritmens arbete.
 *------------------------------------*/
algorithmdataT BruteforceHull(pointsetT ps, hullT *hull) {
    algorithmdataT algo = { 0 };

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
                algo.numOps++;
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
                if (hull->numLines >= hull->maxLines)
                    Fail();

                hull->lines[hull->numLines].a = a;
                hull->lines[hull->numLines].b = b;
                hull->numLines++;
                break;
            }
        }
    }

    return algo;
}

/*********************************************
 *              _      _    _           _ _  *
 *   __ _ _   _(_) ___| | _| |__  _   _| | | *
 *  / _` | | | | |/ __| |/ / '_ \| | | | | | *
 * | (_| | |_| | | (__|   <| | | | |_| | | | *
 *  \__, |\__,_|_|\___|_|\_\_| |_|\__,_|_|_| *
 *     |_|                                   *
 *********************************************/

/*--------------------------------------
 * Function: QH()
 * Parameters:
 *   hull    Array med höljets punkter.
 *   a       A-punkten i triangeln.
 *   b       B-punkten i triangeln.
 *   subset  Punkterna vi ska hantera.
 *
 * Description:
 *   Hanterar punkter och löser det konvexa höljet med hjälp av den rekursiva
 *   algoritmen quickhull.
 *------------------------------------*/
algorithmdataT QH(arrayT* hull, pointT *a, pointT *b, arrayT *subset) {
    algorithmdataT algo = { 0 };

    int numPoints = ArrayLength(subset);

    /*--------------------------------------------------------------------------
     * 1a. INGA PUNKTER
     *
     *   Om vi inte fick några punkter så är linjen a---b ett segment i höljet,
     *   så vi behöver inte göra något här.
     *------------------------------------------------------------------------*/

    if (numPoints == 0)
        return algo;

    /*--------------------------------------------------------------------------
     * 1b. EN ENDA PUNKT
     *
     *   Om vi bara fick en punkt så är segmenten a---p---b segment i höljet,
     *   där p är den enda punkten vi fått in. Vi lägger in den mellan a och b,
     *   sedan är vi klara.
     *------------------------------------------------------------------------*/

    if (numPoints == 1) {
        int n = ArrayLength(hull);
        for (int i = 0; i < n; i++) {
            pointT *point = *(pointT **)ArrayGet(hull, i);
            if (point == b) {
                ArrayInsert(hull, i, ArrayGet(subset, 0));

                algo.numOps = i;
                return algo;
            }
        }

        // Hit kommer vi aldrig.
        Fail();
    }

    /*--------------------------------------------------------------------------
     * 1c. PUNKTEN LÄNGST BORT
     *
     *   Vi bildar en triangel med a, b och p, där p är den punkt som ligger
     *   längst bort från linjen a---b. Segmenten a---p---b är garanterat delar
     *   av höljet. Punkterna inuti triangeln ignoreras. Sedan hanterar vi
     *   punkter på varsin sida om triangeln rekursivt.
     *------------------------------------------------------------------------*/

    float dMax  = -FLT_MAX;
    int   index = -1;

    for (int i = 0; i < numPoints; i++) {
        pointT *point = *(pointT **)ArrayGet(subset, i);

        float d = (b->x - a->x) * (point->y - a->y)
                - (b->y - a->y) * (point->x - a->x);

        if (d < 0.0f)
            d = -d;

        if (d > dMax) {
            index = i;
            dMax  = d;
        }
    }

    algo.numOps += numPoints;

    // Punkten farPoint är den punkt, av alla punkter i 'subset' som är längst
    // bort från linjen a---b.
    pointT *farPoint = *(pointT **)ArrayGet(subset, index);

    int numHullPoints = ArrayLength(hull);
    for (int i = 0; i < numHullPoints; i++) {
        pointT *point = *(pointT **)ArrayGet(hull, i);
        if (point == b) {
            ArrayInsert(hull, i, &farPoint);
            algo.numOps += i;
            break;
        }
    }

    /*--------------------------------------------------------------------------
     * 2. ANDRA PUNKTER
     *
     *   Vi bildar en triangel med a, b och p, där p är den punkt som ligger
     *   längst bort från linjen a---b. Segmenten a---p---b är garanterat delar
     *   av höljet. Punkterna inuti triangeln ignoreras. Sedan hanterar vi
     *   punkter på varsin sida om triangeln rekursivt.
     *------------------------------------------------------------------------*/

    arrayT subsetA, subsetB;
    InitArray(&subsetA, sizeof(pointT *));
    InitArray(&subsetB, sizeof(pointT *));

    algo.numAllocs += 2;

    for (int i = 0; i < numPoints; i++) {
        pointT *point = *(pointT **)ArrayGet(subset, i);

        float d1 = (farPoint->x - a->x) * (point->y - a->y)
                 - (farPoint->y - a->y) * (point->x - a->x);

        if (d1 < 0.0f)
            ArrayAdd(&subsetA, &point);

        float d2 = (b->x - farPoint->x) * (point->y - farPoint->y)
                 - (b->y - farPoint->y) * (point->x - farPoint->x);

        if (d2 < 0.0f)
            ArrayAdd(&subsetB, &point);
    }

    algo.numOps += numPoints;

    /*--------------------------------------------------------------------------
     * 3. REKURSION
     *
     *   Här hanterar vi de två delmängderna med punkter rekursivt. Detta är
     *   algoritmens kärna. Den ena delmängden innehåller punkter på ena sidan
     *   om triangeln, och den andra innehåller punkterna på andra sidan
     *   triangeln:
     *
     *              farPoint
     *                /\
     *               /  \
     *   (subsetA)  /    \  (subsetB)
     *             /      \
     *            /________\
     *          a            b
     *------------------------------------------------------------------------*/

    algorithmdataT algoA = QH(hull, a       , farPoint, &subsetA);
    algorithmdataT algoB = QH(hull, farPoint, b       , &subsetB);

    algo.numOps    += algoA.numOps         + algoB.numOps        ;
    algo.numAllocs += algoA.numAllocs      + algoB.numAllocs     ;
    algo.numBytes  += algoA.numBytes       + algoB.numBytes      ;
    algo.numBytes  += ArrayBytes(&subsetA) + ArrayBytes(&subsetB);

    /*--------------------------------------------------------------------------
     * 4. DEALLOKERING
     *
     *   Dags att rensa upp minnet.
     *------------------------------------------------------------------------*/

    FreeArray(&subsetA);
    FreeArray(&subsetB);

    return algo;
}

/*--------------------------------------
 * Function: Quickhull()
 * Parameters:
 *   pointset  Punktuppsättningen för vilken ett hölje ska genereras.
 *   hull      En pekare till höljet.
 *
 * Description:
 *   Genererar att konvext hölje för punktuppsättningen med hjälp av algoritmen
 *   quickhull. Returnerar data om algoritmens arbete.
 *------------------------------------*/
algorithmdataT Quickhull(pointsetT pointset, hullT *hull) {
    algorithmdataT algo = { 0 };

    /*--------------------------------------------------------------------------
     * 1. INITIERING AV VARIABLER OCH DATA
     *
     *   Här allokerar vi minne och arrayer/vektorer som vi behöver för att
     *   lagra information om höljet och de 'subset' punkter (två "halvor" av
     *   höljet) som vi ska jobba med.
     *
     *------------------------------------------------------------------------*/

    // Vi initierar både left point och right point till den första punkten i
    // uppsättningen, bara för att inte behöva hantera null-pekare.
    pointT *leftPoint  = &pointset.points[0],
           *rightPoint = leftPoint;

    // Efter att algoritmen är klar kommer hullPoints innehålla alla punkter i
    // höljer i medurs ordning.
    arrayT hullPoints, subsetA, subsetB;
    InitArray(&hullPoints, sizeof(pointT *));
    InitArray(&subsetA   , sizeof(pointT *));
    InitArray(&subsetB   , sizeof(pointT *));

    // Tre allokeringar ovan.
    algo.numOps += 3;

    /*--------------------------------------------------------------------------
     * 2. SÖKNING EFTER EXTREMPUNKTER
     *
     *   Vi letar upp vänstra och högra extrempunkterna. Dessa är garanterat en
     *   del av det konvexa höljet, och läggs därför in i punktlistan omgående.
     *------------------------------------------------------------------------*/

    for (int i = 1; i < pointset.numPoints; i++) {
        pointT *point = &pointset.points[i];

        if (point->x < leftPoint ->x) leftPoint  = point;
        if (point->x > rightPoint->x) rightPoint = point;
    }

    ArrayAdd(&hullPoints, &leftPoint );
    ArrayAdd(&hullPoints, &rightPoint);

    algo.numOps += pointset.numPoints-1;

    /*--------------------------------------------------------------------------
     * 3. UPPDELNING I TVÅ HALVOR
     *
     *   Vi drar en linje mellan extrempunkterna. Punkterna på de två sidorna om
     *   linjen hamnar i varsina punktsamlingar; subsetA och subsetB.
     *------------------------------------------------------------------------*/

    for (int i = 0; i < pointset.numPoints; i++) {
        pointT *point = &pointset.points[i];

        // Vänster och höger extrempunkter har vi redan hanterat.
        if (point==leftPoint || point==rightPoint)
            continue;

        float d = (rightPoint->x - leftPoint->x) * (point->y - leftPoint->y)
                - (rightPoint->y - leftPoint->y) * (point->x - leftPoint->x);

        if (d < 0.0f) ArrayAdd(&subsetA, &point); // Övre "halva."
        else          ArrayAdd(&subsetB, &point); // Nedra "halva."
    }

    algo.numOps += pointset.numPoints;

    /*--------------------------------------------------------------------------
     * 4. REKURSION
     *
     *   Här hanterar vi de två punktsamlingarna rekursivt, vilket är quickhull-
     *   algoritmens rekursiva kärna.
     *------------------------------------------------------------------------*/

    algorithmdataT algoA = QH(&hullPoints, leftPoint , rightPoint, &subsetA);
    algorithmdataT algoB = QH(&hullPoints, rightPoint, leftPoint , &subsetB);

    algo.numOps    += algoA.numOps    +  algoB.numOps   ;
    algo.numAllocs += algoA.numAllocs +  algoB.numAllocs;
    algo.numBytes  += algoA.numBytes  +  algoB.numBytes ;

    /*--------------------------------------------------------------------------
     * 5. IHOPSÄTTNING AV HÖLJE
     *
     *   Vi "monterar ihop" höljet genom att länka ihop punkterna. De är sparade
     *   i rätt ordning i listan, så vi kan bara linjer från en punkt till nästa
     *   tills vi gått ett helt varv.
     *------------------------------------------------------------------------*/

    hull->numLines = 0;
    int numHullPoints = ArrayLength(&hullPoints);
    for (int i = 0; i < numHullPoints; i++) {
        if (hull->numLines >= hull->maxLines)
            Fail();

        hull->lines[i].a = *(pointT **)ArrayGet(&hullPoints, i);
        hull->lines[i].b = *(pointT **)ArrayGet(&hullPoints, (i+1) % numHullPoints);
        hull->numLines++;
    }

    algo.numOps += numHullPoints;

    /*--------------------------------------------------------------------------
     * 6. DEALLOKERING
     *
     *   Här rensar vi upp minnet efter oss.
     *------------------------------------------------------------------------*/

    algo.numBytes += ArrayBytes(&hullPoints)
                  +  ArrayBytes(&subsetA   )
                  +  ArrayBytes(&subsetB   );

    FreeArray(&hullPoints);
    FreeArray(&subsetA);
    FreeArray(&subsetB);

    return algo;
}
