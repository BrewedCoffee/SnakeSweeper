#include "geesespotter_lib.h"

char *createBoard(std::size_t xdim, std::size_t ydim);
void computeNeighbors(char *board, std::size_t xdim, std::size_t ydim);
void hideBoard(char *board, std::size_t xdim, std::size_t ydim);
void cleanBoard(char *board);
void printBoard(char *board, std::size_t xdim, std::size_t ydim);
int reveal(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc);
int mark(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc);
bool isGameWon(char *board, std::size_t xdim, std::size_t ydim);

//DONE
char *createBoard(std::size_t xdim, std::size_t ydim) {
	std::size_t length = xdim * ydim;
	char *array(new char[length]{0});
	return array;
}

//DIFFICULT. DONE.
void computeNeighbors(char *board, std::size_t xdim, std::size_t ydim) {
//Let's check every single TILE in turn, starting from row 0 and column 0 and moving right
	for (unsigned long int y = 0; y < ydim; y++) {
		for (unsigned long int x = 0; x < xdim; x++) {
			//We're not going to do anything if the tile is a 9. That would be dumb.
			if (board[y * xdim + x] == 0 ) {
				//Let's make a goose counter variable
				unsigned long int goosecounter{0};

				//STUPID REPEAT TIME 1: x and y = 0
				if (x == 0 and y == 0) {
					//Now we need to LOOK at every single tile around it:
					for (unsigned long int look_y {y}; look_y <= y+1; look_y++) {
						for (unsigned long int look_x {x}; look_x <= x+1; look_x++) {
							//Check if TILE is within board maximum
							if (look_x < xdim && look_y < ydim && look_x >= 0 && look_y >= 0) {
								//Now is this a goose?
								if (board[look_y * xdim + look_x] == 9) {
									goosecounter++;
								}
							}
						}
					}
				}
				//STUPID REPEAT TIME 2: x but NOT y = 0
				else if (x == 0 and y > 0) {
					//Now we need to LOOK at every single tile around it:
					for (unsigned long int look_y {y-1}; look_y <= y+1; look_y++) {
						for (unsigned long int look_x {x}; look_x <= x+1; look_x++) {
							//Check if TILE is within board maximum
							if (look_x < xdim && look_y < ydim && look_x >= 0 && look_y >= 0) {
								//Now is this a goose?
								if (board[look_y * xdim + look_x] == 9) {
									goosecounter++;
								}
							}
						}
					}
				}
				//STUPID REPEAT TIME 3: NOT x but y = 0
				else if (x > 0 and y == 0) {
					//Now we need to LOOK at every single tile around it:
					for (unsigned long int look_y {y}; look_y <= y+1; look_y++) {
						for (unsigned long int look_x {x-1}; look_x <= x+1; look_x++) {
							//Check if TILE is within board maximum
							if (look_x < xdim && look_y < ydim && look_x >= 0 && look_y >= 0) {
								//Now is this a goose?
								if (board[look_y * xdim + look_x] == 9) {
									goosecounter++;
								}
							}
						}
					}
				}
				else {
					//Now we need to LOOK at every single tile around it:
					for (unsigned long int look_y {y-1}; look_y <= y+1; look_y++) {
						for (unsigned long int look_x {x-1}; look_x <= x+1; look_x++) {
							//Check if TILE is within board maximum
							if (look_x < xdim && look_y < ydim && look_x >= 0 && look_y >= 0) {
								//Now is this a goose?
								if (board[look_y * xdim + look_x] == 9) {
									goosecounter++;
								}
							}
						}
					}
				}
				//Now that we know # of geese nearby, we can assign value to TILE
				board[y*xdim + x] = goosecounter;
				goosecounter = 0;
			}
		}
	}
}
//DONE
void hideBoard(char *board, std::size_t xdim, std::size_t ydim) {
	std::size_t length = xdim * ydim;
	//Setting every single part of the array's 5th position to 1
	for (unsigned long int i = 0; i < length; i++) {
		board[i] |= hiddenBit();
	}
}

//EASY... NEED CHECK
void cleanBoard(char *board) {
	delete[] board;
	board = nullptr;
}

//FIRST PART DONE
void printBoard(char *board, std::size_t xdim, std::size_t ydim) {
	int index = 0;
	for (unsigned long int y = 0; y < ydim; y++) {
		for (unsigned long int x = 0; x < xdim; x++) {
			//Is the TILE marked?
			if ( (board[index] & markedBit() ) == markedBit() ) {
				std::cout<<'M';
			}
			//Is the TILE hidden?
			else if ( (board[index] & hiddenBit() ) == hiddenBit()) {
				std::cout<<'*';
			}
			else {
				std::cout<<(int)board[index];
			}
			index++;
		}
		//Line Break
		std::cout<<std::endl;
	}
}

