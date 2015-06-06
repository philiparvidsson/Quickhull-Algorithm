/*------------------------------------------------------------------------------
 * File: quickhull.c
 * Created: May 21, 2015
 * Last changed: May 21, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Inneh�ller algoritmen quickhull f�r att l�sa det konvexa h�ljet.
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
 *   Kommentera ut raden nedan f�r att inte anv�nda array-poolen.
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

#include "assignment/akl-toussaint.h"
#include "assignment/algorithmdata.h"

#include <float.h>

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Variable: arrayPool
 *
 * Description:
 *   En k� d�r vi poolar arrayer f�r att slippa omallokeringar. Vi �teranv�nder
 *   helt enkelt minne ist�llet.
 *------------------------------------*/
static queueADT arrayPool;

/*--------------------------------------
 * Function: GetPointArray()
 * Parameters:
 *
 * Description:
 *   Returnerar en array med pekare till punkter. Antingen skapar funktionen en
 *   ny array, eller s� �teranv�nder den en array fr�n poolen.
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
 *   a  Arrayen som ska sl�ppas tillbaka till array-poolen.
 *
 * Description:
 *   Sl�pper tillbaka en array till poolen.
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
 *   p  Den punkt som ska s�ttas in i a framf�r q.
 *   q  Den punkt i a som p ska s�ttas in framf�r.
 *   a  Array med h�ljets punkter.
 *
 * Description:
 *   S�tter in punkten p framf�r q, i a.
 *------------------------------------*/
static int InsertBefore(pointT **p, pointT *q, arrayADT a) {
    int n = ArrayLength(a);
    for (int i = 0; i < n; i++) {
        pointT *point = *(pointT **)ArrayGet(a, i);
        if (point == q) {
            // H�r stoppar vi in punkten i a, innan p.
            ArrayInsert(a, i, p);
            return i;
        }
    }

    // Det h�r f�r aldrig h�nda.
    Error("This should not happen");
    return -1;
}

/*--------------------------------------
 * Function: QH()
 * Parameters:
 *   hull    Array med h�ljets punkter.
 *   a       A-punkten i triangeln.
 *   b       B-punkten i triangeln.
 *   subset  Punkterna vi ska hantera.
 *
 * Description:
 *   Hanterar punkter och l�ser det konvexa h�ljet med hj�lp av den rekursiva
 *   algoritmen quickhull.
 *------------------------------------*/
