#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include "resources.h"

using namespace Gdiplus;
using namespace std;

#define window_width 1200
#define window_height 800

#define box_width 80
#define board_xPos (window_width - 8 * box_width) / 2
#define board_yPos (window_height - 8 * box_width) / 2

#define GREEN RGB(107, 142, 78)
#define NUDE RGB(254, 230, 196)
#define BROWN RGB(100, 72, 60)

vector<pair<int, int>> possibleMoves;
short selectedColumn = -1;
short selectedRow = -1;

bool whiteTurn = 1;

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

//----
void drawSquare(HDC hdc, unsigned column, unsigned row);
//--


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

void drawOuter(HDC hdc) {
    HBRUSH brown  = CreateSolidBrush(BROWN);
    RECT rect;
    rect.left = board_xPos - 0.8 * box_width;
    rect.top = board_yPos - 0.8 * box_width;
    rect.right = board_xPos + 8.8 * box_width;
    rect.bottom = board_yPos + 8.8 * box_width;

    FillRect(hdc, &rect, brown);
    DeleteObject(brown);
}

void drawInner(HDC hdc) {
    HBRUSH nude  = CreateSolidBrush(NUDE);
    RECT rect;
    rect.left = board_xPos - 0.1 * box_width;
    rect.top = board_yPos - 0.1 * box_width;
    rect.right = board_xPos + 8.1 * box_width;
    rect.bottom = board_yPos + 8.1 * box_width;

    FillRect(hdc, &rect, nude);
    DeleteObject(nude);
}

void drawSquare(HDC hdc, unsigned column, unsigned row) {
    Gdiplus::Graphics graphics(hdc);
    RECT rect;
    rect.left = board_xPos + box_width * column;
    rect.top = board_yPos + box_width * row;
    rect.right = board_xPos + box_width * (column + 1);
    rect.bottom = board_yPos + box_width * (row + 1);
    if (column == selectedColumn && row == selectedRow && board[column][row] != 0) {
        HBRUSH green = CreateSolidBrush(GREEN);
        FillRect(hdc, &rect, green);
        DeleteObject(green);
    } else if ((column % 2 == 0 && row % 2 == 0) || (column % 2 !=0 && row % 2 != 0)) {
        HBRUSH nude  = CreateSolidBrush(NUDE);
        FillRect(hdc, &rect, nude);
        DeleteObject(nude);
    } else {
        HBRUSH brown  = CreateSolidBrush(BROWN);
        FillRect(hdc, &rect, brown);
        DeleteObject(brown);
    }

    // Put icon
    if (board[column][row] != 0) {
        Gdiplus::Image* current = getPieceImage(board[column][row]);
        if (current != nullptr) {
            graphics.DrawImage(current, rect.left, rect.top, box_width, box_width);
        }
    }
}

bool isWhitePiece(int id) {
    return id >= WHITE_PAWN && id <= WHITE_KING;
}

bool isBlackPiece(int id) {
    return id >= BLACK_PAWN && id <= BLACK_KING;
}

vector<pair<int, int>> getPawnMoves(unsigned column, unsigned row, bool isWhite) {
    vector<pair<int, int>> possibleMoves;

    if (isWhite) {
        if (row > 0 && board[column][row - 1] == 0) {
            possibleMoves.push_back({column, row - 1});
            if (row == 6 && board[column][row - 2] == 0) {
                possibleMoves.push_back({column, row - 2});
            }
        }
        if (column < 7 && row > 0 && isBlackPiece(board[column + 1][row - 1])) {
            possibleMoves.push_back({column + 1, row - 1});
        }
        if (column > 0 && row > 0 && isBlackPiece(board[column - 1][row - 1])) {
            possibleMoves.push_back({column - 1, row - 1});
        }
        //PROMOTE to..
        //TBI en passant
    } else {
        if (row < 7 && board[column][row + 1] == 0) {
            possibleMoves.push_back({column, row + 1});
            if (row == 1 && board[column][row + 2] == 0) {
                possibleMoves.push_back({column, row + 2});
            }
        }
        if (column < 7 && row <7 && isWhitePiece(board[column + 1][row + 1])) {
            possibleMoves.push_back({column + 1, row + 1});
        }
        if (column > 0 && row <7 && isWhitePiece(board[column - 1][row + 1])) {
            possibleMoves.push_back({column - 1, row + 1});
        }
        //PROMOTE to..
        //TBI en passant
    }
    return possibleMoves;
}

