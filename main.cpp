#include <windows.h>
#include <gdiplus.h>
#include "resources.h"

using namespace Gdiplus;

#define window_width 1200
#define window_height 800
#define box_width 70
#define board_xPos (window_width - 8 * box_width) / 2
#define board_yPos (window_height - 8 * box_width) / 2

#define GREEN RGB(107, 142, 78)
#define WHITE RGB(245, 245, 245)
#define YELLOW RGB(253, 228, 80)

short selectedColumn = -1;
short selectedRow = -1;

Gdiplus::Image* whitePawn = nullptr;
Gdiplus::Image* whiteKnight = nullptr;
Gdiplus::Image* whiteBishop = nullptr;
Gdiplus::Image* whiteRook = nullptr;
Gdiplus::Image* whiteQueen = nullptr;
Gdiplus::Image* whiteKing = nullptr;

Gdiplus::Image* blackPawn = nullptr;
Gdiplus::Image* blackRook = nullptr;
Gdiplus::Image* blackKnight = nullptr;
Gdiplus::Image* blackBishop = nullptr;
Gdiplus::Image* blackQueen = nullptr;
Gdiplus::Image* blackKing = nullptr;

int board[8][8];

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

Gdiplus::Image* loadResourceImage(int id) {
    HRSRC resource = FindResourceW(NULL, MAKEINTRESOURCEW(id), L"PNG");
    HGLOBAL loadedResource = LoadResource(NULL, resource);
    void* data = LockResource(loadedResource);

    DWORD size = SizeofResource(NULL, resource);

    HGLOBAL buffer = GlobalAlloc(GMEM_MOVEABLE, size);

    void* bufferData = GlobalLock(buffer);
    memcpy(bufferData, data, size);
    GlobalUnlock(buffer);

    IStream* stream = nullptr;

    CreateStreamOnHGlobal(buffer, TRUE, &stream);
    Gdiplus::Image* image = Image::FromStream(stream);

    return image;
}

Gdiplus::Image* getPieceImage(int id) {
    if (id == WHITE_PAWN) {
        return whitePawn;
    } else if (id == WHITE_KNIGHT) {
        return whiteKnight;
    } else if (id == WHITE_BISHOP) {
        return whiteBishop;
    } else if (id == WHITE_ROOK) {
        return whiteRook;
    } else if (id == WHITE_QUEEN) {
        return whiteQueen;
    } else if (id == WHITE_KING) {
        return whiteKing;
    } else if (id == BLACK_PAWN) {
        return blackPawn;
    } else if (id == BLACK_KNIGHT) {
        return blackKnight;
    } else if (id == BLACK_BISHOP) {
        return blackBishop;
    } else if (id == BLACK_ROOK) {
        return blackRook;
    } else if (id == BLACK_QUEEN) {
        return blackQueen;
    } else if (id == BLACK_KING) {
        return blackKing;
    }
    return nullptr;
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
            short column = getColumn(lParam);
            short row = getRow(lParam);
            if (column != -1 && row != -1) {
                selectedColumn = column;
                selectedRow = row;
                InvalidateRect(hwnd, NULL, FALSE);
                UpdateWindow(hwnd);
            }
            return 0;
        }
        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            HBRUSH green  = CreateSolidBrush(GREEN);
            HBRUSH white  = CreateSolidBrush(WHITE);
            HBRUSH yellow = CreateSolidBrush(YELLOW);
            Graphics graphics(hdc);

            // Columns
            for (unsigned int i = 0; i < 8; ++i) {
                //Rows
                for (unsigned int j = 0; j < 8; ++j) {
                    RECT rect;
                    rect.left = board_xPos + box_width * i;
                    rect.top = board_yPos + box_width * j;
                    rect.right = board_xPos + box_width * (i + 1);
                    rect.bottom = board_yPos + box_width * (j + 1);
                    if (i == selectedColumn && j == selectedRow) {
                        FillRect(hdc, &rect, yellow);
                    } else if ((i % 2 == 0 && j % 2 == 0) || (i % 2 !=0 && j % 2 != 0)) {
                        FillRect(hdc, &rect, white);
                    } else {
                        FillRect(hdc, &rect, green);
                    }

                    // Put icons
                    if (board[i][j] != 0) {
                        Gdiplus::Image* current = getPieceImage(board[i][j]);
                        if (current != nullptr) {
                            graphics.DrawImage(current, rect.left, rect.top, box_width, box_width);
                        }
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
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // Load images
    whitePawn = loadResourceImage(WHITE_PAWN);
    whiteKnight = loadResourceImage(WHITE_KNIGHT);
    whiteBishop = loadResourceImage(WHITE_BISHOP);
    whiteRook = loadResourceImage(WHITE_ROOK);
    whiteQueen = loadResourceImage(WHITE_QUEEN);
    whiteKing = loadResourceImage(WHITE_KING);

    blackPawn = loadResourceImage(BLACK_PAWN);
    blackKnight = loadResourceImage(BLACK_KNIGHT);
    blackBishop = loadResourceImage(BLACK_BISHOP);
    blackRook = loadResourceImage(BLACK_ROOK);
    blackQueen = loadResourceImage(BLACK_QUEEN);
    blackKing = loadResourceImage(BLACK_KING);

    board[0][0] = BLACK_ROOK;
    board[1][0] = BLACK_KNIGHT;
    board[2][0] = BLACK_BISHOP;
    board[3][0] = BLACK_QUEEN;
    board[4][0] = BLACK_KING;
    board[5][0] = BLACK_BISHOP;
    board[6][0] = BLACK_KNIGHT;
    board[7][0] = BLACK_ROOK;

    board[0][7] = WHITE_ROOK;
    board[1][7] = WHITE_KNIGHT;
    board[2][7] = WHITE_BISHOP;
    board[3][7] = WHITE_QUEEN;
    board[4][7] = WHITE_KING;
    board[5][7] = WHITE_BISHOP;
    board[6][7] = WHITE_KNIGHT;
    board[7][7] = WHITE_ROOK;

    for (unsigned i = 0; i < 8; ++i) {
        board[i][1] = BLACK_PAWN;
        board[i][6] = WHITE_PAWN;
    }

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