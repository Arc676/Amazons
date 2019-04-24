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

#include <stdio.h>
#include "libamazons.h"

void printBoard(BoardState* board) {
	for (int x = 0; x < board->boardWidth; x++) {
		for (int y = 0; y < board->boardHeight; y++) {
			switch (board->board[x * board->boardWidth + y]) {
			case EMPTY:
				putchar('.');
				break;
			case ARROW:
				putchar('x');
				break;
			case WHITE:
				putchar('W');
				break;
			case BLACK:
				putchar('B');
				break;
			}
		}
		putchar('\n');
	}
}

void swapPlayer(SquareState* player) {
	*player = *player == WHITE ? BLACK : WHITE;
}

int main(int argc, char* argv[]) {
	BoardState board;
	Square wpos[4] = {
		{3, 0}, {0, 3}, {0,6}, {3, 9}
	};
	Square bpos[4] = {
		{6, 0}, {9, 3}, {9, 6}, {6, 9}
	};
	boardstate_init(&board, 4, 4, 10, 10, wpos, bpos);
	SquareState currentPlayer = WHITE;
	while (playerHasValidMove(&board, currentPlayer)) {
		printBoard(&board);
		int x0, y0, x1, y1, xs, ys;
		printf("Enter amazon position [x y]: ");
		scanf("%d %d", &x0, &y0);
		Square src = { x0, y0 };
		printf("Enter destination square [x y]: ");
		scanf("%d %d", &x1, &y1);
		Square dst = { x1, y1 };
		printf("Enter shot square [x y]: ");
		scanf("%d %d", &xs, &ys);
		Square shot = { xs, ys };
		if (move(&board, &src, &dst)) {
			if (shoot(&board, &dst, &shot)) {
				swapPlayer(&currentPlayer);
			} else {
				printf("Invalid shot\n");
				move(&board, &dst, &src);
			}
		} else {
			printf("Invalid move\n");
		}
	}
	printBoard(&board);
	boardstate_free(&board);
	swapPlayer(&currentPlayer);
	if (currentPlayer == WHITE) {
		printf("White wins!\n");
	} else {
		printf("Black wins!\n");
	}
	return 0;
}
