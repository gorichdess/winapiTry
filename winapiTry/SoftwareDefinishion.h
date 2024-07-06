#pragma once
#include "3DEngine.h"

int ScreenWidth=700;
int ScreenHeight = 700;
TryEngine3D te3d;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
WNDCLASS NewWindowClass(HBRUSH hBrush, HCURSOR hCursor, HINSTANCE hInstance, HICON hIcon, LPCWSTR Name, WNDPROC Procedure);
