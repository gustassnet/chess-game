#include <windows.h>
#include <gdiplus.h>
#include "resources.h"
#include "chess.h"
#include "draw.h"

using namespace Gdiplus;
using namespace std;

vector<pair<int, int>> possibleMoves;
short selectedColumn = -1;
short selectedRow = -1;

short getColumn(LPARAM lParam) {
    int xPos = lParam & 0x0000FFFF;
    short column = (xPos - board_xPos) / box_width;
    if ((xPos - board_xPos) >= 0 && column <= 7 && column >= 0) {
        return column;
    }
    return -1;
}

short getRow(LPARAM lParam) {
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
            HDC hdc = GetDC(hwnd);
            short prevColumn = selectedColumn;
            short prevRow = selectedRow;

            selectedColumn = getColumn(lParam);
            selectedRow = getRow(lParam);

            if (prevColumn != -1 && prevRow != -1) {
                drawSquare(hdc, getPieceId(prevColumn, prevRow), prevColumn, prevRow, selectedColumn, selectedRow);
            }

            // clear possible moves
            for (auto move : possibleMoves) {
                drawSquare(hdc, getPieceId(move.first, move.second), move.first, move.second, selectedColumn, selectedRow);
            }

            // try make move
            if (makeMove(possibleMoves, prevColumn, prevRow, selectedColumn, selectedRow)){
                drawSquare(hdc, getPieceId(selectedColumn, selectedRow), selectedColumn, selectedRow, selectedColumn, selectedRow);
                drawSquare(hdc, getPieceId(prevColumn, prevRow), prevColumn, prevRow, selectedColumn, selectedRow);
            }

            // draw new possible moves
            if (selectedColumn != -1 && selectedRow != -1) {
                possibleMoves = getValidMoves(selectedColumn, selectedRow);
                drawPossibleMove(hdc, possibleMoves);
            }

            if (selectedColumn != -1 && selectedRow != -1) {
                drawSquare(hdc, getPieceId(selectedColumn, selectedRow), selectedColumn, selectedRow, selectedColumn, selectedRow);
            }

            ReleaseDC(hwnd, hdc);

            return 0;
        }
        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            drawOuter(hdc);
            drawInner(hdc);
            drawCoordinates(hdc);
            // Columns
            for (unsigned int i = 0; i < 8; ++i) {
                //Rows
                for (unsigned int j = 0; j < 8; ++j) {
                    drawSquare(hdc, getPieceId(i, j), i, j, selectedColumn, selectedRow);
                }
            }

            EndPaint(hwnd, &ps);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // Load images and initialize the board
    loadImages();
    initBoard();

    // Create Window Class
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

    GdiplusShutdown(gdiplusToken);

    return 0;
}