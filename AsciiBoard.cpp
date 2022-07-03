#include "AsciiBoard.h"
#include <iostream>

using namespace Mancala;
using namespace std;

Mancala::AsciiBoard::AsciiBoard() : _horizontalLine(196), _verticalLine(179), _topLeftCorner(218), _topRightCorner(191),
_bottomLeftCorner(192), _bottomRightCorner(217), _space(32) {}

void Mancala::AsciiBoard::draw() {
	int rows = 10;
	int cols = 110;

	// Draw the outline of the game board
	drawTopOfBoard(cols);
	drawMiddleOfBoard(rows, cols);
	drawBottomOfBoard(cols);
}

void Mancala::AsciiBoard::drawTopOfBoard(int cols) {
	cout << char(_topLeftCorner);
	for (int i = 0; i < cols; i++) {
		cout << char(_horizontalLine);
	}
	cout << char(_topRightCorner) << "\n";
}

void Mancala::AsciiBoard::drawBottomOfBoard(int cols) {
	cout << char(_bottomLeftCorner);
	for (int i = 0; i < cols; i++) {
		cout << char(_horizontalLine);
	}
	cout << char(_bottomRightCorner) << "\n";
}

void Mancala::AsciiBoard::drawMiddleOfBoard(int rows, int cols) {
	int bankWidth = 10;
	int cupWidth = 15;

	for (int row = 0; row < rows + 1; row++) {
		cout << char(_verticalLine);
		for (int col = 0; col < cols; col++) {
			char charToPrint = char(_space);

			bool inInnerBoard = col < (cols - bankWidth - 1) && col >= bankWidth;
			bool isDividerInterval = (col - (bankWidth - 1)) % cupWidth == 0;
			if (col == bankWidth - 1 || col == cols - bankWidth - 1 || (inInnerBoard && isDividerInterval)) {
				charToPrint = char(_verticalLine);
			} else if (row == rows / 2 && inInnerBoard) {
				charToPrint = char(_horizontalLine);
			}
			cout << charToPrint;
		}
		cout << char(_verticalLine);
		cout << "\n";
	}
}