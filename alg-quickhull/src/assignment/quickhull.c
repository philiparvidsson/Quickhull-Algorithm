/*------------------------------------------------------------------------------
 * File: quickhull.c
 * Created: May 21, 2015
 * Last changed: May 21, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Innehåller algoritmen quickhull för att lösa det konvexa höljet.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * DEFINES
 *----------------------------------------------*/

/*--------------------------------------
 * Define: ArrayPooling
 *
 * Description:
 *   Kommentera ut raden nedan för att inte använda array-poolen.
 *------------------------------------*/
#define ArrayPooling

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "quickhull.h"

#include "core/common.h"
#include "core/debug.h"
#include "core/math.h"
#include "core/collections/array.h"
#include "core/collections/queue.h"

#include "assignment/algorithmdata.h"

#include <float.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Variable: arrayPool
 *
 * Description:
 *   En kö där vi poolar arrayer för att slippa omallokeringar. Vi återanvänder
 *   helt enkelt minne istället.
 *------------------------------------*/
static queueADT arrayPool;

/*--------------------------------------
 * Function: GetPointArray()
 * Parameters:
 *
 * Description:
 *   Returnerar en array med pekare till punkter. Antingen skapar funktionen en
 *   ny array, eller så återanvänder den en array från poolen.
 *------------------------------------*/
static arrayADT GetPointArray() {
#   ifdef ArrayPooling
        if (!arrayPool)
            arrayPool = NewQueue(32);

        if (QueueIsEmpty(arrayPool))
            return NewArray(sizeof(pointT *));

        return Dequeue(arrayPool);
#   else
        return NewArray(sizeof(pointT *));
#   endif
}

/*--------------------------------------
 * Function: ReleaseArray()
 * Parameters:
 *   a  Arrayen som ska släppas tillbaka till array-poolen.
 *
 * Description:
 *   Släpper tillbaka en array till poolen.
 *------------------------------------*/
static void ReleaseArray(arrayADT a) {
#   ifdef ArrayPooling
        if (QueueIsFull(arrayPool)) {
            FreeArray(a);
            printf("Warning: Array pool is not big enough.\n");
            return;
        }

        ResetArray(a);
        Enqueue(arrayPool, a);
   #else
        FreeArray(a);
#   endif
}

/*--------------------------------------
 * Function: InsertBefore()
 * Parameters:
 *   p  Den punkt som ska sättas in i a framför q.
 *   q  Den punkt i a som p ska sättas in framför.
 *   a  Array med höljets punkter.
 *
 * Description:
 *   Sätter in punkten p framför q, i a.
 *------------------------------------*/
static int InsertBefore(pointT **p, pointT *q, arrayADT a) {
    int n = ArrayLength(a);
    for (int i = 0; i < n; i++) {
        pointT *point = *(pointT **)ArrayGet(a, i);
        if (point == q) {
            // Här stoppar vi in punkten i a, innan p.
            ArrayInsert(a, i, p);
            return i;
        }
    }

    // Det här får aldrig hända.
    Error("This should not happen");
    return -1;
}

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
static algorithmDataT QH(arrayADT hull, pointT *a, pointT *b, arrayADT subset) {
    algorithmDataT algo = { 0 };

    int numPoints = ArrayLength(subset);

    /*--------------------------------------------------------------------------
     * 1a. INGA PUNKTER
     *
     *   Om vi inte fick några punkter så är linjen a---b ett segment i höljet,
     *   så vi behöver inte göra något mer här.
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
        algo.numOps = InsertBefore( ArrayGet(subset, 0), b, hull);
        return algo;
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

        // Vi vill mäta max avstånd oavsett sida om linjen här, så om d är
        // negativ så ser vi till att variabeln får ett positivt värde istället.
        if (d < 0.0f) d = -d;

        if (d > dMax) {
            index = i;
            dMax  = d;
        }
    }

    algo.numOps += numPoints;

    // Punkten farPoint är den punkt, av alla punkter i 'subset' som är längst
    // bort från linjen a---b.
    pointT *farPoint = *(pointT **)ArrayGet(subset, index);

    algo.numOps += InsertBefore(&farPoint, b, hull);

    /*--------------------------------------------------------------------------
     * 2. ANDRA PUNKTER
     *
     *   Vi bildar en triangel med a, b och p, där p är den punkt som ligger
     *   längst bort från linjen a---b. Segmenten a---p---b är garanterat delar
     *   av höljet. Punkterna inuti triangeln ignoreras. Sedan hanterar vi
     *   punkter på varsin sida om triangeln rekursivt.
     *------------------------------------------------------------------------*/

    arrayADT subsetA = GetPointArray(),
             subsetB = GetPointArray();

    algo.numAllocs += 2;

    for (int i = 0; i < numPoints; i++) {
        pointT *point = *(pointT **)ArrayGet(subset, i);

        float dA = (farPoint->x - a->x) * (point->y - a->y)
                 - (farPoint->y - a->y) * (point->x - a->x);
        if (dA < 0.0f) ArrayAdd(subsetA, &point);

        float dB = (b->x - farPoint->x) * (point->y - farPoint->y)
                 - (b->y - farPoint->y) * (point->x - farPoint->x);
        if (dB < 0.0f) ArrayAdd(subsetB, &point);
    }

    algo.numOps += 2 * numPoints;

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

    algorithmDataT algoA = QH(hull, a       , farPoint, subsetA);
    algorithmDataT algoB = QH(hull, farPoint, b       , subsetB);

    algo.numOps    += algoA.numOps    + algoB.numOps   ;
    algo.numAllocs += algoA.numAllocs + algoB.numAllocs;
    algo.numBytes  += algoA.numBytes  + algoB.numBytes ;

    /*--------------------------------------------------------------------------
     * 4. DEALLOKERING
     *
     *   Dags att rensa upp minnet.
     *------------------------------------------------------------------------*/

    algo.numBytes += ArrayBytes(subsetA) + ArrayBytes(subsetB);

    ReleaseArray(subsetA);
    ReleaseArray(subsetB);

    return algo;
}

