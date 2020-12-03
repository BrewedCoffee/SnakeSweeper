#ifndef GEESESPOTTER_H
#define GEESESPOTTER_H

char * makeBoard(std::size_t xdim, std::size_t ydim);
void computeSnakes(char * board, std::size_t xdim, std::size_t ydim);
void hideBoard(char * board, std::size_t xdim, std::size_t ydim);
void clearBoard(char * board);
void printBoard(char * board, std::size_t xdim, std::size_t ydim);
int reveal(char * board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc);
int mark(char * board, std::size_t xdim, std::size_t ydim, std::size_t xloc, std::size_t yloc);
bool isWon(char * board, std::size_t xdim, std::size_t ydim);

#endif
