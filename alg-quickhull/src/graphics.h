/*------------------------------------------------------------------------------
 * File: graphics.h
 * Created: May 15, 2015
 * Last changed: May 15, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Trivialt grafikbibliotek för visualisering av enkel geometri. Här använder
 *   vi OpenGL.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

#ifndef Graphics_h
#define Graphics_h

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "common.h"
#include "math.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*--------------------------------------
 * Function: InitGraphics()
 * Parameters:
 *
 * Description:
 *   Initialiserar grafiksystemet.
 *------------------------------------*/
void InitGraphics();

void SetColor(float red, float green, float blue, float alpha);

void SetFrameRate(int fps);

void ClearDisplay(float red, float green, float blue);

/*--------------------------------------
 * Function: DrawHull()
 * Parameters:
 *   hull  Det hölje som ska ritas upp.
 *
 * Description:
 *   Ritar upp ett hölje.
 *------------------------------------*/
void DrawHull(hullT hull);

/*--------------------------------------
 * Function: DrawLine()
 * Parameters:
 *   line  Den linje som ska ritas upp.
 *
 * Description:
 *   Ritar upp en linje.
 *------------------------------------*/
void DrawLine(lineT line);

/*--------------------------------------
 * Function: DrawPoint()
 * Parameters:
 *   point  Den punkt som ska ritas upp.
 *
 * Description:
 *   Ritar upp en punkt.
 *------------------------------------*/
void DrawPoint(pointT point);

/*--------------------------------------
 * Function: DrawPoints()
 * Parameters:
 *   ps  Den uppsättning punkter som ska ritas upp.
 *
 * Description:
 *   Ritar en uppsättning punkter.
 *------------------------------------*/
void DrawPoints(pointsetT ps);

/*--------------------------------------
 * Function: UpdateDisplay()
 * Parameters:
 *
 * Description:
 *   Uppdaterar ritytan.
 *------------------------------------*/
void UpdateDisplay();

/*--------------------------------------
 * Function: OnKeyPress()
 * Parameters:
 *   c    Tecknet som ska associeras till en funktion.
 *   cb   Callback-funktionen som ska anropas när knappen med det specificerade
 *        tecknet trycks ned.
 *   arg  Det argument som ska skickas till callback-funktionen.
 *
 * Description:
 *   Registerar en callback-funktion för en viss knapp.
 *------------------------------------*/
void OnKeyPress(char c, actionT cb, void *arg);

/*--------------------------------------
 * Function: WindowIsOpen()
 * Parameters:
 *
 * Description:
 *   Returnerar sant om grafikfönstret är öppet.
 *------------------------------------*/
bool WindowIsOpen();

#endif // Graphics_h
