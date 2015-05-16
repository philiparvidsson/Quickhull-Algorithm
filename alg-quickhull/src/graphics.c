/*------------------------------------------------------------------------------
 * File: graphics.c
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

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "common.h"
#include "debug.h"
#include "graphics.h"
#include "math.h"

#include <tchar.h>
#include <Windows.h>

#include <gl/GL.h>

#include <time.h>

#pragma comment(lib, "opengl32.lib")

/*------------------------------------------------
 * CONSTANTS
 *----------------------------------------------*/

/*--------------------------------------
 * Constant: ClassName
 *
 * Description:
 *   Namnet på klassen som används för att skapa grafikfönstret.
 *------------------------------------*/
#define ClassName _T("alg-quickhull")

/*--------------------------------------
 * Constant: WindowTitle
 *
 * Description:
 *   Grafikfönstrets titel.
 *------------------------------------*/
#define WindowTitle _T("Quickhull Demo")

/*------------------------------------------------
 * TYPES
 *----------------------------------------------*/

/*--------------------------------------
 * Type: graphicsT
 *
 * Description:
 *   Håller reda på information om grafiksystemet.
 *------------------------------------*/
typedef struct {
    bool initialized;
    bool windowOpen;
    HWND hwnd;
    HDC  hdc;
    int invFrameRate;
    actionT keyPressCB[UCHAR_MAX];
    actionT keyPressArg[UCHAR_MAX];

    LARGE_INTEGER perfCounterFreq;
    LARGE_INTEGER lastUpdate;
} graphicsT;

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

/*--------------------------------------
 * Type: Variable
 *
 * Description:
 *   Här sparar vi allt vi behöver för att hålla ordning på grafiken.
 *------------------------------------*/
graphicsT graphics;

//------------------------------------------------
// FUNCTIONS
//------------------------------------------------

/*--------------------------------------
 * Function: WindowProc()
 * Parameters:
 *
 * Description:
 *   http://en.wikipedia.org/wiki/WindowProc
 *------------------------------------*/
