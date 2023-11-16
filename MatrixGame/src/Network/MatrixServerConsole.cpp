#include "MatrixServerConsole.h"
#include "3g.hpp"
#include <stupid_logger.hpp>

HWND g_ConsoleWnd = 0;

void ConsoleWindowInit(HINSTANCE hInstance) {
    WNDCLASSEX wcSecond = {sizeof(WNDCLASSEX)};
    wcSecond.lpfnWndProc = ConsoleWindowProc;
    wcSecond.hInstance = hInstance;
    wcSecond.lpszClassName = "Server Console Class";

    RegisterClassEx(&wcSecond);

    HWND hWndSecond = CreateWindow("Server Console Class", "Server Console", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                                   CW_USEDEFAULT, 600, 800, NULL, NULL, hInstance, NULL);

    if (!hWndSecond) {
        lgr.error("Error while creating console window");
        abort();
    }

    g_ConsoleWnd = hWndSecond;

    ShowWindow(hWndSecond, SW_SHOWNORMAL);
}

char *str = nullptr;

LRESULT CALLBACK ConsoleWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            if (str != nullptr) {
                TextOutA(hdc, 10, 10, str, strlen(str));
            }
            else {
                char text[] = "Start Message";
                TextOut(hdc, 10, 10, text, strlen(text));
            }

            EndPaint(hwnd, &ps);
            break;
        }
        case WM_CONSOLE_OUTPUT:
            char *receivedString = reinterpret_cast<char *>(lParam);

            str = receivedString;

            // Request a repaint of the window
            InvalidateRect(hwnd, NULL, TRUE);

            //MessageBox(hwnd, receivedString, ";)", MB_OK);

            //PAINTSTRUCT ps;
            //HDC hdc = BeginPaint(hwnd, &ps);

            //// Draw text on the second window
            //TextOut(hdc, 10, 10, receivedString, strlen(receivedString));

            //EndPaint(hwnd, &ps);
            ////lgr.info("&&&&&77777777 77777777");
            break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
