// Copyright (C) 2019-21 Arc676/Alessandro Vinciguerra

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation (version 3)

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
// See README and LICENSE for more details

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

	board->currentPlayer = WHITE;

	size_t boardSize = bw * bh * sizeof(SquareState);
	board->board = malloc(boardSize);
	board->map = malloc(boardSize);

	memset(board->board, 0, boardSize);
	for (int x = 0; x < bw; x++) {
		for (int y = 0; y < bh; y++) {
			board->map[y * bw + x] = UNDECIDED;
		}
	}

	for (int i = 0; i < wp; i++) {
		Square sq = whiteStart[i];
		board->board[sq.y * bw + sq.x] = WHITE;
	}
	for (int i = 0; i < bp; i++) {
		Square sq = blackStart[i];
		board->board[sq.y * bw + sq.x] = BLACK;
	}
}

void boardstate_standard(BoardState* board) {
	Square wpos[4] = {
		{3, 0}, {0, 3}, {0,6}, {3, 9}
	};
	Square bpos[4] = {
		{6, 0}, {9, 3}, {9, 6}, {6, 9}
	};
	boardstate_init(board, 4, 4, 10, 10, wpos, bpos);
}

void boardstate_free(BoardState* board) {
	free(board->board);
	free(board->map);
}

SquareState boardstate_winner(BoardState* board, int* white, int* black) {
	if (!boardstate_updateRegionMap(board)) {
		boardstate_countControlledSquares(board, white, black);
		if (*white == *black) {
			// If both players control the same number of squares, then the
			// current player wins because the other player will exhaust
			// their free squares first
			return board->currentPlayer;
		} else if (*white < *black) {
			return BLACK;
		} else {
			return WHITE;
		}
	}
	return EMPTY;
}

SquareState boardstate_squareState(BoardState* board, Square* square) {
	return board->board[square->y * board->boardWidth + square->x];
}

SquareState boardstate_squareController(BoardState* board, Square* square) {
	return board->map[square->y * board->boardWidth + square->x];
}

// DFS based on sample code at https://stackoverflow.com/a/53459466/2773311
SquareState dfs(BoardState* board, CheckState* visited, int x, int y) {
	int idx = y * board->boardWidth + x;
	visited[idx] = VISITED;

	SquareState controller = board->board[idx];

	// If an arrow is encountered, stop searching in this direction
	if (controller == ARROW) {
		visited[idx] = ASSIGNED;
		board->map[idx] = ARROW;
		return EMPTY;
	}

	// Check if a player can move in the region
	if (controller & (BLACK | WHITE)) {
		Square sq = { x, y };
		if (hasValidMove(board, &sq)) {
			controller |= HAS_MOVES;
		}
	}

	for (int x1 = x - 1; x1 <= x + 1; x1++) {
		for (int y1 = y - 1; y1 <= y + 1; y1++) {
			Square sq = {x1, y1};
			int idx1 = y1 * board->boardWidth + x1;
			if (isValidSquare(board, &sq) && visited[idx1] == UNCHECKED) {
				controller |= dfs(board, visited, x1, y1);
			}
		}
	}

	return controller;
}

void boardstate_fillRegion(BoardState* board, CheckState* visited, SquareState controller) {
	int bw = board->boardWidth;
	int bh = board->boardHeight;
	for (int x = 0; x < bw; x++) {
		for (int y = 0; y < bh; y++) {
			int idx = y * bw + x;
			if (visited[idx] == VISITED) {
				board->map[idx] = controller;
				visited[idx] = ASSIGNED;
			}
		}
	}
}

