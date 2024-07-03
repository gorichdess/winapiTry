#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include "SoftwareDefinishion.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS SofrwareMainClass = NewWindowClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInstance, LoadIcon(NULL, IDI_QUESTION), CLASS_NAME, WindowProc);

    if (!RegisterClass(&SofrwareMainClass)) {
        return -1;
    }


    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"WinApi Try",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        100, 100, 500, 250,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG SofrwareMainMsg = { };
    while (GetMessage(&SofrwareMainMsg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&SofrwareMainMsg);
        DispatchMessage(&SofrwareMainMsg);
    }

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
    switch (uMsg)
    {
    case WM_CREATE:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}