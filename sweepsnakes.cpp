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
	for (unsigned long int y = 0; y < ydim; y++) {
		for (unsigned long int x = 0; x < xdim; x++) {

			std::cout << "x is: " << x << " y is: " << y << std::endl;
			std::cout << "value is: " << (int)board[y * xdim + x] << std::endl;

			// If the tile is not a snake, compute # of neighboring snakes
			if (board[y * xdim + x] != 9 ) {

				std::cout << "oneman " << std::endl;
				// Snake counter variable
				unsigned long int snakecount{0};


				//   These if statements are meant to prevent the for loops from checking off the board
				//   since the board is really just a single-dimensioned array, "negative" space can be
				//   tiles on the previous row.

				// REPEAT: x and y = 0
				/*
				if (x == 0 and y == 0) {
					//Now look at every single tile around it:
					for (unsigned long int look_y {y}; look_y <= y+1; look_y++) {
						for (unsigned long int look_x {x}; look_x <= x+1; look_x++) {
							// Check if tile is within board dimensions
							if (look_x < xdim && look_y < ydim && look_x >= 0 && look_y >= 0) {
								// Is it a snake?
								if (board[look_y * xdim + look_x] == 9) {
									snakecount++;
								}
							}
						}
					}
				}
				// REPEAT 2: x but NOT y = 0
				else if (x == 0 and y > 0) {
					for (unsigned long int look_y {y-1}; look_y <= y+1; look_y++) {
						for (unsigned long int look_x {x}; look_x <= x+1; look_x++) {
							if (look_x < xdim && look_y < ydim && look_x >= 0 && look_y >= 0) {
								if (board[look_y * xdim + look_x] == 9) {
									snakecount++;
								}
							}
						}
					}
				}
				// REPEAT 3: NOT x but y = 0
				else if (x > 0 and y == 0) {
					for (unsigned long int look_y {y}; look_y <= y+1; look_y++) {
						for (unsigned long int look_x {x-1}; look_x <= x+1; look_x++) {
							if (look_x < xdim && look_y < ydim && look_x >= 0 && look_y >= 0) {
								if (board[look_y * xdim + look_x] == 9) {
									snakecount++;
								}
							}
						}
					}
				}

				else { */
					for (unsigned long int look_y {y-1}; look_y <= y+1; look_y++) {
						for (unsigned long int look_x {x-1}; look_x <= x+1; look_x++) {

//							std::cout << "look_x is: " << look_x << " look_y is: " << look_y << std::endl;

							if (look_x < xdim && look_y < ydim && look_x >= 0 && look_y >= 0) {

//								std::cout << "whassup" << std::endl;

								if (board[look_y * xdim + look_x] == 9) {
									snakecount++;
								//}
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
/*	std::size_t length = xdim * ydim;
	// Set bit 5 of every tile to 1
	for (unsigned long int i = 0; i < length; i++) {
		board[i] |= hiddenBit();
	} */
}

// Cleaning up
void clearBoard(char *board) {
	delete[] board;
	board = nullptr;
}

// Prints the board
void printBoard(char *board, std::size_t xdim, std::size_t ydim) {
	int index = 0;
	for (unsigned long int y = 0; y < ydim; y++) {
		for (unsigned long int x = 0; x < xdim; x++) {
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

		//   These if statements are meant to prevent the for loops from checking off the board
		//   since the board is really just a single-dimensioned array, "negative" space can be
		//   tiles on the previous row.

		// THIS BE A REPEAT. IF X AND Y ARE 0:

		if ((yloc == 0) and (xloc == 0)){
			for (unsigned long int reveal_y {yloc}; reveal_y <= yloc + 1; reveal_y++) {
						for (unsigned long int reveal_x {xloc}; reveal_x <= xloc + 1; reveal_x++) {
							// Check if tile is within board dimensions
							if (reveal_x < xdim && reveal_y < ydim && reveal_x >= 0 && reveal_y >= 0) {
								// Make sure the tile is NOT marked
								if ( (board[reveal_y * xdim + reveal_x] & markedBit() ) == 0) {
									// Reveal space
									board[reveal_y * xdim + reveal_x] = board[reveal_y * xdim + reveal_x] & valueMask();
								}
							}
						}
					}
		}
		// REPEAT 2. IF X BUT NOT Y ARE 0.
		else if ((yloc > 0) and (xloc == 0)){
			for (unsigned long int reveal_y {yloc-1}; reveal_y <= yloc + 1; reveal_y++) {
				for (unsigned long int reveal_x {xloc}; reveal_x <= xloc + 1; reveal_x++) {
					if (reveal_x < xdim && reveal_y < ydim && reveal_x >= 0 && reveal_y >= 0) {
						if ( (board[reveal_y * xdim + reveal_x] & markedBit() ) == 0) {
							board[reveal_y * xdim + reveal_x] = board[reveal_y * xdim + reveal_x] & valueMask();
						}
					}
				}
			}
		}
		// REPEAT 3. IF NOT X BUT Y ARE 0.
		else if ((yloc == 0) and (xloc > 0)){
			for (unsigned long int reveal_y {yloc}; reveal_y <= yloc + 1; reveal_y++) {
				for (unsigned long int reveal_x {xloc-1}; reveal_x <= xloc + 1; reveal_x++) {
					if (reveal_x < xdim && reveal_y < ydim && reveal_x >= 0 && reveal_y >= 0) {
						if ( (board[reveal_y * xdim + reveal_x] & markedBit() ) == 0) {
							board[reveal_y * xdim + reveal_x] = board[reveal_y * xdim + reveal_x] & valueMask();
						}
					}
				}
			}
		}
		else {
			for (unsigned long int reveal_y {yloc-1}; reveal_y <= yloc + 1; reveal_y++) {
							for (unsigned long int reveal_x {xloc-1}; reveal_x <= xloc + 1; reveal_x++) {
								// Check if tile is within board dimensions
								if (reveal_x < xdim && reveal_y < ydim && reveal_x >= 0 && reveal_y >= 0) {
									// Make sure the TILE is NOT MARKED
									if ( (board[reveal_y * xdim + reveal_x] & markedBit() ) == 0) {
										// Reveal space
										board[reveal_y * xdim + reveal_x] = board[reveal_y * xdim + reveal_x] & valueMask();
									}
								}
							}
						}
		}
	}
	// Otherwise, just reveal the space
	else {
		board[yloc * xdim + xloc] = board[yloc * xdim + xloc] & valueMask();
	}
	return 0;
}

// Handles logic for marking a tile
int mark(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc) {
	// If it's not hidden, return 2.
	if ( (board[yloc * xdim + xloc] & hiddenBit() ) == 0) {
		return 2;
	}
	// Otherwise see if MARK = false, then add mark..
	if ( (board[yloc * xdim + xloc] & markedBit() ) == 0) {
		board[yloc * xdim + xloc] |= markedBit();
	}
	else {
		board[yloc * xdim + xloc] ^= markedBit();
	}
	return 0;
}

// Check if every single tile on the board fulfills the victory conditions
bool isWon(char *board, std::size_t xdim, std::size_t ydim) {
	unsigned long int win{0};
	for (unsigned long int y = 0; y < ydim; y++) {
		for (unsigned long int x = 0; x < xdim; x++) {
			if ( ( (board[y * xdim + x] & hiddenBit() ) == hiddenBit() ) and ((board[y * xdim + x] & valueMask()) == 9) ) {
				win++;
			}
			else if ( ( (board[y * xdim + x] & markedBit() ) == markedBit() ) and ((board[y * xdim + x] & valueMask()) == 9) ) {
				win++;
			}
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

