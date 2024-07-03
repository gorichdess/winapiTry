#pragma once

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
WNDCLASS NewWindowClass(HBRUSH hBrush, HCURSOR hCursor, HINSTANCE hInstance, HICON hIcon, LPCWSTR Name, WNDPROC Procedure);
