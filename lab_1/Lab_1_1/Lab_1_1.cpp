// Lab_1_1.cpp : Defines the entry point for the application.

#include "framework.h"
#include "Lab_1_1.h"

#include <windows.h>

#define MAX_LOADSTRING 100

#define ANIMATION_TIMER 1

struct JumpingLetter {
    float baseY;      
    float currentY;   
    float speed;   
    int colorIndex;
};

COLORREF colors[3] = {
    RGB(255, 0, 0),   
    RGB(0, 255, 0),   
    RGB(0, 0, 255)    
};

JumpingLetter letterK = { 200, 200, 0, 0 };
JumpingLetter letterN = { 200, 200, 0, 1 };
JumpingLetter letterA = { 200, 200, 0, 2 };



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
    LoadStringW(hInstance, IDC_LAB11, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB11));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB11));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB11);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstance, nullptr);

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
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...

            HPEN hBlackPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
            SelectObject(hdc, hBlackPen);

            // Буква К
            HBRUSH hRedBrush = CreateSolidBrush(colors[letterK.colorIndex]);
            SelectObject(hdc, hRedBrush);
      
            Rectangle(hdc, 160, (int)letterK.currentY, 180, (int)letterK.currentY + 150);

            POINT upperK[4] = {
                {175, (int)letterK.currentY + 50},   
                {250, (int)letterK.currentY},        
                {250, (int)letterK.currentY + 20},   
                {175, (int)letterK.currentY + 70}
            };
            Polygon(hdc, upperK, 4);

            POINT lowerK[4] = {
                {175, (int)letterK.currentY + 70},    
                {250, (int)letterK.currentY + 130},  
                {250, (int)letterK.currentY + 150},  
                {175, (int)letterK.currentY + 90}     
            };
            Polygon(hdc, lowerK, 4);

            DeleteObject(hRedBrush);

            // Буква Н 
            HBRUSH hGreenBrush = CreateSolidBrush(colors[letterN.colorIndex]);
            SelectObject(hdc, hGreenBrush);

            Rectangle(hdc, 300, (int)letterN.currentY, 320, (int)letterN.currentY + 150);
      
            Rectangle(hdc, 380, (int)letterN.currentY, 400, (int)letterN.currentY + 150);
         
            Rectangle(hdc, 300, (int)letterN.currentY + 65, 400, (int)letterN.currentY + 85);

            DeleteObject(hGreenBrush);

            // Буква А
            HBRUSH hBlueBrush = CreateSolidBrush(colors[letterA.colorIndex]);
            SelectObject(hdc, hBlueBrush);

            POINT leftA[4] = {
                {470, (int)letterA.currentY + 150},   
                {490, (int)letterA.currentY + 150},   
                {530, (int)letterA.currentY + 20},    
                {510, (int)letterA.currentY}
            };
            Polygon(hdc, leftA, 4);

            POINT rightA[4] = {
                {510, (int)letterA.currentY},          
                {530, (int)letterA.currentY + 20},    
                {570, (int)letterA.currentY + 150},   
                {550, (int)letterA.currentY + 150}
            };
            Polygon(hdc, rightA, 4);

            Rectangle(hdc, 500, (int)letterA.currentY + 80, 540, (int)letterA.currentY + 95);

            DeleteObject(hBlueBrush);

            // Очищаем перо
            SelectObject(hdc, GetStockObject(DC_PEN));
            DeleteObject(hBlackPen);


            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        KillTimer(hWnd, ANIMATION_TIMER);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
 
    case WM_CREATE:
        // таймер, который срабатывает каждые 30 миллисекунд
        SetTimer(hWnd, ANIMATION_TIMER, 30, NULL);
        return 0;

    case WM_TIMER:
    {
        // Для буквы К
        letterK.currentY += letterK.speed;
        letterK.speed += 0.5f; 

        
        if (letterK.currentY > letterK.baseY + 40) {
            letterK.currentY = letterK.baseY;
            letterK.speed = -10.0f; 
            letterK.colorIndex = (letterK.colorIndex + 1) % 3;
        }

        // Для буквы Н 
        letterN.currentY += letterN.speed;
        letterN.speed += 0.5f; 

        if (letterN.currentY > letterN.baseY + 50) {
            letterN.currentY = letterN.baseY;
            letterN.speed = -10.0f; 
            letterN.colorIndex = (letterN.colorIndex + 1) % 3;
        }

        // Для буквы А
        letterA.currentY += letterA.speed;
        letterA.speed += 0.5f; 

        if (letterA.currentY > letterA.baseY + 60) {
            letterA.currentY = letterA.baseY;
            letterA.speed = -10.0f; 
            letterA.colorIndex = (letterA.colorIndex + 1) % 3;
        }

        // перерисовать окно
        InvalidateRect(hWnd, NULL, TRUE);
    }
    return 0;
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
