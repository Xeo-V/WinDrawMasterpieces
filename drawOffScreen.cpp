#include <windows.h>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            RECT rect;
            GetClientRect(hwnd, &rect);

            // Create an off-screen DC and Bitmap
            HDC hdcMem = CreateCompatibleDC(hdc);
            HBITMAP hbmMem = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
            HGDIOBJ hbmOld = SelectObject(hdcMem, hbmMem);

            // Draw transparent background
            SetBkMode(hdcMem, TRANSPARENT);

            // Create and select a large font
            HFONT hFont = CreateFontW(72, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET,
                                      OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                      DEFAULT_PITCH | FF_SWISS, L"Arial");
            SelectObject(hdcMem, hFont);

            // Draw opaque text
            SetTextColor(hdcMem, RGB(255, 0, 0)); // Text color: Red
            DrawTextW(hdcMem, L"Hello, World!", -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            // Transfer the off-screen DC to the screen
            BitBlt(hdc, 0, 0, rect.right, rect.bottom, hdcMem, 0, 0, SRCCOPY);

            // Clean up
            SelectObject(hdcMem, hbmOld);
            DeleteObject(hbmMem);
            DeleteObject(hFont);
            DeleteDC(hdcMem);

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
    const wchar_t* className = L"myTransparentTextWindowClass";
    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = className;

    if (!RegisterClassW(&wc)) {
        return 1;
    }

    HWND hwnd = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_LAYERED,
        className,
        L"Transparent Text Window",
        WS_POPUP,
        0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (hwnd == nullptr) {
        return 1;
    }

    // Set the transparency of the window to 70%
    SetLayeredWindowAttributes(hwnd, 0, (255 * 50) / 100, LWA_ALPHA);

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
