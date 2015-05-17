/*------------------------------------------------------------------------------
 * File: sandbox.c
 * Created: May 16, 2015
 * Last changed: May 16, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Sandbox-funktioner för att visualisera alltihop på roliga sätt.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "common.h"
#include "graphics.h"
#include "math.h"

#include <math.h>
#include <stdio.h>

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: Speed
 *
 * Description:
 *   Hastighetsmultipel för körningshastighet vad gäller simulering av
 *   punkterna. 0.5 = halv hastighet, 1.0 = normal, 2.0 = dubbel, etc.
 *------------------------------------*/
#define Speed 1.0f

/*--------------------------------------
 * Constant: FrameRate
 *
 * Description:
 *   Antal bildrutor per sekund.
 *------------------------------------*/
#define FrameRate 60

/*--------------------------------------
 * Constant: StepSize
 *
 * Description:
 *   Tidssteget vi simulerar i fysiksimuleringen varje steg. Flera steg kan
 *   köras varje bildruta för att garantera välsimulerad fysik.
 *------------------------------------*/
#define StepSize 1.0f / 120.0f

/*--------------------------------------
 * Constant: BottomEdge
 *
 * Description:
 *   Nedre kantens y-koordinat.
 *------------------------------------*/
#define BottomEdge -0.9f

/*--------------------------------------
 * Constant: LeftEdge
 *
 * Description:
 *   Vänsterkantens x-koordinat.
 *------------------------------------*/
#define LeftEdge -0.9f

/*--------------------------------------
 * Constant: RightEdge
 *
 * Description:
 *   Högerkantens x-koordinat.
 *------------------------------------*/
#define RightEdge 0.9f

/*--------------------------------------
 * Constant: TopEdge
 *
 * Description:
 *   Övre kantens y-koordinat.
 *------------------------------------*/
#define TopEdge 0.9f

/*--------------------------------------
 * Constant: BlackHoleForce
 *
 * Description:
 *   Svarta hålets dragningskraft.
 *------------------------------------*/
#define BlackHoleForce 6.0f

/*--------------------------------------
 * Constant: DampingCoefficient
 *
 * Description:
 *   Dämpningskoefficient.
 *------------------------------------*/
#define DampingCoefficient 1.3f

/*--------------------------------------
 * Constant: Gravity
 *
 * Description:
 *   Gravitationskraft.
 *------------------------------------*/
#define Gravity -9.81f

/*--------------------------------------
 * Constant: Wind
 *
 * Description:
 *   Vindens kraft.
 *------------------------------------*/
#define Wind 7.0f

/*--------------------------------------
 * Constant: SpringCoefficient
 *
 * Description:
 *   Fjädringskoefficienten. Högre värde ger "stelare" gummiband runt prickarna.
 *------------------------------------*/
#define SpringCoefficient 20.0f

/*--------------------------------------
 * Constant: SpringDamping
 *
 * Description:
 *   Mjukar sammandragningen och expansionen av gummibandet.
 *------------------------------------*/
#define SpringDamping 0.4f

/*--------------------------------------
 * Constant: SpringLength
 *
 * Description:
 *   Längden som segmenten i gummibandet eftersträvar.
 *------------------------------------*/
#define SpringLength 0.07f

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

// Dessa variabler förklaras i instruktionerna vid körning.
bool blackHole    = FALSE;
bool damping      = FALSE;
bool drawHull     = TRUE;
bool drawPoints   = TRUE;
bool gravity      = FALSE;
bool rubberBand   = FALSE;
bool slopedFloor  = FALSE;
bool useQuickhull = TRUE;
bool wind         = FALSE;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Nedan är ett gäng funktioner som hanterar knapptryckningar, för att sätta  */
/* och stänga av olika switchar, etc.                                         */
/*----------------------------------------------------------------------------*/

static void RandomizePS(void *arg) {
    pointsetT *ps = (pointsetT *)arg;
    RandomizePoints(*ps);
}

static void ToggleBlackHole(void *arg) {
    blackHole = !blackHole;

    if (blackHole) printf(":: Black hole enabled.\n");
    else           printf(":: Black hole disabled.\n");
}