LRESULT CALLBACK WindowProc(_In_ HWND   hwnd,
                            _In_ UINT   uMsg,
                            _In_ WPARAM wParam,
                            _In_ LPARAM lParam)
{
    if (uMsg == WM_CLOSE)
        graphics.windowOpen = FALSE;

    if (uMsg == WM_CHAR) {
        bool repeated = lParam & 0x40000000;
        if (!repeated) {
            char c = tolower(wParam);
            actionT cb = graphics.keyPressCB[c];
            if (cb)
                cb(graphics.keyPressArg[c]);
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

static void CheckInitGraphics() {
#ifdef _DEBUG
    if (!graphics.initialized)
        Error("InitGraphics() must be called first");
#endif
}

static void InitOpenGL() {
    HGLRC hglrc = wglCreateContext(graphics.hdc);

    if (!hglrc)
        Error("wglCreateContext() failed");

    if (!wglMakeCurrent(graphics.hdc, hglrc))
        Error("wglMakeCurrent() failed");

    // Utan GL_BLEND fungerar inte kantutjämningen för linjer.
    glEnable   (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Runda, fina prickar! :-)
    glEnable   (GL_POINT_SMOOTH);
    glHint     (GL_POINT_SMOOTH_HINT, GL_NICEST);
    glPointSize(10.0f);

    // Mjuka, fina linjer!
    glEnable   (GL_LINE_SMOOTH);
    glHint     (GL_LINE_SMOOTH_HINT, GL_NICEST);
    glLineWidth(3.0f);
}

static void InitPixelFormat() {
    PIXELFORMATDESCRIPTOR pfd;

    pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion   = 1;
    pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cAlphaBits = 8;
    pfd.cDepthBits = 32;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(graphics.hdc, &pfd);
    if (pixelFormat == 0)
        Error("ChoosePixelFormat() failed");

    if (!SetPixelFormat(graphics.hdc, pixelFormat, &pfd))
        Error("SetPixelFormat() failed");
}

static void InitWindow() {
    WNDCLASSEX wcx;

    wcx.cbSize        = sizeof(WNDCLASSEX);
    wcx.style         = CS_HREDRAW | CS_VREDRAW;
    wcx.lpfnWndProc   = WindowProc;
    wcx.cbClsExtra    = 0;
    wcx.cbWndExtra    = 0;
    wcx.hInstance     = GetModuleHandle(NULL);
    wcx.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wcx.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wcx.lpszMenuName  = NULL;
    wcx.lpszClassName = ClassName;
    wcx.hIconSm       = NULL;


    if (!RegisterClassEx(&wcx))
        Error("RegisterClassEx() failed");


    DWORD dwStyle = WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;

    RECT rect = { 0, 0, 600, 600 };

    if (!AdjustWindowRectEx(&rect, dwStyle, FALSE, WS_EX_TOPMOST))
        Error("AdjustWindowRectEx() failed");

    graphics.hwnd = CreateWindowEx(WS_EX_TOPMOST,
                                   ClassName,
                                   WindowTitle,
                                   dwStyle,
                                   CW_USEDEFAULT,
                                   CW_USEDEFAULT,
                                   rect.right - rect.left,
                                   rect.bottom - rect.top,
                                   HWND_DESKTOP,
                                   NULL,
                                   wcx.hInstance,
                                   NULL);

    if (!graphics.hwnd)
        Error("CreateWindowEx() failed");

    ShowWindow(graphics.hwnd, SW_SHOW);
    SetFocus(graphics.hwnd);

    graphics.hdc = GetDC(graphics.hwnd);
}

/*--------------------------------------
 * Function: InitGraphics()
 * Parameters:
 *
 * Description:
 *   Initialiserar grafiksystemet.
 *------------------------------------*/
void InitGraphics() {
    if (graphics.initialized)
        Error("Graphics already initialized");

    InitWindow();
    InitPixelFormat();
    InitOpenGL();

    SetFrameRate(30);

    QueryPerformanceFrequency(&graphics.perfCounterFreq);

    graphics.initialized = TRUE;
    graphics.windowOpen  = TRUE;
}

void SetFrameRate(int fps) {
    CheckInitGraphics();

    graphics.invFrameRate = 1000000 / fps;
}

void SetColor(float red, float green, float blue, float alpha) {
    CheckInitGraphics();

    glColor4f(red, green, blue, alpha);
}

void ClearDisplay(float red, float green, float blue) {
    CheckInitGraphics();

    glClearColor(red, green, blue, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/*--------------------------------------
 * Function: DrawHull()
 * Parameters:
 *   hull  Det hölje som ska ritas upp.
 *
 * Description:
 *   Ritar upp ett hölje.
 *------------------------------------*/
void DrawHull(hullT hull) {
    CheckInitGraphics();

    for (int i = 0; i < hull.numLines; i++)
        DrawLine(hull.lines[i]);
}

/*--------------------------------------
 * Function: DrawLine()
 * Parameters:
 *   line  Den linje som ska ritas upp.
 *
 * Description:
 *   Ritar upp en linje.
 *------------------------------------*/
void DrawLine(lineT line) {
    CheckInitGraphics();

    glBegin(GL_LINES);
    glVertex2f(line.a->x, line.a->y);
    glVertex2f(line.b->x, line.b->y);
    glEnd();
}

/*--------------------------------------
 * Function: DrawPoint()
 * Parameters:
 *   point  Den punkt som ska ritas upp.
 *
 * Description:
 *   Ritar upp en punkt.
 *------------------------------------*/
void DrawPoint(pointT point) {
    CheckInitGraphics();

    glBegin(GL_POINTS);
    glVertex2f(point.x, point.y);
    glEnd();
}

/*--------------------------------------
 * Function: DrawPoints()
 * Parameters:
 *   ps  Den uppsättning punkter som ska ritas upp.
 *
 * Description:
 *   Ritar en uppsättning punkter.
 *------------------------------------*/
void DrawPoints(pointsetT ps) {
    CheckInitGraphics();

    for (int i = 0; i < ps.numPoints; i++)
        DrawPoint(ps.points[i]);
}

/*--------------------------------------
 * Function: UpdateDisplay()
 * Parameters:
 *
 * Description:
 *   Uppdaterar ritytan.
 *------------------------------------*/
void UpdateDisplay() {
    LARGE_INTEGER perfCount;

    CheckInitGraphics();

    SwapBuffers(graphics.hdc);

    while (TRUE) {
        MSG msg;
        while (PeekMessage(&msg, graphics.hwnd, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage (&msg);
        }

        QueryPerformanceCounter(&perfCount);
        perfCount.QuadPart -= graphics.lastUpdate.QuadPart;
        perfCount.QuadPart *= 1000000;
        perfCount.QuadPart /= graphics.perfCounterFreq.QuadPart;

        if (perfCount.QuadPart > graphics.invFrameRate)
            break;
    }

    QueryPerformanceCounter(&graphics.lastUpdate);
}

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
void OnKeyPress(char c, actionT cb, void *arg) {
    CheckInitGraphics();

    graphics.keyPressCB [c] = cb;
    graphics.keyPressArg[c] = arg;
}

/*--------------------------------------
 * Function: WindowIsOpen()
 * Parameters:
 *
 * Description:
 *   Returnerar sant om grafikfönstret är öppet.
 *------------------------------------*/
bool WindowIsOpen() {
    CheckInitGraphics();

    return graphics.windowOpen;
}