//DIFFICULT. DONE, NEED CHECK.
int reveal(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc) {
	//Is MARKED=true? DO I NEED TO SHOW ANYTHING? DONT THINK SO.
	if ( (board[yloc * xdim + xloc] & markedBit() ) != 0) {
		return 1;
	}

	//Is it HIDDEN=false? Then we don't need to do anything.
	else if ( (board[yloc * xdim + xloc] & hiddenBit() ) == 0) {
		return 2;
	}
	//Is GOOSE=true (9)? Then we show 9.
	else if ( (board[yloc*xdim+xloc] & valueMask() ) == 9) {
		board[yloc * xdim + xloc] = board[yloc * xdim + xloc] & valueMask();
		return 9;
	}
	//Check for a few other things before this: the below is ONLY if computeNeighbours is 0
	//Check if what we'll reveal is going to be 0: if it is, we need to reveal all 9 tiles.
	if ( (board[yloc*xdim+xloc] & valueMask() ) == 0) {


		//THIS BE A REPEAT. IF X AND Y ARE 0:
		if ((yloc == 0) and (xloc == 0)){
			for (unsigned long int reveal_y {yloc}; reveal_y <= yloc + 1; reveal_y++) {
						for (unsigned long int reveal_x {xloc}; reveal_x <= xloc + 1; reveal_x++) {
							//Check if TILE is within board maximum
							if (reveal_x < xdim && reveal_y < ydim && reveal_x >= 0 && reveal_y >= 0) {
								//Make sure the TILE is NOT MARKED
								if ( (board[reveal_y * xdim + reveal_x] & markedBit() ) == 0) {
									//reveal space
									board[reveal_y * xdim + reveal_x] = board[reveal_y * xdim + reveal_x] & valueMask();
									//Alternate: board[reveal_y * xdim + reveal_x] = board[reveal_y * xdim + reveal_x] & ~hiddenBit();
								}
							}
						}
					}
		}
		//BE REPEAT 2. IF X BUT NOT Y ARE 0.
		else if ((yloc > 0) and (xloc == 0)){
			for (unsigned long int reveal_y {yloc-1}; reveal_y <= yloc + 1; reveal_y++) {
				for (unsigned long int reveal_x {xloc}; reveal_x <= xloc + 1; reveal_x++) {
					//Check if TILE is within board maximum
					if (reveal_x < xdim && reveal_y < ydim && reveal_x >= 0 && reveal_y >= 0) {
						//Make sure the TILE is NOT MARKED
						if ( (board[reveal_y * xdim + reveal_x] & markedBit() ) == 0) {
							//reveal space
							board[reveal_y * xdim + reveal_x] = board[reveal_y * xdim + reveal_x] & valueMask();
						}
					}
				}
			}
		}
		//BE REPEAT 3. IF NOT X BUT Y ARE 0.
		else if ((yloc == 0) and (xloc > 0)){
			for (unsigned long int reveal_y {yloc}; reveal_y <= yloc + 1; reveal_y++) {
				for (unsigned long int reveal_x {xloc-1}; reveal_x <= xloc + 1; reveal_x++) {
					//Check if TILE is within board maximum
					if (reveal_x < xdim && reveal_y < ydim && reveal_x >= 0 && reveal_y >= 0) {
						//Make sure the TILE is NOT MARKED
						if ( (board[reveal_y * xdim + reveal_x] & markedBit() ) == 0) {
							//reveal space
							board[reveal_y * xdim + reveal_x] = board[reveal_y * xdim + reveal_x] & valueMask();
						}
					}
				}
			}
		}
		else {
			for (unsigned long int reveal_y {yloc-1}; reveal_y <= yloc + 1; reveal_y++) {
							for (unsigned long int reveal_x {xloc-1}; reveal_x <= xloc + 1; reveal_x++) {
								//Check if TILE is within board maximum
								if (reveal_x < xdim && reveal_y < ydim && reveal_x >= 0 && reveal_y >= 0) {
									//Make sure the TILE is NOT MARKED
									if ( (board[reveal_y * xdim + reveal_x] & markedBit() ) == 0) {
										//reveal space
										board[reveal_y * xdim + reveal_x] = board[reveal_y * xdim + reveal_x] & valueMask();
									}
								}
							}
						}
		}
	}
	//otherwise, just reveal the space
	else {
		board[yloc * xdim + xloc] = board[yloc * xdim + xloc] & valueMask();
	}
	return 0;
}

//OK
int mark(char *board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc) {
	//If it's not hidden, return 2.
	if ( (board[yloc * xdim + xloc] & hiddenBit() ) == 0) {
		return 2;
	}
	//Otherwise see if MARK = false, then add mark..
	if ( (board[yloc * xdim + xloc] & markedBit() ) == 0) {
		board[yloc * xdim + xloc] |= markedBit();
	}
	else {
		board[yloc * xdim + xloc] ^= markedBit();
	}
	return 0;
}

//OK
bool isGameWon(char *board, std::size_t xdim, std::size_t ydim) {
//Let's check every single tile. A victory
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