static void ToggleDamping(void *arg) {
    damping = !damping;

    if (damping) printf(":: Damping enabled.\n");
    else         printf(":: Damping disabled.\n");
}

static void ToggleGravity(void *arg) {
    gravity = !gravity;

    if (gravity) printf(":: Gravity enabled.\n");
    else         printf(":: Gravity disabled.\n");
}

static void ToggleHull(void *arg) {
    drawHull = !drawHull;

    if (drawHull) printf(":: Hull rendering enabled.\n");
    else          printf(":: Hull rendering disabled.\n");
}

static void TogglePoints(void *arg) {
    drawPoints = !drawPoints;

    if (drawPoints) printf(":: Point rendering enabled.\n");
    else            printf(":: Point rendering disabled.\n");
}

static void ToggleRubberBand(void *arg) {
    rubberBand = !rubberBand;

    if (rubberBand) printf(":: Rubber band enabled.\n");
    else            printf(":: Rubber band disabled.\n");
}

static void ToggleWind(void *arg) {
    wind = !wind;

    if (wind) printf(":: Wind enabled.\n");
    else      printf(":: Wind disabled.\n");
}

static void ToggleSlopedFloor(void *arg) {
    slopedFloor = !slopedFloor;

    if (slopedFloor) printf(":: Sloped floor enabled.\n");
    else             printf(":: Sloped floor disabled.\n");
}

static void ToggleQuickhull(void *arg) {
    useQuickhull = !useQuickhull;

    if (useQuickhull) printf(":: Using quickhull.\n");
    else              printf(":: Using bruteforce.\n");
}

/*----------------------------------------------------------------------------*/
/* Här nedan är sandbox-lägets mest relevanta funktioner, som meddelanden,        */
/* uppdatering av punkter, rendering etc.                                     */
/*----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: PrintInstructions()
 * Parameters:
 *
 * Description:
 *   Skriver ut instruktionerna för sandbox-läget.
 *------------------------------------*/
static void PrintInstructions() {
    printf("\nINSTRUCTIONS:\n\n"
           "KEYS\n------------\n"
           "Key    Effect\n\n"
           "  b    Toggles black hole (gravity towards center).\n"
           "  d    Toggles damping.\n"
           "  g    Toggles gravity.\n"
           "  h    Toggles hull rendering.\n"
           "  p    Toggles point rendering.\n"
           "  q    Toggles between bruteforce and quickhull.\n"
           "  r    Toggles rubber band mode (hull becomes a rubber band).\n"
           "  s    Toggles sloped floor.\n"
           "  w    Toggles wind.\n"
           "  x    Randomizes point velocities.\n"
           "  z    Randomizes point positions.\n"
           "------------\n\n");
}

/*--------------------------------------
 * Function: UpdatePoints()
 * Parameters:
 *   aps   Accelerationer.
 *   ps    Positioner.
 *   vps   Hastigheter.
 *   hull  Höljet.
 *   dt    Den tid som ska simuleras, i sekunder.
 *
 * Description:
 *   Stegar punkterna framåt i tiden genom att simulera deras fysik.
 *------------------------------------*/
