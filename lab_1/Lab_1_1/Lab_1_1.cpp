// Lab_1_1.cpp : Defines the entry point for the application.

#include "framework.h"
#include "Lab_1_1.h"
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#define MAX_LOADSTRING 100
#define ANIMATION_TIMER 1
#define WINDOW_CLASS_NAME  L"Lab_1_1_Window"


struct JumpingLetter {
    float baseY;      
    float currentY;   
    float speed;   
    int colorIndex;
};

// ✓ не использовать глобальные переменные!
struct WindowData {
    JumpingLetter letterK;
    JumpingLetter letterN;
    JumpingLetter letterA;
    HINSTANCE hInst;
    WCHAR szTitle[MAX_LOADSTRING];     
    COLORREF colors[3];
};

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

    // Register the window class FIRST!
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
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

    return (int)msg.wParam;
}


//  PURPOSE: Registers the window class.
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB11));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB11);
    wcex.lpszClassName = WINDOW_CLASS_NAME;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//   PURPOSE: Saves instance handle and creates main window
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    WindowData* pData = new WindowData;
    pData->hInst = hInstance;
    pData->letterK = { 200, 200, 0, 0 };
    pData->letterN = { 200, 200, 0, 1 };
    pData->letterA = { 200, 200, 0, 2 };
    pData->colors[0] = RGB(255, 0, 0);
    pData->colors[1] = RGB(0, 255, 0);
    pData->colors[2] = RGB(0, 0, 255);
    LoadStringW(hInstance, IDS_APP_TITLE, pData->szTitle, MAX_LOADSTRING);

    HWND hWnd = CreateWindowW(
        WINDOW_CLASS_NAME,
        pData->szTitle,        
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        nullptr, nullptr, hInstance, pData
    );

   if (!hWnd)
   {
      delete pData;
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

void UpdateAnimation(WindowData* pData, float deltaTime) {
    if (pData == NULL) return;

    const float GRAVITY = 800.0f;        
    const float JUMP_SPEED = -450.0f;    
    const float MAX_HEIGHT[] = { 40, 50, 60 }; 

    JumpingLetter* letters[] = {
        &pData->letterK,
        &pData->letterN,
        &pData->letterA
    };

    for (int i = 0; i < 3; i++) {
        // Обновляем позицию и скорость с учетом времени
        letters[i]->currentY += letters[i]->speed * deltaTime;
        letters[i]->speed += GRAVITY * deltaTime;

        if (letters[i]->currentY > letters[i]->baseY + MAX_HEIGHT[i]) {
            letters[i]->currentY = letters[i]->baseY;    
            letters[i]->speed = JUMP_SPEED;              
            letters[i]->colorIndex = (letters[i]->colorIndex + 1) % 3;
        }
    }
}

//  PURPOSE: Processes messages for the main window.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    WindowData* pData = (WindowData*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    if (pData == NULL && message != WM_DESTROY && message != WM_CREATE) {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                if (pData != NULL) {
                    DialogBox(pData->hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                }
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
        // ✓вернуть контекст устройства в прошщлое состояние (selectBrush сохрнаить в переменную)
        HPEN hBlackPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hBlackPen);

        if (pData == NULL) {
            EndPaint(hWnd, &ps);
            break;
        }
        // Буква К 
        HBRUSH hRedBrush = CreateSolidBrush(pData->colors[pData->letterK.colorIndex]);
        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hRedBrush);

        Rectangle(hdc, 160, (int)pData->letterK.currentY, 180, (int)pData->letterK.currentY + 150);

        POINT upperK[4] = {
            {175, (int)pData->letterK.currentY + 50},
            {250, (int)pData->letterK.currentY},
            {250, (int)pData->letterK.currentY + 20},
            {175, (int)pData->letterK.currentY + 70}
        };
        Polygon(hdc, upperK, 4);

        POINT lowerK[4] = {
            {175, (int)pData->letterK.currentY + 70},
            {250, (int)pData->letterK.currentY + 130},
            {250, (int)pData->letterK.currentY + 150},
            {175, (int)pData->letterK.currentY + 90}
        };
        Polygon(hdc, lowerK, 4);

        SelectObject(hdc, hOldBrush);
        DeleteObject(hRedBrush);

        // Буква Н 
        HBRUSH hGreenBrush = CreateSolidBrush(pData->colors[pData->letterN.colorIndex]);
        hOldBrush = (HBRUSH)SelectObject(hdc, hGreenBrush);

        Rectangle(hdc, 300, (int)pData->letterN.currentY, 320, (int)pData->letterN.currentY + 150);
        Rectangle(hdc, 380, (int)pData->letterN.currentY, 400, (int)pData->letterN.currentY + 150);
        Rectangle(hdc, 300, (int)pData->letterN.currentY + 65, 400, (int)pData->letterN.currentY + 85);

        SelectObject(hdc, hOldBrush);
        DeleteObject(hGreenBrush);

        // Буква А
        HBRUSH hBlueBrush = CreateSolidBrush(pData->colors[pData->letterA.colorIndex]);
        hOldBrush = (HBRUSH)SelectObject(hdc, hBlueBrush);

        POINT leftA[4] = {
            {470, (int)pData->letterA.currentY + 150},
            {490, (int)pData->letterA.currentY + 150},
            {530, (int)pData->letterA.currentY + 20},
            {510, (int)pData->letterA.currentY}
        };
        Polygon(hdc, leftA, 4);

        POINT rightA[4] = {
            {510, (int)pData->letterA.currentY},
            {530, (int)pData->letterA.currentY + 20},
            {570, (int)pData->letterA.currentY + 150},
            {550, (int)pData->letterA.currentY + 150}
        };
        Polygon(hdc, rightA, 4);

        Rectangle(hdc, 500, (int)pData->letterA.currentY + 80, 540, (int)pData->letterA.currentY + 95);

        SelectObject(hdc, hOldBrush);
        DeleteObject(hBlueBrush);

        SelectObject(hdc, hOldPen);
        DeleteObject(hBlackPen);

        EndPaint(hWnd, &ps);
    }
    break;
    
    case WM_DESTROY:
        KillTimer(hWnd, ANIMATION_TIMER);
        if (pData != NULL) {
            delete pData;
        }
        PostQuitMessage(0);
        break;
 
    case WM_CREATE:
        {
        CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
        WindowData* pNewData = (WindowData*)pCreate->lpCreateParams;
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pNewData);
        pData = pNewData;
        SetTimer(hWnd, ANIMATION_TIMER, 30, NULL);
        return 0;
    }

    case WM_TIMER:
    {
        static DWORD lastTime = timeGetTime();
        DWORD currentTime = timeGetTime();
        float deltaTime = (currentTime - lastTime) / 1000.0f;

        if (deltaTime > 0.1f) deltaTime = 0.1f;

        lastTime = currentTime;

        UpdateAnimation(pData, deltaTime);

        InvalidateRect(hWnd, NULL, TRUE);
    }
    break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
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