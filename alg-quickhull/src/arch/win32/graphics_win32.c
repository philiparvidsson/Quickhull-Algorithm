/*------------------------------------------------------------------------------
 * File: graphics_win32.c
 * Created: September 30, 2015
 * Last changed: September 30, 2015
 *
 * Author(s): Philip Arvidsson (philip@philiparvidsson.com)
 *
 * Description:
 *   Win32-implementation av grafikbiblioteket.
 *
 * Changes:
 *
 *----------------------------------------------------------------------------*/

#ifdef _WIN32

/*------------------------------------------------
 * INCLUDES
 *----------------------------------------------*/

#include "core/common.h"
#include "core/debug.h"

#include <limits.h>

#include <tchar.h>
#include <windows.h>

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
#define WindowTitle _T("Quickhull Sandbox")

/*------------------------------------------------
 * GLOBALS
 *----------------------------------------------*/

/*--------------------------------------
 * Variable: microSecsPerFrame
 *
 * Description:
 *   Antal mikrosekunder som varje bildruta ska visas.
 *------------------------------------*/
static int microSecsPerFrame;

/*--------------------------------------
 * Variable: hdc
 *
 * Description:
 *   Grafikfönstrets DC (device context).
 *------------------------------------*/
static HDC hdc;

/*--------------------------------------
 * Variable: hwnd
 *
 * Description:
 *   Grafikfönstrets fönster-handtag (kommentarer på svenska äger).
 *------------------------------------*/
static HWND hwnd;

/*--------------------------------------
 * Variable: keyPressCB
 *
 * Description:
 *   Callback-funktioner för olika knapptryck.
 *------------------------------------*/
static actionT keyPressCB[UCHAR_MAX];

/*--------------------------------------
 * Variable: keyPressArg
 *
 * Description:
 *   Argumentet till de olika callback-funktionerna.
 *------------------------------------*/
static void *keyPressArg[UCHAR_MAX];

/*--------------------------------------
 * Variable: windowOpen
 *
 * Description:
 *   Indikerar om grafikfönstret är öppet.
 *------------------------------------*/
static bool windowOpen;

/*------------------------------------------------
 * FUNCTIONS
 *----------------------------------------------*/

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
    if (uMsg == WM_CLOSE) {
        // Fönstret stängdes.
        windowOpen = FALSE;
    }
    else if (uMsg == WM_CHAR) {
        // Knapptryck. Om repeated blir sant så ska vi inte anropa callback-
        // funktionen. Detta motverkar att man kan spamma funktionerna genom att
        // hålla ned en knapp.
        bool repeated = lParam & 0x40000000;
        if (!repeated) {
            char    c  = tolower(wParam);
            actionT cb = keyPressCB[c];
            if (cb)
                cb(keyPressArg[c]);
        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

/*--------------------------------------
 * Function: InitPixelFormat()
 * Parameters:
 *
 * Description:
 *   Ställer in lite grejer med fönstret så att OpenGL fungerar med det...
 *------------------------------------*/
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

    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    if (pixelFormat == 0)
        Error("ChoosePixelFormat() failed");

    if (!SetPixelFormat(hdc, pixelFormat, &pfd))
        Error("SetPixelFormat() failed");
}

/*--------------------------------------
 * Function: InitGraphicsImpl()
 * Parameters:
 *
 * Description:
 *   Initierar grafik-läget.
 *------------------------------------*/
void InitGraphicsImpl() {
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

    if (!AdjustWindowRectEx(&rect, dwStyle, FALSE, WS_EX_LEFT))
        Error("AdjustWindowRectEx() failed");

    hwnd = CreateWindowEx(WS_EX_LEFT,
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

    if (!hwnd)
        Error("CreateWindowEx() failed");

    ShowWindow(hwnd, SW_SHOW);
    SetFocus  (hwnd);

    hdc = GetDC(hwnd);

    InitPixelFormat();

    HGLRC hglrc = wglCreateContext(hdc);

    if (!hglrc)
        Error("wglCreateContext() failed");

    if (!wglMakeCurrent(hdc, hglrc))
        Error("wglMakeCurrent() failed");

    windowOpen = TRUE;
}

/*--------------------------------------
 * Function: WindowIsOpenImpl()
 * Parameters:
 *
 * Description:
 *   Returnerar sant om grafikfönstret är öppet.
 *------------------------------------*/
bool WindowIsOpenImpl() {
    return windowOpen;
}

/*----------------------------------------------------------------------------*/
/* Ritfunktioner för att rensa ritytan, rita punkter, linjer, trianglar m.m.  */
/*----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: UpdateDisplayImpl()
 * Parameters:
 *
 * Description:
 *   Uppdaterar ritytan.
 *------------------------------------*/
void UpdateDisplayImpl() {
    SwapBuffers(hdc);

    MSG msg;
    while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage (&msg);
    }
}

/*----------------------------------------------------------------------------*/
/* Funktioner för input, t ex associera knappar med callback-funktioner m.m.  */
/*----------------------------------------------------------------------------*/

/*--------------------------------------
 * Function: OnKeyPressImpl()
 * Parameters:
 *   c    Tecknet som ska associeras till en funktion.
 *   cb   Callback-funktionen som ska anropas när knappen med det specificerade
 *        tecknet trycks ned.
 *   arg  Det argument som ska skickas till callback-funktionen.
 *
 * Description:
 *   Registerar en callback-funktion för en viss knapp.
 *------------------------------------*/
void OnKeyPressImpl(char c, actionT cb, void *arg) {
    keyPressCB [c] = cb;
    keyPressArg[c] = arg;
}

#endif // _WIN32