static void UpdatePoints(pointsetT aps, pointsetT ps, pointsetT vps, hullT hull,
                         float dt)
{
    if (rubberBand) {
        // Här nedan tillämpar vi Hookes lag på segmenten i höljet, så att
        // höljet drar sig samman runt punkterna likt ett gummiband.
        //
        // Se http://en.wikipedia.org/wiki/Hooke%27s_law för mer information.
        //
        for (int k = 0; k < hull.numLines; k++) {
            lineT line = hull.lines[k];

            // Nedan använder vi lite ful pekararitmetik för att lista
            // ut punkternas index, så att vi kan arbeta med deras
            // hastigheter och accelerationer.
            int i = line.a - ps.points;
            int j = line.b - ps.points;

            pointT dp = { ps .points[j].x - ps .points[i].x,
                          ps .points[j].y - ps .points[i].y };

            pointT dv = { vps.points[j].x - vps.points[i].x,
                          vps.points[j].y - vps.points[i].y };

            float r = (float)sqrt(dp.x*dp.x + dp.y*dp.y);

            // Punkterna ligger kanske för nära varandra för att vi ska kunna
            // trycka isär dem utan avrundningsproblem.
            if (r < 0.000001f)
                continue;

            dp.x /= r;
            dp.y /= r;

            r -= SpringLength;

            pointT f = { dp.x*r*SpringCoefficient + dv.x*SpringDamping,
                         dp.y*r*SpringCoefficient + dv.y*SpringDamping };

            aps.points[i].x += f.x;
            aps.points[i].y += f.y;
            aps.points[j].x -= f.x;
            aps.points[j].y -= f.y;
        }
    }

    for (int i = 0; i < ps.numPoints; i++) {
        pointT *a = &aps.points[i],
               *p = &ps .points[i],
               *v = &vps.points[i];

        //-----------------------------------
        // Här tillämpar vi miljömässiga effekter på punkterna.

        if (blackHole) {
            float r = (float)sqrt(p->x*p->x + p->y*p->y);
            // Vi använder en linjär modell för gravitationen, inte inverse
            // square law. Den funkar uselt med Euler-integrering oavsett.
            if (r > 0.000001f) {
                a->x -= p->x*BlackHoleForce/r;
                a->y -= p->y*BlackHoleForce/r;
            }
        }

        if (gravity)
            a->y += Gravity;

        if (damping) {
            a->x -= v->x*DampingCoefficient;
            a->y -= v->y*DampingCoefficient;
        }

        if (wind)
            a->x += Wind;

        //-----------------------------------

        // Vi stegar punkterna framåt linjärt med Euler-integrering.
        v->x += a->x*dt; v->y += a->y*dt;
        p->x += v->x*dt; p->y += v->y*dt;

        // Efter att vi tillämpat accelerationerna så nollställer vi dem.
        a->x = a->y = 0.0f;

        // Här ser vi till att punkterna inte åker för långt ut mot kanterna,
        // utan att de i sådana fall studsar mot kanten och åker tillbaka.
             if (p->x <  LeftEdge) { p->x =  LeftEdge; v->x *= -1.0f; }
        else if (p->x > RightEdge) { p->x = RightEdge; v->x *= -1.0f; }
             if (p->y >   TopEdge) { p->y =   TopEdge; v->y *= -1.0f; }

        // Ett lutande golv behöver lite specialbehandling.
        if (slopedFloor) {
            float x     = (p->x-LeftEdge) / (RightEdge-LeftEdge);
            float floor = -0.9f * (1.0f-x) - 0.3f * x;

            if (p->y < floor) {
                p->y  = floor;

                // Vektorn n är golvets perp-vektor normaliserad. Vi räknar ut
                // den såhär:
                //
                //   g = (0.9 - (-0.9), (-0.3) - (-0.9))
                //     = (1.8, 0.6)
                //
                //   g_perp = (-0.6, 1.8)
                //
                //   n^ = g_perp / ||g_perp|| 
                //      = (-0.6, 1.8) / ||(-0.6, 1.8)||
                //      = (-0.6, 1.8) / 1.89737
                //      = (-0.316228, 0.948683)
                //
                pointT n = { -0.316228f, 0.948683f };
                *v = Reflect(*v, n);
            }
        }
        else if (p->y < BottomEdge) {
            p->y  = BottomEdge;
            v->y *= -1.0f;
        }
    }
}

/*--------------------------------------
 * Function: RunSandbox()
 * Parameters:
 *   numPoints  Antal punkter att använda vid uträkning av det konvexa höljet.
 *
 * Description:
 *   Kör programmet i sandbox-läge.
 *------------------------------------*/
