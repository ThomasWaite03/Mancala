#include <iostream>
#include "board.h"

using namespace std;

int main()
{
	std::cout << "Welcome to my mancala app!\n";
	Board board;
	std::cout << board.getCountAtPosition(4);
	board.startAtPosition(3, board.PLAYER_2);
}