vector<pair<int, int>> getKnightMoves(unsigned column, unsigned row, bool isWhite) {
    vector<pair<int, int>> possibleMoves;

    if (isWhite) {
        if (column < 7 && row > 1 && !isWhitePiece(board[column + 1][row - 2])) {
            possibleMoves.push_back({column + 1, row - 2});
        }
        if (column < 6 && row > 0 && !isWhitePiece(board[column + 2][row - 1])) {
            possibleMoves.push_back({column + 2, row - 1});
        }
        if (column < 6 && row < 7 && !isWhitePiece(board[column + 2][row + 1])) {
            possibleMoves.push_back({column + 2, row + 1});
        }
        if (column < 7 && row < 6 && !isWhitePiece(board[column + 1][row + 2])) {
            possibleMoves.push_back({column + 1, row + 2});
        }
        if (column > 0 && row < 6 && !isWhitePiece(board[column - 1][row + 2])) {
            possibleMoves.push_back({column - 1, row + 2});
        }
        if (column > 1 && row < 7 && !isWhitePiece(board[column - 2][row + 1])) {
            possibleMoves.push_back({column - 2, row + 1});
        }
        if (column > 1 && row > 0 && !isWhitePiece(board[column - 2][row - 1])) {
            possibleMoves.push_back({column - 2, row - 1});
        }
        if (column > 0 && row > 1 && !isWhitePiece(board[column - 1][row - 2])) {
            possibleMoves.push_back({column - 1, row - 2});
        }
    } else {
        if (column < 7 && row > 1 && !isBlackPiece(board[column + 1][row - 2])) {
            possibleMoves.push_back({column + 1, row - 2});
        }
        if (column < 6 && row > 0 && !isBlackPiece(board[column + 2][row - 1])) {
            possibleMoves.push_back({column + 2, row - 1});
        }
        if (column < 6 && row < 7 && !isBlackPiece(board[column + 2][row + 1])) {
            possibleMoves.push_back({column + 2, row + 1});
        }
        if (column < 7 && row < 6 && !isBlackPiece(board[column + 1][row + 2])) {
            possibleMoves.push_back({column + 1, row + 2});
        }
        if (column > 0 && row < 6 && !isBlackPiece(board[column - 1][row + 2])) {
            possibleMoves.push_back({column - 1, row + 2});
        }
        if (column > 1 && row < 7 && !isBlackPiece(board[column - 2][row + 1])) {
            possibleMoves.push_back({column - 2, row + 1});
        }
        if (column > 1 && row > 0 && !isBlackPiece(board[column - 2][row - 1])) {
            possibleMoves.push_back({column - 2, row - 1});
        }
        if (column > 0 && row > 1 && !isBlackPiece(board[column - 1][row - 2])) {
            possibleMoves.push_back({column - 1, row - 2});
        }
    }
    return possibleMoves;
}

vector<pair<int, int>> getPossibleMoves(unsigned column, unsigned row) {
    if (whiteTurn) {
        if (board[column][row] == WHITE_PAWN) {
            return getPawnMoves(column, row, true);
        }
        if (board[column][row] == WHITE_KNIGHT) {
            return getKnightMoves(column, row, true);
        }
    } else {
        if (board[column][row] == BLACK_PAWN) {
            return getPawnMoves(column, row, false);
        }
        if (board[column][row] == BLACK_KNIGHT) {
            return getKnightMoves(column, row, true);
        }
    }
    return {};
}

void drawCoordinates(HDC hdc) {
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, NUDE);

    HFONT font = CreateFontW(
        24,
        0,
        0,
        0,
        FW_BOLD,
        FALSE,
        FALSE,
        FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        L"Arial"
    );

    HFONT oldFont = (HFONT)SelectObject(hdc, font);

    const wchar_t letters[] = L"abcdefgh";

    for (int i = 0; i < 8; ++i) {
        int x = board_xPos + i * box_width + box_width / 2 - 7;
        int y = board_yPos + 8 * box_width + 10;

        TextOutW(hdc, x, y, &letters[i], 1);
    }

    // Skaičiai kairėje
    for (int i = 0; i < 8; ++i) {
        wchar_t number = L'8' - i;

        int x = board_xPos - 25;
        int y = board_yPos + i * box_width + box_width / 2 - 12;

        TextOutW(hdc, x, y, &number, 1);
    }

    SelectObject(hdc, oldFont);
    DeleteObject(font);
}

void drawPossibleMove(HDC hdc, vector<pair<int, int>> possibleMoves) {
    Gdiplus::Graphics graphics(hdc);
    Gdiplus::SolidBrush brown(Gdiplus::Color(150, 0, 0, 0));

    for (auto move : possibleMoves) {
        int column = move.first;
        int row = move.second;

        int centerX = board_xPos + (column + 0.5) * box_width;
        int centerY = board_yPos + (row + 0.5) * box_width;

        graphics.FillEllipse(&brown, centerX - 10,centerY - 10, 20, 20);
    }
    //DeleteObject(brown);
}

void makeMove(HDC hdc, vector<pair<int, int>> possibleMoves, unsigned prevColumn, unsigned prevRow, unsigned newColumn, unsigned newRow){
    // delete previous possible moves
    for (auto move : possibleMoves) {
        drawSquare(hdc, move.first, move.second);
    }

    // make move
    for (auto move : possibleMoves){
        if (move.first == newColumn && move.second == newRow) {
            board[newColumn][newRow] = board[prevColumn][prevRow];
            board[prevColumn][prevRow] = 0;
            drawSquare(hdc, newColumn, newRow);
            drawSquare(hdc, prevColumn, prevRow);

            whiteTurn = !whiteTurn;
        }
    }
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
                drawSquare(hdc, prevColumn, prevRow);
            }

            // try make move
            makeMove(hdc, possibleMoves, prevColumn, prevRow, selectedColumn, selectedRow);

            // draw new possible moves
            if (selectedColumn != -1 && selectedRow != -1) {
                possibleMoves = getPossibleMoves(selectedColumn, selectedRow);
                drawPossibleMove(hdc, possibleMoves);
            }

            if (selectedColumn != -1 && selectedRow != -1) {
                drawSquare(hdc, selectedColumn, selectedRow);
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
                    drawSquare(hdc, i, j);
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