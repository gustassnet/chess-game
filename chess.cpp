#include "chess.h"
#include "resources.h"

int board[8][8];
bool whiteTurn = true;

void initBoard() {
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
}

std::vector<std::pair<int, int>> getPossibleMoves(unsigned column, unsigned row) {
    if (whiteTurn) {
        if (board[column][row] == WHITE_PAWN) {
            return getPawnMoves(column, row);
        }
        if (board[column][row] == WHITE_KNIGHT) {
            return getKnightMoves(column, row);
        }
        if (board[column][row] == WHITE_BISHOP) {
            return getBishopMoves(column, row);
        }
    } else {
        if (board[column][row] == BLACK_PAWN) {
            return getPawnMoves(column, row);
        }
        if (board[column][row] == BLACK_KNIGHT) {
            return getKnightMoves(column, row);
        }
        if (board[column][row] == BLACK_BISHOP) {
            return getBishopMoves(column, row);
        }
    }
    return {};
}

std::vector<std::pair<int, int>> getPawnMoves(unsigned column, unsigned row) {
    std::vector<std::pair<int, int>> possibleMoves;

    if (isWhitePiece(board[column][row])) {
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

std::vector<std::pair<int, int>> getKnightMoves(unsigned column, unsigned row) {
    std::vector<std::pair<int, int>> possibleMoves;

    if (isWhitePiece(board[column][row])) {
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

std::vector<std::pair<int, int>> getBishopMoves(unsigned column, unsigned row) {
    return getDiagonalMoves(column, row);
}

std::vector<std::pair<int, int>> getDiagonalMoves(unsigned column, unsigned row) {
    std::vector<std::pair<int, int>> possibleMoves;
    std::vector<std::pair<int, int>> moves;

    moves = checkDiagonal(column, row, 1, 1);
    possibleMoves.insert(possibleMoves.end(), moves.begin(), moves.end());

    moves = checkDiagonal(column, row, 1, -1);
    possibleMoves.insert(possibleMoves.end(), moves.begin(), moves.end());

    moves = checkDiagonal(column, row, -1, 1);
    possibleMoves.insert(possibleMoves.end(), moves.begin(), moves.end());
    
    moves = checkDiagonal(column, row, -1, -1);
    possibleMoves.insert(possibleMoves.end(), moves.begin(), moves.end());

    return possibleMoves;
}

std::vector<std::pair<int, int>> checkDiagonal(unsigned column, unsigned row, short columnDirection, short rowDirection) {
    std::vector<std::pair<int, int>> possibleMoves;

    for (int i = column + columnDirection, j = row + rowDirection; i >= 0 && i < 8 && j >= 0 && j < 8; i += columnDirection, j += rowDirection) {
        if (!isWhitePiece(board[i][j]) && !isBlackPiece(board[i][j])) {
            possibleMoves.push_back({i, j});
        } else {
            if (isWhitePiece(board[column][row])) {
                if (isBlackPiece(board[i][j])) {
                    possibleMoves.push_back({i, j});
                }
                break;
            } else {
                if (isWhitePiece(board[i][j])) {
                    possibleMoves.push_back({i, j});
                }
                break;
            }
        }
    }
    return possibleMoves;
}

int makeMove(std::vector<std::pair<int, int>> possibleMoves, unsigned prevColumn, unsigned prevRow, unsigned newColumn, unsigned newRow){
    for (auto move : possibleMoves){
        if (move.first == newColumn && move.second == newRow) {
            board[newColumn][newRow] = board[prevColumn][prevRow];
            board[prevColumn][prevRow] = 0;
            whiteTurn = !whiteTurn;
            return 1;
        }
    }
    return 0;
}

int getPieceId(unsigned column, unsigned row) {
    return board[column][row];
}

bool isWhitePiece(int id) {
    return id >= WHITE_PAWN && id <= WHITE_KING;
}

bool isBlackPiece(int id) {
    return id >= BLACK_PAWN && id <= BLACK_KING;
}