/*--------------------------------------
 * Function: Quickhull()
 * Parameters:
 *   ps    Punktuppsättningen för vilken ett hölje ska genereras.
 *   hull  En pekare till höljet.
 *
 * Description:
 *   Genererar att konvext hölje för punktuppsättningen med hjälp av algoritmen
 *   quickhull. Returnerar data om algoritmens arbete.
 *------------------------------------*/
algorithmDataT Quickhull(pointsetT ps, hullT *hull) {
    algorithmDataT algo = { 0 };

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
    pointT *leftPoint  = &ps.points[0],
           *rightPoint = leftPoint;

    // Efter att algoritmen är klar kommer hullPoints innehålla alla punkter i
    // höljer i medurs ordning.
    arrayADT hullPoints = GetPointArray(),
             subsetA    = GetPointArray(),
             subsetB    = GetPointArray();

    // Tre allokeringar ovan.
    algo.numAllocs += 3;

    /*--------------------------------------------------------------------------
     * 2. SÖKNING EFTER EXTREMPUNKTER
     *
     *   Vi letar upp vänstra och högra extrempunkterna. Dessa är garanterat en
     *   del av det konvexa höljet, och läggs därför in i punktlistan omgående.
     *------------------------------------------------------------------------*/

    for (int i = 1; i < ps.numPoints; i++) {
        pointT *point = &ps.points[i];

        if (point->x < leftPoint ->x) leftPoint  = point;
        if (point->x > rightPoint->x) rightPoint = point;
    }

    ArrayAdd(hullPoints, &leftPoint );
    ArrayAdd(hullPoints, &rightPoint);

    algo.numOps += ps.numPoints - 1;

    /*--------------------------------------------------------------------------
     * 3. UPPDELNING I TVÅ HALVOR
     *
     *   Vi drar en linje mellan extrempunkterna. Punkterna på de två sidorna om
     *   linjen hamnar i varsina punktsamlingar; subsetA och subsetB.
     *------------------------------------------------------------------------*/

    for (int i = 0; i < ps.numPoints; i++) {
        pointT *point = &ps.points[i];

        // Vänster och höger extrempunkter har vi redan hanterat.
        if (point==leftPoint || point==rightPoint)
            continue;

        float d = (rightPoint->x - leftPoint->x) * (point->y - leftPoint->y)
                - (rightPoint->y - leftPoint->y) * (point->x - leftPoint->x);

        if (d < 0.0f) ArrayAdd(subsetA, &point); // Övre "halva."
        else          ArrayAdd(subsetB, &point); // Nedra "halva."
    }

    algo.numOps += ps.numPoints;

    /*--------------------------------------------------------------------------
     * 4. REKURSION
     *
     *   Här hanterar vi de två punktsamlingarna rekursivt, vilket är quickhull-
     *   algoritmens rekursiva kärna.
     *------------------------------------------------------------------------*/

    algorithmDataT algoA = QH(hullPoints, leftPoint , rightPoint, subsetA);
    algorithmDataT algoB = QH(hullPoints, rightPoint, leftPoint , subsetB);

    algo.numOps    += algoA.numOps    + algoB.numOps   ;
    algo.numAllocs += algoA.numAllocs + algoB.numAllocs;
    algo.numBytes  += algoA.numBytes  + algoB.numBytes ;

    /*--------------------------------------------------------------------------
     * 5. IHOPSÄTTNING AV HÖLJE
     *
     *   Vi "monterar ihop" höljet genom att länka ihop punkterna. De är sparade
     *   i rätt ordning i listan, så vi kan bara linjer från en punkt till nästa
     *   tills vi gått ett helt varv.
     *------------------------------------------------------------------------*/

    hull->numLines = 0;
    int numHullPoints = ArrayLength(hullPoints);
    for (int i = 0; i < numHullPoints; i++) {
        if (hull->numLines >= hull->maxLines)
            Error("Hull is too small");

        hull->lines[i].a = *(pointT **)ArrayGet(hullPoints, i);
        hull->lines[i].b = *(pointT **)ArrayGet(hullPoints,
                                                (i+1) % numHullPoints);
        hull->numLines++;
    }

    algo.numOps += numHullPoints;

    /*--------------------------------------------------------------------------
     * 6. DEALLOKERING
     *
     *   Här rensar vi upp minnet efter oss.
     *------------------------------------------------------------------------*/

    algo.numBytes += ArrayBytes(hullPoints)
                  +  ArrayBytes(subsetA   )
                  +  ArrayBytes(subsetB   );

    ReleaseArray(hullPoints);
    ReleaseArray(subsetA   );
    ReleaseArray(subsetB   );

    return algo;
}
