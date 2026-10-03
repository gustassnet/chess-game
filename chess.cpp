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

std::vector<std::pair<int, int>> getValidMoves(unsigned column, unsigned row) {
    std::vector<std::pair<int, int>> validMoves;
    std::vector<std::pair<int, int>> possibleMoves = getPossibleMoves(column, row);
    for (auto move : possibleMoves) {
        if (validMove(column, row, move.first, move.second)) {
            validMoves.push_back(move);
        }
    }
    return validMoves;
}

// Depends on whos turn to play
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
        if (board[column][row] == WHITE_ROOK) {
            return getRookMoves(column, row);
        }
        if (board[column][row] == WHITE_QUEEN) {
            return getQueenMoves(column, row);
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
        if (board[column][row] == BLACK_ROOK) {
            return getRookMoves(column, row);
        }
        if (board[column][row] == BLACK_QUEEN) {
            return getQueenMoves(column, row);
        }
    }
    return {};
}

// Returns all moves for a piece, regardless of whose turn it is
std::vector<std::pair<int, int>> getPieceMoves(unsigned column, unsigned row) {
    if (board[column][row] == WHITE_PAWN) {
        return getPawnMoves(column, row);
    } else if (board[column][row] == WHITE_KNIGHT) {
        return getKnightMoves(column, row);
    } else if (board[column][row] == WHITE_BISHOP) {
        return getBishopMoves(column, row);
    } else if (board[column][row] == WHITE_ROOK) {
        return getRookMoves(column, row);
    } else if (board[column][row] == WHITE_QUEEN) {
        return getQueenMoves(column, row);
    } else if (board[column][row] == BLACK_PAWN) {
        return getPawnMoves(column, row);
    } else if (board[column][row] == BLACK_KNIGHT) {
        return getKnightMoves(column, row);
    } else if (board[column][row] == BLACK_BISHOP) {
        return getBishopMoves(column, row);
    } else if (board[column][row] == BLACK_ROOK) {
        return getRookMoves(column, row);
    } else if (board[column][row] == BLACK_QUEEN) {
        return getQueenMoves(column, row);
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
    std::vector<std::pair<int, int>> possibleMoves;
    std::vector<std::pair<int, int>> moves;

    moves = checkStraight(column, row, 1, 1);
    possibleMoves.insert(possibleMoves.end(), moves.begin(), moves.end());

    moves = checkStraight(column, row, 1, -1);
    possibleMoves.insert(possibleMoves.end(), moves.begin(), moves.end());

    moves = checkStraight(column, row, -1, 1);
    possibleMoves.insert(possibleMoves.end(), moves.begin(), moves.end());
    
    moves = checkStraight(column, row, -1, -1);
    possibleMoves.insert(possibleMoves.end(), moves.begin(), moves.end());

    return possibleMoves;
}

std::vector<std::pair<int, int>> getRookMoves(unsigned column, unsigned row) {
    std::vector<std::pair<int, int>> possibleMoves;
    std::vector<std::pair<int, int>> moves;

    moves = checkStraight(column, row, 1, 0);
    possibleMoves.insert(possibleMoves.end(), moves.begin(), moves.end());

    moves = checkStraight(column, row, -1, 0);
    possibleMoves.insert(possibleMoves.end(), moves.begin(), moves.end());

    moves = checkStraight(column, row, 0, 1);
    possibleMoves.insert(possibleMoves.end(), moves.begin(), moves.end());
    
    moves = checkStraight(column, row, 0, -1);
    possibleMoves.insert(possibleMoves.end(), moves.begin(), moves.end());

    return possibleMoves;
}

std::vector<std::pair<int, int>> getQueenMoves(unsigned column, unsigned row) {
    std::vector<std::pair<int, int>> possibleMoves;
    std::vector<std::pair<int, int>> moves;

    moves = getBishopMoves(column, row);
    possibleMoves.insert(possibleMoves.end(), moves.begin(), moves.end());

    moves = getRookMoves(column, row);
    possibleMoves.insert(possibleMoves.end(), moves.begin(), moves.end());

    return possibleMoves;
}

std::vector<std::pair<int, int>> checkStraight(unsigned column, unsigned row, short columnDirection, short rowDirection) {
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

bool validMove(unsigned prevColumn, unsigned prevRow, unsigned newColumn, unsigned newRow) {
    int temp1 = board[newColumn][newRow];
    int temp2 = board[prevColumn][prevRow];

    board[newColumn][newRow] = board[prevColumn][prevRow];
    board[prevColumn][prevRow] = 0;
    if (kingInCheck()) {
        board[prevColumn][prevRow] = temp2;
        board[newColumn][newRow] = temp1;
        return false;
    }
    board[prevColumn][prevRow] = temp2;
    board[newColumn][newRow] = temp1;
    return true;
}

bool kingInCheck() {
    for (unsigned i = 0; i < 8; ++i) {
        for (unsigned j = 0; j < 8; ++j) {
            if (!whiteTurn && isWhitePiece(board[i][j])) {
                std::vector<std::pair<int, int>> possibleMoves = getPieceMoves(i, j);
                for (auto move : possibleMoves) {
                    if (board[move.first][move.second] == BLACK_KING) {
                        return true;
                    }
                }
            } else if (whiteTurn && isBlackPiece(board[i][j])) {
                std::vector<std::pair<int, int>> possibleMoves = getPieceMoves(i, j);
                for (auto move : possibleMoves) {
                    if (board[move.first][move.second] == WHITE_KING) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool isWhitePiece(int id) {
    return id >= WHITE_PAWN && id <= WHITE_KING;
}

bool isBlackPiece(int id) {
    return id >= BLACK_PAWN && id <= BLACK_KING;
}