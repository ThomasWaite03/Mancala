#include "AsciiBoard.h"
#include <iostream>
#include <string>

using namespace Mancala;
using namespace std;

Mancala::AsciiBoard::AsciiBoard() : _horizontalLine(196), _verticalLine(179), _topLeftCorner(218), _topRightCorner(191),
_bottomLeftCorner(192), _bottomRightCorner(217), _space(32), _storeWidth(10), _pocketWidth(15) {}

void Mancala::AsciiBoard::draw() {
	int rows = 10;
	int cols = 110;

	// Draw the outline of the game board
	drawTopOfBoard(cols);
	drawMiddleOfBoard(rows, cols);
	drawBottomOfBoard(cols);
}

void Mancala::AsciiBoard::drawTopOfBoard(int cols) {
	for (int i = 0; i < cols; i++) {
		if ((i - 17) % 15 == 0 && i > _storeWidth && i < cols - _storeWidth) {
			cout << "#" << ((i - 17) / 15) + 1;
			i++;
		}
		else {
			cout << char(_space);
		}
	}
	cout << "\n";

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
	cout << char(_bottomRightCorner) << "\n\n";
}

void Mancala::AsciiBoard::drawMiddleOfBoard(int rows, int cols) {
	for (int row = 0; row < rows + 1; row++) {
		cout << char(_verticalLine);
		for (int col = 0; col < cols; col++) {
			char charToPrint = char(_space);

			bool inInnerBoard = col < (cols - _storeWidth - 1) && col >= _storeWidth;
			bool isDividerInterval = (col - (_storeWidth - 1)) % _pocketWidth == 0;
			if (col == _storeWidth - 1 || col == cols - _storeWidth - 1 || (inInnerBoard && isDividerInterval)) {
				charToPrint = char(_verticalLine);
			}
			else if (row == rows / 2 && inInnerBoard) {
				charToPrint = char(_horizontalLine);
			}

			if (getBoardIndexByCoords(row, col) != -1 && inInnerBoard && !isDividerInterval) {
				int idx = getBoardIndexByCoords(row, col);
				cout << getCountAtPosition(idx);
			}
			else if (row == (rows / 2) && col == (_storeWidth / 2)) {
				cout << getPlayerTwoScore();
			}
			else if (row == (rows / 2) && col == cols - (_storeWidth / 2)) {
				cout << getPlayerOneScore();
			}
			else {
				cout << charToPrint;
			}
		}
		cout << char(_verticalLine);
		cout << "\n";
	}
}

int Mancala::AsciiBoard::getBoardIndexByCoords(int row, int col) {
	if (row == 2) {
		switch (col) {
		case 17:
			return 11;
		case 32:
			return 10;
		case 47:
			return 9;
		case 62:
			return 8;
		case 77:
			return 7;
		case 92:
			return 6;
		default:
			return -1;
		}
	}
	else if (row == 8) {
		switch (col) {
		case 17:
			return 0;
		case 32:
			return 1;
		case 47:
			return 2;
		case 62:
			return 3;
		case 77:
			return 4;
		case 92:
			return 5;
		default:
			return -1;
		}
	}
	else {
		return -1;
	}
}

int Mancala::AsciiBoard::labelToBoardPosition(int labelValue, string player) {
	if (labelValue >= 1 && labelValue <= _NUMBER_OF_POCKETS / 2) {
		if (player == PLAYER_1) {
			return labelValue - 1;
		}
		else {
			return _NUMBER_OF_POCKETS - labelValue;
		}
	}
	else {
		return -1;
	}
}