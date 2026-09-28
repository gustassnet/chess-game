#include <windows.h>

#define box_width 50

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            unsigned int x = 200;
            unsigned int y = 100;
            HBRUSH hbrush  = CreateSolidBrush(RGB(0, 128, 0));

            // Collumns
            for (unsigned int i = 0; i < 8; ++i) {
                //Rows
                for (unsigned int j = 0; j < 8; ++j) {
                    Rectangle(hdc, x + box_width * i, y + box_width * j, x + box_width * (i + 1), y + box_width * (j + 1));

                    if ((i % 2 == 0 && j % 2 == 0) || (i % 2 !=0 && j % 2 != 0)) {
                        RECT rect;
                        rect.left = x + box_width * i;
                        rect.top = y + box_width * j;
                        rect.right = x + box_width * (i + 1);
                        rect.bottom = y + box_width * (j + 1);
                        FillRect(hdc, &rect, hbrush);
                    }
                }
            }
            DeleteObject(hbrush);
            EndPaint(hwnd, &ps);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // 1. Create Window Class
    WNDCLASSW wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"ChessGame";
    HBRUSH hbrush = CreateSolidBrush(RGB(37, 43, 51));
    wc.hbrBackground = hbrush;

    RegisterClassW(&wc);

    // 2. Create window
    HWND hwnd = CreateWindowExW(
        0,
        L"ChessGame",
        L"Chess Game",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        800,
        600,

        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    // 3. Show window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // 4. Message loop
    MSG msg = { };

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}