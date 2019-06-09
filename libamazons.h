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

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Represents the possible states of a square on the board
 */
typedef enum SquareState {
	EMPTY = 0,
	ARROW = 1,
	WHITE = 2,
	BLACK = 4
} SquareState;

/**
 * Represents the board state of an ongoing game
 */
typedef struct BoardState {
	int whitePieces, blackPieces;
	int boardWidth, boardHeight;
	SquareState* board;
} BoardState;

/**
 * Represents a square on the board
 */
typedef struct Square {
	int x, y;
} Square;

/**
 * Swaps the given player to the other player
 * @param player Player variable to swap
 */
void swapPlayer(SquareState* player);

/**
 * Compares two integers and returns the unit displacement between them
 * @param a First integer
 * @param b Second integer
 * @return 1, 0, -1 if a > b, a == b, or a < b respectively
 */
int cmp(int a, int b);

/**
 * Initializes a board with the given game settings
 * @param board Board state to initialize
 * @param wp Number of white/player 1 pieces
 * @param bp Number of black/player 2 pieces
 * @param bw Board width
 * @param bh Board height
 * @param whiteStart Array of Squares indicating starting locations for white player/player 1
 * @Param blackStart Array of Squares indicating starting locations for black player/player 2
 */
void boardstate_init(BoardState* board, int wp, int bp, int bw, int bh, Square* whiteStart, Square* blackStart);

/**
 * Deallocates the memory occupied by a board
 * @param board Board state
 */
void boardstate_free(BoardState* board);

/**
 * Determine whether an Amazon at a given location has any valid moves
 * @param board Board state
 * @param square Location to check
 * @return Whether any of the surrounding squares are empty
 */
int hasValidMove(BoardState* board, Square* square);

/**
 * Determines whether a player has any valid moves
 * @param board Board state
 * @param player The player to check
 * @return Whether the specified player has any valid moves
 */
int playerHasValidMove(BoardState* board, SquareState player);

/**
 * Determine whether the path between two squares is unobstructed
 * @param board Board state
 * @param src Starting location
 * @param dst Ending location
 * @return Whether all squares between the given squares are empty
 */
int pathUnobstructed(BoardState* board, Square* src, Square* dst);

/**
 * Determine whether a square is valid
 * @param board Board state
 * @param square THe square to check
 * @return Whether the square exists on the board
 */
int isValidSquare(BoardState* board, Square* square);

/**
 * Determine whether a move is valid
 * @param board Board state
 * @param src Starting location
 * @param dst Ending location
 * @return Whether the specified squares are on the same row, column, or diagonal
 */
int isValidMove(BoardState* board, Square* src, Square* dst);

/**
 * Move an Amazon
 * @param board Board state
 * @param src Current location of the Amazon
 * @param dst Destination location of the Amazon
 * @return Whether the specified move is valid
 */
int amazons_move(BoardState* board, Square* src, Square* dst);

/**
 * Shoot an arrow
 * @param board Board state
 * @param src Current location of the firing Amazon
 * @param target Target location of the firing Amazon
 * @return Whether the specified shot is valid
 */
int amazons_shoot(BoardState* board, Square* src, Square* target);

#ifdef __cplusplus
}
#endif

#endif
