#include "AsciiBoard.h"
#include <iostream>
#include <string>

using namespace Mancala;
using namespace std;

Mancala::AsciiBoard::AsciiBoard() : _horizontalLine(196), _verticalLine(179), _topLeftCorner(218), _topRightCorner(191),
_bottomLeftCorner(192), _bottomRightCorner(217), _space(32), _storeWidth(10), _pocketWidth(15), _rows(10), _cols(110) {}

void Mancala::AsciiBoard::draw() {

	// Draw the outline of the game board
	drawTopOfBoard();
	drawMiddleOfBoard();
	drawBottomOfBoard();
}

void Mancala::AsciiBoard::drawTopOfBoard() {
	for (int col = 0; col < _cols; col++) {
		if ((col - (_pocketWidth + 2)) % _pocketWidth == 0 && col > _storeWidth && col < _cols - _storeWidth) {
			cout << "#" << ((col - (_pocketWidth + 2)) / _pocketWidth) + 1;
			col++;
		}
		else {
			cout << char(_space);
		}
	}
	cout << "\n";

	cout << char(_topLeftCorner);
	for (int col = 0; col < _cols; col++) {
		cout << char(_horizontalLine);
	}
	cout << char(_topRightCorner) << "\n";
}

void Mancala::AsciiBoard::drawBottomOfBoard() {
	cout << char(_bottomLeftCorner);
	for (int col = 0; col < _cols; col++) {
		cout << char(_horizontalLine);
	}
	cout << char(_bottomRightCorner) << "\n\n";
}

void Mancala::AsciiBoard::drawMiddleOfBoard() {
	for (int row = 0; row < _rows + 1; row++) {
		cout << char(_verticalLine);
		for (int col = 0; col < _cols; col++) {
			char charToPrint = char(_space);

			bool inInnerBoard = col < (_cols - _storeWidth - 1) && col >= _storeWidth;
			bool isDividerInterval = (col - (_storeWidth - 1)) % _pocketWidth == 0;
			if (col == _storeWidth - 1 || col == _cols - _storeWidth - 1 || (inInnerBoard && isDividerInterval)) {
				charToPrint = char(_verticalLine);
			}
			else if (row == _rows / 2 && inInnerBoard) {
				charToPrint = char(_horizontalLine);
			}

			if (getBoardIndexByCoords(row, col) != -1 && inInnerBoard && !isDividerInterval) {
				int idx = getBoardIndexByCoords(row, col);
				cout << getCountAtPosition(idx);
			}
			else if (row == (_rows / 2) && col == (_storeWidth / 2)) {
				cout << getPlayerTwoScore();
			}
			else if (row == (_rows / 2) && col == _cols - (_storeWidth / 2)) {
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
	if (row == _rows / 4 && (col - (_pocketWidth + 2)) % _pocketWidth == 0 && col > _storeWidth && col < 110 - _storeWidth) {
		return _NUMBER_OF_POCKETS - 1 - ((col - (_pocketWidth + 2)) / _pocketWidth);
	}
	else if (row == _rows - (_rows / 4) && (col - (_pocketWidth + 2)) % _pocketWidth == 0 && col > _storeWidth && col < _cols - _storeWidth) {
		return (col - (_pocketWidth + 2)) / _pocketWidth;
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