/*------------------------------------------------------------------------------
 * File: bruteforce.c
 * Created: May 21, 2015
 * Last changed: May 27, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Inneh�ller bruteforce-algoritmen f�r att l�sa det konvexa h�ljet.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "bruteforce.h"

#include "core/common.h"
#include "core/debug.h"
#include "core/math.h"
#include "core/collections/array.h"
#include "core/collections/queue.h"

#include "assignment/algorithmdata.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: BruteforceHull()
 * Parameters:
 *   ps    Punktupps�ttningen f�r vilken ett h�lje ska genereras.
 *   hull  En pekare till h�ljet.
 *
 * Description:
 *   Genererar att konvext h�lje f�r punktupps�ttningen genom utt�mmande
 *   s�kning. Returnerar information om algoritmens arbete.
 *------------------------------------*/
algorithmDataT BruteforceHull(pointsetT ps, hullT *hull) {
    algorithmDataT algo = { 0 };

    // For-looparna med i och j anv�nds f�r att konstruera alla t�nkbara
    // kombinationer av par bland punkterna. Vi konstruerar dem �t b�da h�ll,
    // d.v.s. b�de (a,b) och (b,a).

    hull->numLines = 0;
    for (int i = 0; i < ps.numPoints; i++) {
        for (int j = (i+1); j < (i+ps.numPoints); j++) {
            pointT *a, *b, *c;

            // For-loopen med k anv�nds f�r att kolla att alla punkter ligger p�
            // r�tt sida av linjen mellan punkt i och j.

            bool outside = FALSE;
            for (int k = 0; k < ps.numPoints; k++) {
                // Linjen a---b �r (potentiellt) ett segment i h�ljet. Vi avg�r
                // det genom att se vilken sida om linjen som punkten c ligger
                // p�.
                a = &ps.points[i];
                b = &ps.points[j % ps.numPoints];
                c = &ps.points[k];

                // Nedan avg�r vi vilken sida om linjen punkten ligger p�.
                //
                // d<0.0 : Punkten �r utanf�r.
                // d=0.0 : Punkten ligger p� linjen.
                // d>0.0 : Punkten �r innanf�r.
                //
                // Dessutom har jag l�tit detta utg�ra algoritmens kritiska
                // operation, vilket jag hoppas �r adekvat.
                algo.numOps++;
                float d = (b->x - a->x) * (c->y - a->y)
                        - (b->y - a->y) * (c->x - a->x);

                if (d < 0.0f) {
                    outside = TRUE;
                    break;
                }
            }

            // Om alla punkter visade sig vara innanf�r linjen, s� beh�ller vi
            // den som ett segment i det konvexa h�ljet.
            if (!outside) {
                if (hull->numLines >= hull->maxLines)
                    Error("Hull is too small");

                hull->lines[hull->numLines].a = a;
                hull->lines[hull->numLines].b = b;
                hull->numLines++;
                break;
            }
        }
    }

    return algo;
}
