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

#ifndef LIBAMAZONS_H
#define LIBAMAZONS_H

#include <stdlib.h>
#include <string.h>

typedef enum SquareState {
	EMPTY = 0,
	ARROW,
	WHITE,
	BLACK
} SquareState;

typedef struct BoardState {
	int whitePieces, blackPieces;
	int boardWidth, boardHeight;
	SquareState* board;
} BoardState;

typedef struct Square {
	int x, y;
} Square;

int cmp(int a, int b);

void boardstate_init(BoardState* board, int wp, int bp, int bw, int bh, Square* whiteStart, Square* blackStart);

void boardstate_free(BoardState* board);

int hasValidMove(BoardState* board, Square* square);

int playerHasValidMove(BoardState* board, SquareState player);

int pathUnobstructed(BoardState* board, Square* src, Square* dst);

int isValidSquare(BoardState* board, Square* square);

int isValidMove(BoardState* board, Square* src, Square* dst);

int move(BoardState* board, Square* src, Square* dst);

int shoot(BoardState* board, Square* src, Square* target);

#endif
