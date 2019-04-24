//Copyright (C) 2019 Arc676/Alessandro Vinciguerra

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation (version 3)

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//See README and LICENSE for more details

#include "libamazons.h"

void swapPlayer(SquareState* player) {
	*player = *player == WHITE ? BLACK : WHITE;
}

int cmp(int a, int b) {
	if (a == b) {
		return 0;
	}
	if (a < b) {
		return -1;
	}
	return 1;
}

void boardstate_init(BoardState* board, int wp, int bp, int bw, int bh, Square* whiteStart, Square* blackStart) {
	board->whitePieces = wp;
	board->blackPieces = bp;
	board->boardWidth = bw;
	board->boardHeight = bh;
	board->board = malloc(bw * bh * sizeof(SquareState));
	memset(board->board, 0, bw * bh * sizeof(SquareState));
	for (int i = 0; i < wp; i++) {
		Square sq = whiteStart[i];
		board->board[sq.x * bw + sq.y] = WHITE;
	}
	for (int i = 0; i < bp; i++) {
		Square sq = blackStart[i];
		board->board[sq.x * bw + sq.y] = BLACK;
	}
}

void boardstate_free(BoardState* board) {
	free(board->board);
}

int hasValidMove(BoardState* board, Square* square) {
	for (int x = square->x - 1; x < square->x + 2; x++) {
		for (int y = square->y - 1; y < square->y + 2; y++) {
			if (x < 0 || x >= board->boardWidth || y < 0 || y >= board->boardHeight || (x == square->x && y == square->y)) {
				continue;
			}
			if (board->board[x * board->boardWidth + y] == EMPTY) {
				return 1;
			}
		}
	}
	return 0;
}

int playerHasValidMove(BoardState* board, SquareState player) {
	if (player != WHITE && player != BLACK) {
		return 0;
	}
	int piecesChecked = 0;
	int totalPieces = player == WHITE ? board->whitePieces : board->blackPieces;
	for (int x = 0; x < board->boardWidth && piecesChecked < totalPieces; x++) {
		for (int y = 0; y < board->boardHeight && piecesChecked < totalPieces; y++) {
			if (board->board[x * board->boardWidth + y] == player) {
				piecesChecked++;
				Square square = {x, y};
				if (hasValidMove(board, &square)) {
					return 1;
				}
			}
		}
	}
	return 0;
}

int pathUnobstructed(BoardState* board, Square* src, Square* dst) {
	int dx = cmp(dst->x, src->x);
	int dy = cmp(dst->y, src->y);
	int x = src->x + dx;
	int y = src->y + dy;
	while (x != dst->x && y != dst->y) {
		if (board->board[x * board->boardWidth + y] != EMPTY) {
			return 0;
		}
		x += dx;
		y += dy;
	}
	return 1;
}

int isValidSquare(BoardState* board, Square* square) {
	return square->x >= 0 && square->x < board->boardWidth && square->y >= 0 && square->y < board->boardHeight;
}

int isValidMove(BoardState* board, Square* src, Square* dst) {
	if (!isValidSquare(board, src) || !isValidSquare(board, dst)) {
		return 0;
	}
	if (src->x == dst->x && src->y == dst->y) {
		return 0;
	}
	if ((src->x == dst->x) ^ (src->y == dst->y)) {
		return pathUnobstructed(board, src, dst);;
	}
	int dx = dst->x - src->x;
	dx = dx > 0 ? dx : -dx;
	int dy = dst->y - src->y;
	dy = dy > 0 ? dy : -dy;
	if (dx == dy) {
		return pathUnobstructed(board, src, dst);;
	}
	return 0;
}

int amazons_move(BoardState* board, Square* src, Square* dst) {
	if (!isValidMove(board, src, dst)) {
		return 0;
	}
	board->board[dst->x * board->boardWidth + dst->y] = board->board[src->x * board->boardWidth + src->y];
	board->board[src->x * board->boardWidth + src->y] = EMPTY;
	return 1;
}

int amazons_shoot(BoardState* board, Square* src, Square* dst) {
	if (!isValidMove(board, src, dst)) {
		return 0;
	}
	board->board[dst->x * board->boardWidth + dst->y] = ARROW;
	return 1;
}
