#ifndef DRAW_H
#define DRAW_H

#include <windows.h>
#include <gdiplus.h>
#include <vector>
#include <utility>

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

void drawOuter(HDC hdc);
void drawInner(HDC hdc);
void drawCoordinates(HDC hdc);
void drawSquare(HDC hdc, int id, unsigned column, unsigned row, short selectedColumn, short selectedRow);
void drawPossibleMove(HDC hdc, vector<pair<int, int>> possibleMoves);

void loadImages();
Gdiplus::Image* loadResourceImage(int id);
Gdiplus::Image* getPieceImage(int id);

#endif