int boardstate_updateRegionMap(BoardState* board) {
	int bw = board->boardWidth;
	int bh = board->boardHeight;
	CheckState visited[bh * bw];
	memset(visited, 0, bh * bw * sizeof(CheckState));

	SquareState* map = board->map;
	int hasDisputedRegions = 0;

	for (int x = 0; x < bw; x++) {
		for (int y = 0; y < bh; y++) {
			int idx = y * bw + x;
			if (!visited[idx] && map[idx] == UNDECIDED) {
				SquareState control = dfs(board, visited, x, y);
				// If only one player is present in the region, it
				// doesn't matter if that player has further moves to make
				// (Logical XOR see https://stackoverflow.com/a/1596970/2773311)
				if (!(control & BLACK) != !(control & WHITE)) {
					control &= ~HAS_MOVES;
				}
				if (control != EMPTY) {
					boardstate_fillRegion(board, visited, control);
				}
				hasDisputedRegions |= control == UNDECIDED;
			}
		}
	}

	return hasDisputedRegions;
}

void boardstate_countControlledSquares(BoardState* board, int* white, int* black) {
	*white = 0;
	*black = 0;
	for (int x = 0; x < board->boardWidth; x++) {
		for (int y = 0; y < board->boardHeight; y++) {
			switch (board->map[y * board->boardWidth + x]) {
			case WHITE:
				(*white)++;
				break;
			case BLACK:
				(*black)++;
				break;
			default:
				break;
			}
		}
	}

	*white -= board->whitePieces;
	*black -= board->blackPieces;
}

int hasValidMove(BoardState* board, Square* square) {
	for (int x = square->x - 1; x <= square->x + 1; x++) {
		for (int y = square->y - 1; y <= square->y + 1; y++) {
			Square sq = {x, y};
			if (!isValidSquare(board, &sq)) {
				continue;
			}
			// We only have to check that a single neighboring cell is empty
			// because the arrow can always be fired towards the original square
			if (boardstate_squareState(board, &sq) == EMPTY) {
				return 1;
			}
		}
	}
	return 0;
}

int playerHasValidMove(BoardState* board, SquareState player) {
	if ((player & (WHITE | BLACK)) == 0) {
		return 0;
	}
	int piecesChecked = 0;
	int totalPieces = player == WHITE ? board->whitePieces : board->blackPieces;
	for (int x = 0; x < board->boardWidth; x++) {
		for (int y = 0; y < board->boardHeight; y++) {
			Square square = {x, y};
			if (boardstate_squareState(board, &square) == player) {
				if (hasValidMove(board, &square)) {
					return 1;
				}
				piecesChecked++;
				if (piecesChecked >= totalPieces) {
					return 0;
				}
			}
		}
	}
	return 0;
}

int pathUnobstructed(BoardState* board, Square* src, Square* dst) {
	if (boardstate_squareState(board, dst) != EMPTY) {
		return 0;
	}
	int dx = cmp(dst->x, src->x);
	int dy = cmp(dst->y, src->y);
	int x = src->x + dx;
	int y = src->y + dy;
	while (x != dst->x || y != dst->y) {
		if (board->board[y * board->boardWidth + x] != EMPTY) {
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
	if ((boardstate_squareState(board, src) & (WHITE | BLACK)) == 0) {
		return 0;
	}
	if (src->x == dst->x && src->y == dst->y) {
		return 0;
	}
	if ((src->x == dst->x) ^ (src->y == dst->y)) {
		return pathUnobstructed(board, src, dst);
	}
	int dx = cmp(dst->x, src->x);
	int dy = cmp(dst->y, src->y);
	// Square displacement to get absolute value
	if (dx * dx == dy * dy) {
		return pathUnobstructed(board, src, dst);
	}
	return 0;
}

int amazons_move(BoardState* board, Square* src, Square* dst) {
	if (!isValidMove(board, src, dst)) {
		return 0;
	}
	board->board[dst->y * board->boardWidth + dst->x] = boardstate_squareState(board, src);
	board->board[src->y * board->boardWidth + src->x] = EMPTY;
	return 1;
}

int amazons_shoot(BoardState* board, Square* src, Square* dst) {
	if (!isValidMove(board, src, dst)) {
		return 0;
	}
	board->board[dst->y * board->boardWidth + dst->x] = ARROW;
	return 1;
}
