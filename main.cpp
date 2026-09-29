#include <windows.h>

#define window_width 1200
#define window_height 800
#define box_width 70
#define board_xPos (window_width - 8 * box_width) / 2
#define board_yPos (window_height - 8 * box_width) / 2

#define GREEN RGB(107, 142, 78)
#define WHITE RGB(245, 245, 245)
#define YELLOW RGB(253, 228, 80)

short selectedCollumn = -1;
short selectedRow = -1;

short getCollumn (LPARAM lParam) {
    int xPos = lParam & 0x0000FFFF;
    short collumn = (xPos - board_xPos) / box_width;
    if ((xPos - board_xPos) >= 0 && collumn <= 7 && collumn >= 0) {
        return collumn;
    }
    return -1;
}

short getRow (LPARAM lParam) {
    int yPos = (lParam & 0xFFFF0000) >> 16;
    short row = (yPos - board_yPos) / box_width;
    if ((yPos - board_yPos) >= 0 && row <= 7 && row >= 0) {
        return row;
    }
    return -1;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_LBUTTONDOWN:
        {
            short collumn = getCollumn(lParam);
            short row = getRow(lParam);
            if (collumn != -1 && row != -1) {
                selectedCollumn = collumn;
                selectedRow = row;
                InvalidateRect(hwnd, NULL, TRUE);
            }
            return 0;
        }
        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            HBRUSH green  = CreateSolidBrush(GREEN);
            HBRUSH white  = CreateSolidBrush(WHITE);
            HBRUSH yellow = CreateSolidBrush(YELLOW);

            // Collumns
            for (unsigned int i = 0; i < 8; ++i) {
                //Rows
                for (unsigned int j = 0; j < 8; ++j) {
                    Rectangle(hdc, board_xPos + box_width * i, board_yPos + box_width * j, board_xPos + box_width * (i + 1), board_yPos + box_width * (j + 1));
                    RECT rect;
                    rect.left = board_xPos + box_width * i;
                    rect.top = board_yPos + box_width * j;
                    rect.right = board_xPos + box_width * (i + 1);
                    rect.bottom = board_yPos + box_width * (j + 1);
                    if (i == selectedCollumn && j == selectedRow) {
                        FillRect(hdc, &rect, yellow);
                    } else if ((i % 2 == 0 && j % 2 == 0) || (i % 2 !=0 && j % 2 != 0)) {
                        FillRect(hdc, &rect, white);
                    } else {
                        FillRect(hdc, &rect, green);
                    }
                }
            }
            DeleteObject(green);
            DeleteObject(white);
            DeleteObject(yellow);
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
        window_width,
        window_height,
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