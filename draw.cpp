#include "draw.h"
#include "resources.h"

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

void drawSquare(HDC hdc, int id, unsigned column, unsigned row, short selectedColumn, short selectedRow) {
    Gdiplus::Graphics graphics(hdc);
    RECT rect;
    rect.left = board_xPos + box_width * column;
    rect.top = board_yPos + box_width * row;
    rect.right = board_xPos + box_width * (column + 1);
    rect.bottom = board_yPos + box_width * (row + 1);
    if (column == selectedColumn && row == selectedRow && id != 0) {
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
    if (id != 0) {
        Gdiplus::Image* current = getPieceImage(id);
        if (current != nullptr) {
            graphics.DrawImage(current, rect.left, rect.top, box_width, box_width);
        }
    }
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
}

void loadImages() {
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