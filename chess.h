#ifndef CHESS_H
#define CHESS_H

#include <vector>
#include <utility>

void initBoard();

std::vector<std::pair<int, int>> getPossibleMoves(unsigned column, unsigned row);
std::vector<std::pair<int, int>> getPawnMoves(unsigned column, unsigned row);
std::vector<std::pair<int, int>> getKnightMoves(unsigned column, unsigned row);
std::vector<std::pair<int, int>> getBishopMoves(unsigned column, unsigned row);

std::vector<std::pair<int, int>> getDiagonalMoves(unsigned column, unsigned row);
std::vector<std::pair<int, int>> getStraightMoves(unsigned column, unsigned row);
std::vector<std::pair<int, int>> checkStraight(unsigned column, unsigned row, short columnDirection, short rowDirection);

int makeMove(std::vector<std::pair<int, int>> possibleMoves, unsigned prevColumn, unsigned prevRow, unsigned newColumn, unsigned newRow);

int getPieceId(unsigned column, unsigned row);
bool isWhitePiece(int id);
bool isBlackPiece(int id);

#endif