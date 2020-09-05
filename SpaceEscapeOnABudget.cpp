// SpaceEscapeOnABudget.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "SpaceEscapeOnABudget.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SPACEESCAPEONABUDGET, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPACEESCAPEONABUDGET));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPACEESCAPEONABUDGET));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPACEESCAPEONABUDGET);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   RECT screen;
   HWND desktop = GetDesktopWindow();

   GetWindowRect(desktop, &screen);

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_THICKFRAME | WS_MAXIMIZEBOX | WS_SYSMENU,
      (screen.right - width) / 2, (screen.bottom - height) / 2, width, height, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        SetTimer(hWnd, NULL, 20, NULL);
        break;
    }
    case WM_MOUSEMOVE:
    {
        if (MouseDown)
        {
            Mouse.x = GET_X_LPARAM(lParam);
            Mouse.y = GET_Y_LPARAM(lParam);
        }
        break;
    }
    case WM_LBUTTONUP:
    {
        MouseDown = FALSE;
        break;
    }
    case WM_LBUTTONDOWN:
    {
        MouseDown = TRUE;

        Mouse.x = GET_X_LPARAM(lParam);
        Mouse.y = GET_Y_LPARAM(lParam);
        break;
    }
    case WM_SIZE:
    {
        RECT size;
        GetWindowRect(hWnd, &size);

        width = size.right;
        height = size.bottom;

        RePaint();
        break;
    }
    case WM_SIZING:
    {
        RECT size;
        GetWindowRect(hWnd, &size);

        width  = size.right;
        height = size.bottom;

        RePaint();
        break;
    }
    case WM_KEYUP:
    {
        if (wParam == VK_ESCAPE)
            PostQuitMessage(0);
        break;
    }
    case WM_TIMER:
    {
        UpdatePlayer();

        if (rand() % 25 == 0)
        {
            int size = rand() % 50 + 50;

            Enemies.push_back((Enemy*) new Astroid({ width, rand() % height }, { size, size }));
        }

        for (int i = Enemies.size() - 1; i >= 0; i--)
            Enemies[i]->Update(i);

        RePaint();
        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        FillRect(0, 0, width, height, 0x393939);

        for (size_t i = 0; i < Enemies.size(); i++)
            Enemies[i]->Draw(hdc);

        DrawPlayer(hdc);
        
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
