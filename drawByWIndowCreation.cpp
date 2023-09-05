#define UNICODE
#include <windows.h>
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rect;
            GetClientRect(hwnd, &rect);
            DrawText(hdc, L"Hello, World!", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    const wchar_t* className = L"myWindowClass";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles
        className,                      // Window class
        L"Hello World Window",          // Window title
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, // Window style
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr,       // Parent window    
        nullptr,       // Menu
        hInstance,  // Instance handle
        nullptr       // Additional application data
    );

    if (hwnd == nullptr) {
        return 0;
    }

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
