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

#include <signal.h>
#include <stdio.h>
#include <string.h>

#include "libamazons.h"

BoardState board;

void quit(int signal) {
	printf("\nQuitting...\n");
	fflush(stdout);
	if (board.board) {
		boardstate_free(&board);
	}
	exit(0);
}

void printBoard(BoardState* board) {
	for (int x = 0; x < board->boardWidth; x++) {
		for (int y = 0; y < board->boardHeight; y++) {
			switch (board->board[y * board->boardWidth + x]) {
			default:
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

int main(int argc, char* argv[]) {
	memset(&board, 0, sizeof(board));
	struct sigaction handler;
	memset(&handler, 0, sizeof(handler));
	handler.sa_handler = quit;
	sigaction(SIGINT, &handler, NULL);
	printf("Game of the Amazons! Available under GPLv3.\n");

	printf("Use default settings? [Y/n]: ");
	char def;
	scanf("%c", &def);
	if (def == 'n' || def == 'N') {
		int wp, bp, bw, bh;
		printf(
			"Enter game properties [white pieces, black pieces, board "
			"width, board height]: ");
		scanf("%d %d %d %d", &wp, &bp, &bw, &bh);
		Square wpos[wp], bpos[bp];
		for (int i = 0; i < wp; i++) {
			printf("Enter starting location for white pawn #%d [x y]: ", i);
			scanf("%d %d", &wpos[i].x, &wpos[i].y);
		}
		for (int i = 0; i < bp; i++) {
			printf("Enter starting location for black pawn #%d [x y]: ", i);
			scanf("%d %d", &bpos[i].x, &bpos[i].y);
		}
		boardstate_init(&board, wp, bp, bw, bh, wpos, bpos);
	} else {
		boardstate_standard(&board);
	}
	while (playerHasValidMove(&board, board.currentPlayer)) {
		printf("------\n\n");
		if (board.currentPlayer == WHITE) {
			printf("White to move\n");
		} else {
			printf("Black to move\n");
		}
		printBoard(&board);

		int x0, y0, x1, y1, xs, ys;
		printf("Enter amazon position [x y]: ");
		scanf("%d %d", &x0, &y0);
		Square src = {x0, y0};
		printf("Enter destination square [x y]: ");
		scanf("%d %d", &x1, &y1);
		Square dst = {x1, y1};
		printf("Enter shot square [x y]: ");
		scanf("%d %d", &xs, &ys);
		Square shot = {xs, ys};
		if (boardstate_squareState(&board, &src) == board.currentPlayer &&
		    amazons_move(&board, &src, &dst)) {
			if (amazons_shoot(&board, &dst, &shot)) {
				int whiteSquares = 0, blackSquares = 0;
				SquareState winner =
					boardstate_winner(&board, &whiteSquares, &blackSquares);
				if (winner != EMPTY) {
					board.currentPlayer = winner;
					// Player is swapped again outside of loop
					swapPlayer(&(board.currentPlayer));
					printf(
						"The board has been divided. White controls %d "
						"squares. Black controls %d squares.\n",
						whiteSquares, blackSquares);
					break;
				}

				swapPlayer(&(board.currentPlayer));
			} else {
				printf("Invalid shot\n");
				amazons_move(&board, &dst, &src);
			}
		} else {
			printf("Invalid move\n");
		}
	}
	printBoard(&board);
	boardstate_free(&board);
	swapPlayer(&(board.currentPlayer));
	if (board.currentPlayer == WHITE) {
		printf("White wins!\n");
	} else {
		printf("Black wins!\n");
	}
	return 0;
}
