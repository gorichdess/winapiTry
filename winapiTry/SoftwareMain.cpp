#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <objidl.h>
#include <gdiplus.h>
#include "SoftwareDefinishion.h"
#include "resource.h"
#include "3DEngine.h"
using namespace Gdiplus;
#pragma comment (lib, "Gdiplus.lib")

GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR gdiplusToken;

VOID OnPaint(HDC hdc)
{
    Graphics graphics(hdc);

    SolidBrush blackBrush(Color(255, 0, 0, 0)); 
    graphics.FillRectangle(&blackBrush, 0, 0, ScreenWidth, ScreenHeight+1);// Black BG

    Pen pen(Color(255, 255, 255, 255));

    //Draw triangles
    for (triangle tri : te3d.meshCube.tris) {
        triangle trian = te3d.OnUserUpdate(tri, (float)ScreenWidth, (float)ScreenHeight);
        graphics.DrawLine(&pen, trian.p[0].x, trian.p[0].y, trian.p[1].x, trian.p[1].y);
        graphics.DrawLine(&pen, trian.p[1].x, trian.p[1].y, trian.p[2].x, trian.p[2].y);
        graphics.DrawLine(&pen, trian.p[2].x, trian.p[2].y, trian.p[0].x, trian.p[0].y);
    }
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    const wchar_t CLASS_NAME[] = L"Sample Window Class";
    WNDCLASS SofrwareMainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInstance, LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)), CLASS_NAME, WindowProc);

    if (!RegisterClass(&SofrwareMainClass)) {
        return -1;
    }

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"3D Cube",                     // Window text
        WS_OVERLAPPED | WS_CAPTION |
        WS_SYSMENU | WS_MINIMIZEBOX,            // Window style (no window resizing)
        100, 100, ScreenWidth, ScreenHeight+40,             // Size and position
        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // «апуск цикла обработки сообщений
    MSG SofrwareMainMsg = { };
    while (GetMessage(&SofrwareMainMsg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&SofrwareMainMsg);
        DispatchMessage(&SofrwareMainMsg);
    }

    GdiplusShutdown(gdiplusToken);
    return 0;
}

WNDCLASS NewWindowClass(HBRUSH BGColor, HCURSOR Cursor, HINSTANCE hInst, HICON Icon, LPCWSTR Name, WNDPROC Procedure) {
    WNDCLASS wc = { };

    wc.hIcon = Icon;
    wc.hCursor = Cursor;
    wc.lpfnWndProc = Procedure;
    wc.hInstance = hInst;
    wc.hbrBackground = BGColor;
    wc.lpszClassName = Name;

    return wc;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static DWORD lastTick = GetTickCount();
    switch (uMsg)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        OnPaint(hdc);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_CREATE:
        SetTimer(hwnd, 1, 70, NULL); // timer
        te3d.OnCreate(800, 700);
        break;
    case WM_TIMER:
        if (wParam == 1) {
            DWORD currentTick = GetTickCount();
            float fElapsedTime = (currentTick - lastTick) / 1000.0f;
            lastTick = currentTick;
            te3d.SetTime(fElapsedTime); 

            InvalidateRect(hwnd, NULL, TRUE); // ѕерерисовать окно посылает сообщение WM_PAINT
        }
        break;
    case WM_DESTROY:
        KillTimer(hwnd, 1); // Delete timer
        PostQuitMessage(0);
        return 0;
    case WM_CLOSE:
        if (MessageBox(hwnd, L"Really want to quit?", L"3D Cube", MB_OKCANCEL) == IDOK)
        {
            DestroyWindow(hwnd);
        }
        return 0;
    default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}
