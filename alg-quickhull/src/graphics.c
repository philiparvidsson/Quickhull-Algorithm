//------------------------------------------------
// INCLUDES
//------------------------------------------------

#include "common.h"
#include "debug.h"
#include "graphics.h"
#include "math.h"

#include <tchar.h>
#include <Windows.h>

#include <gl/GL.h>
#include <gl/GLU.h>

#include <time.h>

#pragma comment(lib, "opengl32.lib")

//------------------------------------------------
// CONSTANTS
//------------------------------------------------

/*--------------------------------------
 * Constant: ClassName
 *
 * Description:
 *   Namnet på klassen som används för att skapa grafikfönstret.
 *------------------------------------*/
#define ClassName _T("alg-quickhull")

/*--------------------------------------
 * Constant: WindowName
 *
 * Description:
 *   Grafikfönstrets titel.
 *------------------------------------*/
#define WindowTitle _T("Quickhull Demo")

//------------------------------------------------
// TYPES
//------------------------------------------------

/*--------------------------------------
 * Type: graphicsT
 *
 * Description:
 *   Håller reda på information om grafiksystemet.
 *------------------------------------*/
typedef struct {
    bool initialized;
    bool window_open;
    HWND hwnd;
    HDC  hdc;
    int invFrameRate;
    clock_t lastUpdate;
} graphicsT;

//------------------------------------------------
// GLOBALS
//------------------------------------------------

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

static void CheckInitGraphics() {
#ifdef _DEBUG
    if (!graphics.initialized)
        Error("InitGraphics() must be called first");
#endif
}

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
        graphics.window_open = FALSE;

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
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
 * Parameters: hull  Det hölje som ska ritas upp.
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
 * Parameters: line  Den linje som ska ritas upp.
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
 * Parameters: point  Den punkt som ska ritas upp.
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
 * Parameters: ps  Den uppsättning punkter som ska ritas upp.
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
 * Function: InitGraphics()
 * Parameters:
 *
 * Description:
 *   Initialiserar grafiksystemet.
 *------------------------------------*/
void InitGraphics() {
    if (graphics.initialized)
        Error("Graphics already initialized");

    HINSTANCE  hinst = GetModuleHandle(NULL);
    WNDCLASSEX wcx;

    wcx.cbSize        = sizeof(WNDCLASSEX);
    wcx.style         = CS_HREDRAW | CS_VREDRAW;
    wcx.lpfnWndProc   = WindowProc;
    wcx.cbClsExtra    = 0;
    wcx.cbWndExtra    = 0;
    wcx.hInstance     = hinst;
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
                                   hinst,
                                   NULL);

    if (!graphics.hwnd)
        Error("CreateWindowEx() failed");

    ShowWindow(graphics.hwnd, SW_SHOW);

    PIXELFORMATDESCRIPTOR pfd;
    graphics.hdc   = GetDC(graphics.hwnd);

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

    HGLRC hglrc = wglCreateContext(graphics.hdc);

    if (!hglrc)
        Error("wglCreateContext() failed");

    if (!wglMakeCurrent(graphics.hdc, hglrc))
        Error("wglMakeCurrent() failed");

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glPointSize(6.0f);
    glLineWidth(1.0f);

    graphics.initialized = TRUE;
    graphics.window_open = TRUE;

    SetFrameRate(30);
}

void SetFrameRate(int fps) {
    CheckInitGraphics();

    graphics.invFrameRate = 1000.0f / (float)fps;
}

/*--------------------------------------
 * Function: UpdateDisplay()
 * Parameters:
 *
 * Description:
 *   Uppdaterar ritytan.
 *------------------------------------*/
void UpdateDisplay() {
    CheckInitGraphics();

    SwapBuffers(graphics.hdc);

    MSG msg;
    while (PeekMessage(&msg, graphics.hwnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    while (TRUE) {
        int timeMs = (1000 * (clock() - graphics.lastUpdate)) / CLOCKS_PER_SEC;
        if (timeMs > graphics.invFrameRate)
            break;
        Sleep(0);
    }

    graphics.lastUpdate = clock();
}

bool WindowIsOpen() {
    CheckInitGraphics();

    return graphics.window_open;
}