void RunSandbox(int numPoints) {
    // Vi skapar uppsättningen med punkter som ska visas på skärmen.
    printf("Creating random points...");
    pointsetT ps = CreatePoints(numPoints);
    RandomizePoints(ps);
    printf(" done.\n");

    // Vi skapar en till uppsättning punkter - lika många som i ps - och
    // sparar dem i v. Vi använder i själva verket dessa som hastigheter för
    // punkterna i ps, för att kunna låta dem studsa omkring på skärmen!
    printf("Randomizing velocities...");
    pointsetT vps = CreatePoints(ps.numPoints);
    RandomizePoints(vps);
    printf(" done.\n");

    // Den tredje uppsättningen använder vi till accelerationer.
    printf("Creating acceleration set...");
    pointsetT aps = CreatePoints(ps.numPoints);
    for (int i = 0; i < aps.numPoints; i++) {
        aps.points[i].x = 0.0f;
        aps.points[i].y = 0.0f;
    }
    printf(" done.\n");

    // Höljet skapar vi här för att slippa massa allokeringar inuti huvudloopen.
    printf("Initializing hull...");
    hullT hull = InitHull(ps);
    printf(" done.\n");

    // Här skapar vi de fyra hörnen och räknar ut höljet(!) för dem, för att få
    // en fin ruta som visar var kanterna går.
    printf("Initializing world hull...");
    pointsetT corners = CreatePoints(4);
    hullT     edges   = InitHull    (corners);

    corners.points[0].x =  LeftEdge; corners.points[0].y =    TopEdge;
    corners.points[1].x = RightEdge; corners.points[1].y =    TopEdge;
    corners.points[2].x = RightEdge; corners.points[2].y = BottomEdge;
    corners.points[3].x =  LeftEdge; corners.points[3].y = BottomEdge;

    Quickhull(corners, &edges, NULL);
    printf(" done.\n");

    PrintInstructions();

    printf("Press ENTER to start...\n");
    getchar();

    printf("Initializing graphics...");
    InitGraphics();
    SetFrameRate(FrameRate);

    // Här ställer vi in lite roliga knappar så att sandbox blir lite mer
    // interaktiv och rolig.
    OnKeyPress('b', ToggleBlackHole  , NULL);
    OnKeyPress('d', ToggleDamping    , NULL);
    OnKeyPress('f', ToggleSlopedFloor, NULL);
    OnKeyPress('g', ToggleGravity    , NULL);
    OnKeyPress('h', ToggleHull       , NULL);
    OnKeyPress('p', TogglePoints     , NULL);
    OnKeyPress('q', ToggleQuickhull  , NULL);
    OnKeyPress('r', ToggleRubberBand , NULL);
    OnKeyPress('w', ToggleWind       , NULL);
    OnKeyPress('x', RandomizePS      , &vps);
    OnKeyPress('z', RandomizePS      , &ps);

    printf(" done.\n");
    printf("Enjoy! :-)\n\n");

    float dt = 0.0f;
    while (WindowIsOpen()) {
        dt += Speed / FrameRate;
        while (dt >= StepSize) {
            UpdatePoints(aps, ps, vps, hull, StepSize);
            dt -= StepSize;

            if (useQuickhull) Quickhull     (ps, &hull, NULL);
            else              BruteforceHull(ps, &hull, NULL);
        }

        // Dags att rita upp allting! Rensa ritytan!
        ClearCanvas(0.992f, 0.964f, 0.890f);

        // Först ritar vi kanterna och höljet.
        SetColor(0.000f, 0.000f, 0.000f, 1.000f);
        DrawHull(edges);

        if (drawHull) DrawHull(hull);

        // Sedan miljögrejer.
        if (slopedFloor) {
            pointT a = {  LeftEdge, -0.9f },
                   b = { RightEdge, -0.3f };

            lineT floor = { &a, &b };

            DrawLine(floor);
        }

        if (drawPoints) {
            // Sedan prickarna (så att de ligger ovanpå höljet).
            SetColor  (0.866f, 0.000f, 0.000f, 1.000f);
            DrawPoints(ps);
        }

        // Fram med allt på skärmen!
        UpdateDisplay();
    }

    printf("Exiting...\n");

    FreeHull  (edges);
    FreePoints(corners);

    FreeHull  (hull);
    FreePoints(vps);
    FreePoints(ps);
}
