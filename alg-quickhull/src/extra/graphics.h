/*------------------------------------------------------------------------------
 * File: graphics.h
 * Created: May 15, 2015
 * Last changed: May 15, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Enkelt grafikbibliotek för uppritning av simpel geometri. Här använder vi
 *   OpenGL.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

#ifndef _graphics_h_
#define _graphics_h_

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"
#include "core/math.h"

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Grundläggande funktioner för att initiera grafikfönstret, ställa in        */
/* egenskaper, med mera.                                                      */
/*----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: InitGraphics()
 * Parameters:
 *
 * Description:
 *   Initierar grafikbiblioteket.
 *------------------------------------*/
void InitGraphics();

/*--------------------------------------
 * Function: SetFrameRate()
 * Parameters:
 *   fps  Antalet bildrutor per sekund.
 *
 * Description:
 *   Ställer in hur många bildrutor per sekund som ska ritas.
 *------------------------------------*/
void SetFrameRate(int fps);

/*--------------------------------------
 * Function: SetColor()
 * Parameters:
 *   red    Röd färgkomponent.
 *   green  Grön färgkomponent.
 *   blue   Blå färgkomponent.
 *   alpha  Alphavärde.
 *
 * Description:
 *   Ställer in färgen för nästa ritoperation.
 *------------------------------------*/
void SetColor(float red, float green, float blue, float alpha);

/*--------------------------------------
 * Function: SetLineWidth()
 * Parameters:
 *   width  Bredden på linjer, i pixlar.
 *
 * Description:
 *   Ställer in bredden på linjer.
 *------------------------------------*/
void SetLineWidth(float width);

/*--------------------------------------
 * Function: WindowIsOpen()
 * Parameters:
 *
 * Description:
 *   Returnerar sant om grafikfönstret är öppet.
 *------------------------------------*/
bool WindowIsOpen();

/*----------------------------------------------------------------------------*/
/* Ritfunktioner för att rensa ritytan, rita punkter, linjer, trianglar m.m.  */
/*----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: ClearCanvas()
 * Parameters:
 *   red    Röd färgkomponent.
 *   green  Grön färgkomponent.
 *   blue   Blå färgkomponent.
 *
 * Description:
 *   Rensar ritytan till den specificerade färgen.
 *------------------------------------*/
void ClearCanvas(float red, float green, float blue);

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
 *   p  Punkten som ska ritas.
 *
 * Description:
 *   Ritar en punkt på ritytan.
 *------------------------------------*/
void DrawPoint(pointT p);

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

/*----------------------------------------------------------------------------*/
/* Funktioner för input, t ex associera knappar med callback-funktioner m.m.  */
/*----------------------------------------------------------------------------*/

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

#endif // _graphics_h_
