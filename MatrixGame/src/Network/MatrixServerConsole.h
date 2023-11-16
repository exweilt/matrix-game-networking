#pragma once

#include "d3d9.h"
#include "d3dx9tex.h"

#define WM_CONSOLE_OUTPUT (WM_USER + 7)

extern HWND g_ConsoleWnd;

void ConsoleWindowInit(HINSTANCE hInstance);

LRESULT CALLBACK ConsoleWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