static algorithmDataT QH(arrayADT hull, pointT *a, pointT *b, arrayADT subset) {
    algorithmDataT algo = { 0 };

    int numPoints = ArrayLength(subset);

    /*--------------------------------------------------------------------------
     * 1a. INGA PUNKTER
     *
     *   Om vi inte fick n�gra punkter s� �r linjen a---b ett segment i h�ljet,
     *   s� vi beh�ver inte g�ra n�got mer h�r.
     *------------------------------------------------------------------------*/

    if (numPoints == 0)
        return algo;

    /*--------------------------------------------------------------------------
     * 1b. EN ENDA PUNKT
     *
     *   Om vi bara fick en punkt s� �r segmenten a---p---b segment i h�ljet,
     *   d�r p �r den enda punkten vi f�tt in. Vi l�gger in den mellan a och b,
     *   sedan �r vi klara.
     *------------------------------------------------------------------------*/

    if (numPoints == 1) {
        algo.numOps = InsertBefore(ArrayGet(subset, 0), b, hull);
        return algo;
    }

    /*--------------------------------------------------------------------------
     * 1c. PUNKTEN L�NGST BORT
     *
     *   Vi bildar en triangel med a, b och p, d�r p �r den punkt som ligger
     *   l�ngst bort fr�n linjen a---b. Segmenten a---p---b �r garanterat delar
     *   av h�ljet. Punkterna inuti triangeln ignoreras. Sedan hanterar vi
     *   punkter p� varsin sida om triangeln rekursivt.
     *------------------------------------------------------------------------*/

    float dMax  = -FLT_MAX;
    int   index = -1;

    for (int i = 0; i < numPoints; i++) {
        pointT *point = *(pointT **)ArrayGet(subset, i);

        float d = (b->x - a->x) * (point->y - a->y)
                - (b->y - a->y) * (point->x - a->x);

        // Vi vill m�ta max avst�nd oavsett sida om linjen h�r, s� om d �r
        // negativ s� ser vi till att variabeln f�r ett positivt v�rde ist�llet.
        if (d < 0.0f) d = -d;

        if (d > dMax) {
            index = i;
            dMax  = d;
        }
    }

    algo.numOps += numPoints;

    // Punkten farPoint �r den punkt, av alla punkter i 'subset' som �r l�ngst
    // bort fr�n linjen a---b.
    pointT *farPoint = *(pointT **)ArrayGet(subset, index);

    algo.numOps += InsertBefore(&farPoint, b, hull);

    /*--------------------------------------------------------------------------
     * 2. ANDRA PUNKTER
     *
     *   Vi bildar en triangel med a, b och p, d�r p �r den punkt som ligger
     *   l�ngst bort fr�n linjen a---b. Segmenten a---p---b �r garanterat delar
     *   av h�ljet. Punkterna inuti triangeln ignoreras. Sedan hanterar vi
     *   punkter p� varsin sida om triangeln rekursivt.
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
     *   H�r hanterar vi de tv� delm�ngderna med punkter rekursivt. Detta �r
     *   algoritmens k�rna. Den ena delm�ngden inneh�ller punkter p� ena sidan
     *   om triangeln, och den andra inneh�ller punkterna p� andra sidan
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
 *   ps    Punktupps�ttningen f�r vilken ett h�lje ska genereras.
 *   hull  En pekare till h�ljet.
 *
 * Description:
 *   Genererar att konvext h�lje f�r punktupps�ttningen med hj�lp av algoritmen
 *   quickhull. Returnerar data om algoritmens arbete.
 *------------------------------------*/
algorithmDataT Quickhull(pointsetT ps, hullT *hull) {
    algorithmDataT algo = { 0 };

    /*--------------------------------------------------------------------------
     * 1. INITIERING AV VARIABLER OCH DATA
     *
     *   H�r allokerar vi minne och arrayer/vektorer som vi beh�ver f�r att
     *   lagra information om h�ljet och de 'subset' punkter (tv� "halvor" av
     *   h�ljet) som vi ska jobba med.
     *
     *------------------------------------------------------------------------*/

    // Vi initierar b�de left point och right point till den f�rsta punkten i
    // upps�ttningen, bara f�r att inte beh�va hantera null-pekare.
    pointT *leftPoint  = &ps.points[0],
           *rightPoint = leftPoint;

    // Efter att algoritmen �r klar kommer hullPoints inneh�lla alla punkter i
    // h�ljer i medurs ordning.
    arrayADT hullPoints = GetPointArray(),
             subsetA    = GetPointArray(),
             subsetB    = GetPointArray();

    // Tre allokeringar ovan.
    algo.numAllocs += 3;

    /*--------------------------------------------------------------------------
     * 2. S�KNING EFTER EXTREMPUNKTER
     *
     *   Vi letar upp v�nstra och h�gra extrempunkterna. Dessa �r garanterat en
     *   del av det konvexa h�ljet, och l�ggs d�rf�r in i punktlistan omg�ende.
     *------------------------------------------------------------------------*/

    for (int i = 1; i < ps.numPoints; i++) {
        pointT *point = &ps.points[i];

        if (point->x < leftPoint ->x)  leftPoint = point;
        if (point->x > rightPoint->x) rightPoint = point;
    }

    ArrayAdd(hullPoints, &leftPoint );
    ArrayAdd(hullPoints, &rightPoint);

    algo.numOps += ps.numPoints - 1;

    /*--------------------------------------------------------------------------
     * 3. UPPDELNING I TV� HALVOR
     *
     *   Vi drar en linje mellan extrempunkterna. Punkterna p� de tv� sidorna om
     *   linjen hamnar i varsina punktsamlingar; subsetA och subsetB.
     *------------------------------------------------------------------------*/

    for (int i = 0; i < ps.numPoints; i++) {
        pointT *point = &ps.points[i];

        // V�nster och h�ger extrempunkter har vi redan hanterat.
        if (point==leftPoint || point==rightPoint)
            continue;

        float d = (rightPoint->x - leftPoint->x) * (point->y - leftPoint->y)
                - (rightPoint->y - leftPoint->y) * (point->x - leftPoint->x);

        if (d < 0.0f) ArrayAdd(subsetA, &point); // �vre "halva."
        else          ArrayAdd(subsetB, &point); // Nedra "halva."
    }

    algo.numOps += ps.numPoints;

    /*--------------------------------------------------------------------------
     * 4. REKURSION
     *
     *   H�r hanterar vi de tv� punktsamlingarna rekursivt, vilket �r quickhull-
     *   algoritmens rekursiva k�rna.
     *------------------------------------------------------------------------*/

    algorithmDataT algoA = QH(hullPoints, leftPoint , rightPoint, subsetA);
    algorithmDataT algoB = QH(hullPoints, rightPoint, leftPoint , subsetB);

    algo.numOps    += algoA.numOps    + algoB.numOps   ;
    algo.numAllocs += algoA.numAllocs + algoB.numAllocs;
    algo.numBytes  += algoA.numBytes  + algoB.numBytes ;

    /*--------------------------------------------------------------------------
     * 5. IHOPS�TTNING AV H�LJE
     *
     *   Vi "monterar ihop" h�ljet genom att l�nka ihop punkterna. De �r sparade
     *   i r�tt ordning i listan, s� vi kan bara linjer fr�n en punkt till n�sta
     *   tills vi g�tt ett helt varv.
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
     *   H�r rensar vi upp minnet efter oss.
     *------------------------------------------------------------------------*/

    algo.numBytes += ArrayBytes(hullPoints)
                  +  ArrayBytes(subsetA   )
                  +  ArrayBytes(subsetB   );

    ReleaseArray(hullPoints);
    ReleaseArray(subsetA   );
    ReleaseArray(subsetB   );

    return algo;
}
