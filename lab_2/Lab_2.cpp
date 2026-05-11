// Lab_2.cpp : Defines the entry point for the application.

#include "Lab_2.h"
#include "framework.h"
#include <objidl.h>
#include <memory>
#include <gdiplus.h>
#include <comdef.h> 

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;
using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
ULONG_PTR g_gdiplusToken;                       // для инициализации GDI+

struct AppState
{
    auto_ptr<Bitmap> pBitmap;

    bool bDragMode = true;
    bool bDragging = false;

    POINT dragStart = { 0, 0 };
    POINT imageOffset = { 0, 0 };

    auto_ptr<Bitmap> pBackground;  
    bool bNeedRedrawBackground = true;

    AppState() = default;
};


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void OnOpenFile(HWND hWnd, AppState* pState);
void DrawImage(HWND hWnd, HDC hdc, AppState* pState);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Initialize GDI+
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, NULL);

    AppState* pState = new AppState();

    // Perform application initialization:
    if (!InitInstance(hInstance, nCmdShow))
    {
        delete pState;
        return FALSE;
    }

    HWND hWnd = FindWindowW(szWindowClass, szTitle);
    if (hWnd)
    {
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pState);
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB2));

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

    delete pState;
    GdiplusShutdown(g_gdiplusToken);

    return (int)msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = sizeof(AppState*);  
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB2));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_LAB2);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
     AppState* pState = (AppState*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

    switch (message)
    {
    case WM_NCCREATE:
    {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        case ID_FILE_OPEN:
            if (pState) OnOpenFile(hWnd, pState);
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

        HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
        HPEN hOldPen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));

        if (pState) DrawImage(hWnd, hdc, pState);

        SelectObject(hdc, hOldBrush);
        SelectObject(hdc, hOldPen);

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_SIZE:
    {
        if (pState) {
            pState->bNeedRedrawBackground = true;  
            if (!pState->bDragMode) {
                InvalidateRect(hWnd, NULL, TRUE);
            }
        }
        break;
    }

    case WM_LBUTTONDOWN:
    {
        if (pState && pState->pBitmap.get() != NULL)
        {
            pState->bDragging = true;
            pState->dragStart.x = LOWORD(lParam);
            pState->dragStart.y = HIWORD(lParam);
            SetCapture(hWnd);
            pState->bDragMode = true;
        }
        break;
    }

    case WM_MOUSEMOVE:
    {
        if (pState && pState->bDragging && pState->pBitmap.get() != NULL)
        {
            int mouseX = LOWORD(lParam);
            int mouseY = HIWORD(lParam);

            pState->imageOffset.x += (mouseX - pState->dragStart.x);
            pState->imageOffset.y += (mouseY - pState->dragStart.y);

            pState->dragStart.x = mouseX;
            pState->dragStart.y = mouseY;

            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    }

    case WM_LBUTTONUP:
    {
        if (pState)
        {
            pState->bDragging = false;
            ReleaseCapture();
        }
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


void OnOpenFile(HWND hWnd, AppState* pState)
{
    if (!pState) return;

    OPENFILENAME ofn = {};
    WCHAR fileName[MAX_PATH] = L"";

    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.hInstance = hInst;
    ofn.lpstrFile = fileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"Images (*.bmp;*.png;*.jpg)\0*.bmp;*.png;*.jpg\0All Files (*.*)\0*.*\0";
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

    if (GetOpenFileName(&ofn))
    {
        Image img(ofn.lpstrFile);

        if (img.GetLastStatus() == Ok)
        {
            pState->pBitmap = auto_ptr<Bitmap>(new Bitmap(
                img.GetWidth(),
                img.GetHeight(),
                PixelFormat32bppARGB
            ));

            Graphics g(pState->pBitmap.get());
            g.DrawImage(&img, 0, 0, img.GetWidth(), img.GetHeight());

            pState->bDragMode = false;
            pState->imageOffset.x = 0;
            pState->imageOffset.y = 0;

            InvalidateRect(hWnd, NULL, TRUE);
        }
        else
        {
            MessageBox(hWnd, L"Не удалось загрузить изображение", L"Ошибка", MB_OK | MB_ICONERROR);
        }
    }
}

void CreateBackground(HWND hWnd, AppState* pState)
{
    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    int winW = clientRect.right - clientRect.left;
    int winH = clientRect.bottom - clientRect.top;

    pState->pBackground = auto_ptr<Bitmap>(new Bitmap(winW, winH, PixelFormat32bppARGB));
    Graphics g(pState->pBackground.get());

    SolidBrush lightBrush(Color(200, 200, 200));
    SolidBrush darkBrush(Color(150, 150, 150));

    int cellSize = 20;
    for (int gridX = 0; gridX < winW; gridX += cellSize)
    {
        for (int gridY = 0; gridY < winH; gridY += cellSize)
        {
            if ((gridX / cellSize + gridY / cellSize) % 2 == 0)
                g.FillRectangle(&lightBrush, gridX, gridY, cellSize, cellSize);
            else
                g.FillRectangle(&darkBrush, gridX, gridY, cellSize, cellSize);
        }
    }

    pState->bNeedRedrawBackground = false;
}

void DrawImage(HWND hWnd, HDC hdc, AppState* pState)
{
    if (!pState || pState->pBitmap.get() == NULL) return;

    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    int winW = clientRect.right - clientRect.left;
    int winH = clientRect.bottom - clientRect.top;

    Bitmap memBitmap(winW, winH, PixelFormat32bppARGB);
    Graphics memGraphics(&memBitmap);

    memGraphics.SetSmoothingMode(SmoothingModeHighQuality);
    memGraphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);

    if (pState->bNeedRedrawBackground || !pState->pBackground.get()) {
        CreateBackground(hWnd, pState);
    }

    if (pState->pBackground.get()) {
        memGraphics.DrawImage(pState->pBackground.get(), 0, 0, winW, winH);
    }
    else {
        SolidBrush whiteBrush(Color(255, 255, 255));
        memGraphics.FillRectangle(&whiteBrush, 0, 0, winW, winH);
    }

    int imgW = pState->pBitmap->GetWidth();
    int imgH = pState->pBitmap->GetHeight();

    if (imgW <= 0 || imgH <= 0) return;

    float scaleX = (float)winW / imgW;
    float scaleY = (float)winH / imgH;
    float scale = min(scaleX, scaleY);

    int scaledW = (int)(imgW * scale);
    int scaledH = (int)(imgH * scale);

    int drawX, drawY;

    if (pState->bDragMode) {
        drawX = pState->imageOffset.x;
        drawY = pState->imageOffset.y;
    }
    else {
        drawX = (winW - scaledW) / 2;
        drawY = (winH - scaledH) / 2;

        pState->imageOffset.x = drawX;
        pState->imageOffset.y = drawY;
    }

    memGraphics.DrawImage(pState->pBitmap.get(), drawX, drawY, scaledW, scaledH);

    Graphics screenGraphics(hdc);
    screenGraphics.DrawImage(&memBitmap, 0, 0, winW, winH);
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