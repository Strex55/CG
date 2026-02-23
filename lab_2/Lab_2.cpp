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
auto_ptr<Bitmap> g_pBitmap;                     // для хранения картинки
ULONG_PTR g_gdiplusToken;                       // для инициализации GDI+
void OnOpenFile(HWND hWnd);
void DrawImage(HWND hWnd, HDC hdc);
bool g_bDragging = false;          
POINT g_dragStart = { 0, 0 };         
POINT g_imageOffset = { 0, 0 };      
bool g_bDragMode = true;

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
    LoadStringW(hInstance, IDC_LAB2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, NULL);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
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

    return (int) msg.wParam;
    GdiplusShutdown(g_gdiplusToken);
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

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
    switch (message)
    {
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
            OnOpenFile(hWnd);
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
        DrawImage(hWnd, hdc);
        EndPaint(hWnd, &ps);
    }
    break;

    case WM_SIZE:  
    {
        if (!g_bDragMode) {
            InvalidateRect(hWnd, NULL, TRUE);
        }
    }
    break;

    case WM_LBUTTONDOWN:
    {
        if (g_pBitmap.get() == NULL) break;

        g_bDragging = true;
        g_dragStart.x = LOWORD(lParam);
        g_dragStart.y = HIWORD(lParam);
        SetCapture(hWnd);
        g_bDragMode = true;

        if (g_imageOffset.x == 0 && g_imageOffset.y == 0 && g_pBitmap.get() != NULL)
        {
            RECT clientRect;
            GetClientRect(hWnd, &clientRect);
            int winW = clientRect.right - clientRect.left;
            int winH = clientRect.bottom - clientRect.top;

            int imgW = g_pBitmap->GetWidth();
            int imgH = g_pBitmap->GetHeight();

            g_imageOffset.x = (winW - imgW) / 2;
            g_imageOffset.y = (winH - imgH) / 2;
        }
    }
    break;

    case WM_MOUSEMOVE:
    {
        if (g_bDragging && g_pBitmap.get() != NULL)
        {
            int mouseX = LOWORD(lParam);
            int mouseY = HIWORD(lParam);

            g_imageOffset.x += (mouseX - g_dragStart.x);
            g_imageOffset.y += (mouseY - g_dragStart.y);

            g_dragStart.x = mouseX;
            g_dragStart.y = mouseY;

            InvalidateRect(hWnd, NULL, TRUE);
        }
    }
    break;

    case WM_LBUTTONUP:
    {
        g_bDragging = false;
        ReleaseCapture();
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void OnOpenFile(HWND hWnd)
{
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
            g_pBitmap = auto_ptr<Bitmap>(new Bitmap(
                img.GetWidth(),
                img.GetHeight(),
                PixelFormat32bppARGB
            ));

            Graphics g(g_pBitmap.get());
            g.DrawImage(&img, 0, 0, img.GetWidth(), img.GetHeight());

            // Сбрасываем в центрированный режим при открытии новой картинки
            g_bDragMode = false;
            g_imageOffset.x = 0;
            g_imageOffset.y = 0;

            InvalidateRect(hWnd, NULL, TRUE);
        }
        else
        {
            MessageBox(hWnd, L"Не удалось загрузить изображение", L"Ошибка", MB_OK | MB_ICONERROR);
        }
    }
}

void DrawImage(HWND hWnd, HDC hdc)
{
    if (g_pBitmap.get() == NULL) return;

    Graphics g(hdc);
    g.SetSmoothingMode(SmoothingModeHighQuality);
    g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

    RECT clientRect;
    GetClientRect(hWnd, &clientRect);
    int winW = clientRect.right - clientRect.left;
    int winH = clientRect.bottom - clientRect.top;

    int imgW = g_pBitmap->GetWidth();
    int imgH = g_pBitmap->GetHeight();

    if (imgW <= 0 || imgH <= 0) return;

    int drawX, drawY, drawW, drawH;

    if (g_bDragMode) {
        // перетаскиваем
        drawW = imgW;
        drawH = imgH;
        drawX = g_imageOffset.x;
        drawY = g_imageOffset.y;
    }
    else {
        // центрируем 
        float scaleX = (float)winW / imgW;
        float scaleY = (float)winH / imgH;
        float scale = min(scaleX, scaleY);

        drawW = (int)(imgW * scale);
        drawH = (int)(imgH * scale);
        drawX = (winW - drawW) / 2;
        drawY = (winH - drawH) / 2;

        g_imageOffset.x = 0;
        g_imageOffset.y = 0;
    }

    if (g_pBitmap->GetPixelFormat() & PixelFormatAlpha)
    {
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
    }

    g.DrawImage(g_pBitmap.get(), drawX, drawY, drawW, drawH);
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
