#ifndef SWEEPSNAKES_LIB_H
#define SWEEPSNAKES_LIB_H

#include <iostream>

int main();
bool game();
void startGame(char * & board, std::size_t & xdim, std::size_t & ydim, unsigned int & numsnakes);
char getAction();
void actionShow(char * & board, std::size_t & xdim, std::size_t & ydim, unsigned int & numsnakes);
void actionMark(char * board, std::size_t xdim, std::size_t ydim);
std::size_t readSizeT();
std::size_t xdim_max();
std::size_t ydim_max();
char markedBit();
char hiddenBit();
char valueMask();
void spreadSnakes(char * board, std::size_t xdim, std::size_t ydim, unsigned int numsnakes);

#endif
