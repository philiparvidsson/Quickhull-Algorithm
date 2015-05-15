#ifndef GRAPHICS_H_
#define GRAPHICS_H_

//------------------------------------------------
// INCLUDES
//------------------------------------------------

#include "common.h"
#include "math.h"

//------------------------------------------------
// FUNCTIONS
//------------------------------------------------

void ClearDisplay(float red, float green, float blue);

void DrawHull(hullT hull);

void DrawLine(lineT line);

void DrawPoint(pointT point);

void DrawPoints(pointsetT ps);

void InitGraphics();

void SetColor(float red, float green, float blue, float alpha);

void SetFrameRate(int fps);

void UpdateDisplay();

bool WindowIsOpen();

#endif // GRAPHICS_H_
