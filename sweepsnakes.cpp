#include "sweepsnakes_lib.h"

char *makeBoard(std::size_t xdim, std::size_t ydim);
void computeSnakes(char *board, std::size_t xdim, std::size_t ydim);
void hideBoard(char *board, std::size_t xdim, std::size_t ydim);
void clearBoard(char *board);
void printBoard(char *board, std::size_t xdim, std::size_t ydim);
int reveal(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc);
int mark(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc);
bool isWon(char *board, std::size_t xdim, std::size_t ydim);

//Return a board (single-dimension array) of size x by y
char *makeBoard(std::size_t xdim, std::size_t ydim) {
	std::size_t length = xdim * ydim;
	char *array(new char[length]{0});
	return array;
}

// Logic for computing # of snakes around a tile
void computeSnakes (char *board, std::size_t xdim, std::size_t ydim) {

	// Check every single tile in turn, starting from row 0 and column 0 and moving right
	for (int y = 0; y < ydim; y++) {
		for (int x = 0; x < xdim; x++) {

			// If the tile is not a snake, compute # of neighboring snakes
			if (board[y * xdim + x] != 9 ) {

				// Snake counter variable
				int snakecount{0};

				// Check a 3x3 around the selected tile
				for (int check_y {y-1}; check_y <= y+1; check_y++) {
					for (int check_x {x-1}; check_x <= x+1; check_x++) {

						// Check tile is within dimensions
						if (check_x < xdim && check_y < ydim && check_x >= 0 && check_y >= 0) {

							if (board[check_y * xdim + check_x] == 9) {
								snakecount++;
							}
						}
					}
				}

			//Now that # of neighbor snakes is known, assign the count to the tile
			board[y*xdim + x] = snakecount;
			snakecount = 0;
			}
		}
	}
}

// Add hidden bit to every tile on the board
void hideBoard(char *board, std::size_t xdim, std::size_t ydim) {
	std::size_t length = xdim * ydim;
	// Set bit 5 of every tile to 1
	for (long unsigned int i = 0; i < length; i++) {
		board[i] |= hiddenBit();
	}
}

// Cleaning up
void clearBoard(char *board) {
	delete[] board;
	board = nullptr;
}

// Prints the board
void printBoard(char *board, std::size_t xdim, std::size_t ydim) {
	int index = 0;
	for (int y = 0; y < ydim; y++) {
		for (int x = 0; x < xdim; x++) {
			// Marked
			if ( (board[index] & markedBit() ) == markedBit() ) {
				std::cout<<'M';
			}
			// Hidden
			else if ( (board[index] & hiddenBit() ) == hiddenBit()) {
				std::cout<<'*';
			}
			// Value (0-8 or snake: 9)
			else {
				std::cout<<(int)board[index];
			}
			index++;
		}
		// Line break at row end
		std::cout<<std::endl;
	}
}

// Logic for revealing tiles
int reveal(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc) {

	// Typecast from unsigned int to int
	int x = (int)xloc;
	int y = (int)yloc;

	// MARKED=true
	if ( (board[yloc * xdim + xloc] & markedBit() ) != 0) {
		return 1;
	}

	// HIDDEN=false
	else if ( (board[yloc * xdim + xloc] & hiddenBit() ) == 0) {
		return 2;
	}
	// SNAKE=true
	else if ( (board[yloc*xdim+xloc] & valueMask() ) == 9) {
		board[yloc * xdim + xloc] = board[yloc * xdim + xloc] & valueMask();
		return 9;
	}

	// If value=0, reveal surrounding tiles.
	if ( (board[yloc*xdim+xloc] & valueMask() ) == 0) {

		// Check a 3x3 around selected tile
		for (int reveal_y {y-1}; reveal_y <= yloc + 1; reveal_y++) {
			for (int reveal_x {x-1}; reveal_x <= xloc + 1; reveal_x++) {

				// Check tile within dimensions
				if (reveal_x < xdim && reveal_y < ydim && reveal_x >= 0 && reveal_y >= 0) {
					// Reveal only if tile is not marked
					if ( (board[reveal_y * xdim + reveal_x] & markedBit() ) == 0) {
						board[reveal_y * xdim + reveal_x] = board[reveal_y * xdim + reveal_x] & valueMask();
					}
				}
			}
		}
	}

	// If tile is a snake, reveal the space
	else {
		board[yloc * xdim + xloc] = board[yloc * xdim + xloc] & valueMask();
	}
	return 0;
}

// Handles logic for marking a tile
int mark(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc) {
	// Check if already revealed
	if ( (board[yloc * xdim + xloc] & hiddenBit() ) == 0) {
		return 2;
	}
	// Otherwise if unmarked, then mark tile
	if ( (board[yloc * xdim + xloc] & markedBit() ) == 0) {
		board[yloc * xdim + xloc] |= markedBit();
	}
	// If marked, unmark tile
	else {
		board[yloc * xdim + xloc] ^= markedBit();
	}
	return 0;
}

// Check if every single tile on the board fulfills the victory conditions
bool isWon(char *board, std::size_t xdim, std::size_t ydim) {
	int win{0};

	for (long unsigned int y = 0; y < ydim; y++) {
		for (long unsigned int x = 0; x < xdim; x++) {
			// Hidden snakes are good
			if ( ( (board[y * xdim + x] & hiddenBit() ) == hiddenBit() ) and ((board[y * xdim + x] & valueMask()) == 9) ) {
				win++;
			}
			// Marked snakes are good
			else if ( ( (board[y * xdim + x] & markedBit() ) == markedBit() ) and ((board[y * xdim + x] & valueMask()) == 9) ) {
				win++;
			}
			// Revealed tiles are good
			else if ( (board[y * xdim + x] & hiddenBit() ) == 0) {
				win++;
			}
		}
	}
	if (win == (xdim*ydim)) {
		return true;
	}
	else {
		return false;
	}
}

