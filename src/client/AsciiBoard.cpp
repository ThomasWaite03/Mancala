#include "AsciiBoard.h"
#include <iostream>
#include <string>

using namespace Mancala;
using namespace std;

Mancala::AsciiBoard::AsciiBoard() : _horizontalLine(196), _verticalLine(179), _topLeftCorner(218), _topRightCorner(191),
_bottomLeftCorner(192), _bottomRightCorner(217), _space(32), _storeWidth(10), _pocketWidth(15), _rows(10), _cols(110) {}

#pragma region Public

void Mancala::AsciiBoard::draw() {

	// Draw the outline of the game board
	drawTopOfBoard();
	drawMiddleOfBoard();
	drawBottomOfBoard();
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

#pragma endregion

#pragma region Private

void Mancala::AsciiBoard::drawTopOfBoard() {
	const char topIntersection = char(194);

	// Print the column labels
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

	// Print the top line of the actual board
	cout << char(_topLeftCorner);
	for (int col = 0; col < _cols; col++) {
		if ((col - (_storeWidth - 1)) % _pocketWidth == 0 && col >= _storeWidth - 1 && col < _cols - _storeWidth) {
			cout << topIntersection;
		}
		else {
			cout << char(_horizontalLine);
		}
	}
	cout << char(_topRightCorner) << "\n";
}

void Mancala::AsciiBoard::drawBottomOfBoard() {
	const char bottomIntersection = char(193);

	// Print the bottom line of the board
	cout << char(_bottomLeftCorner);
	for (int col = 0; col < _cols; col++) {
		if ((col - (_storeWidth - 1)) % _pocketWidth == 0 && col >= _storeWidth - 1 && col < _cols - _storeWidth) {
			cout << bottomIntersection;
		}
		else {
			cout << char(_horizontalLine);
		}
	}
	cout << char(_bottomRightCorner) << "\n\n";
}

void Mancala::AsciiBoard::drawMiddleOfBoard() {
	const char fullIntersection = char(197);
	const char leftIntersection = char(195);
	const char rightIntersection = char(180);
	
	const int middleRow = _rows / 2;

	for (int row = 0; row < _rows + 1; row++) {
		std::cout << char(_verticalLine);
		for (int col = 0; col < _cols; col++) {
			char charToPrint = char(_space);

			// Print out the different box drawing characters to create an outline of the game board
			bool inInnerBoard = col < (_cols - _storeWidth - 1) && col >= _storeWidth;
			bool isDividerInterval = (col - (_storeWidth - 1)) % _pocketWidth == 0;
			if (col == _storeWidth - 1 && row == middleRow) {
				charToPrint = leftIntersection;
			}
			else if (col == _cols - _storeWidth - 1 && row == middleRow) {
				charToPrint = rightIntersection;
			}
			else if (col == _storeWidth - 1 || col == _cols - _storeWidth - 1) {
				charToPrint = char(_verticalLine);
			}
			else if (row == middleRow && inInnerBoard && isDividerInterval) {
				charToPrint = fullIntersection;
			}
			else if (inInnerBoard && isDividerInterval) {
				charToPrint = char(_verticalLine);
			}
			else if (row == middleRow && inInnerBoard) {
				charToPrint = char(_horizontalLine);
			}

			// Print out numeric values for the number of stones in each pocket or store
			if (getBoardIndexByCoords(row, col) != -1 && inInnerBoard && !isDividerInterval) {
				int idx = getBoardIndexByCoords(row, col);
				printIntValue(getCountAtPosition(idx));
				col++;
			}
			else if (row == middleRow && col == (_storeWidth / 2) - 1) {
				printIntValue(getPlayerTwoScore());
				col++;
			}
			else if (row == middleRow && col == _cols - (_storeWidth / 2) - 1) {
				printIntValue(getPlayerOneScore());
				col++;
			}
			else {
				cout << charToPrint;
			}
		}
		cout << char(_verticalLine);
		cout << "\n";
	}
}

void Mancala::AsciiBoard::printIntValue(int val) {
	string valueString = to_string(val);
	if (valueString.length() > 1) {
		cout << valueString;
	}
	else {
		cout << " " + valueString;
	}
}

int Mancala::AsciiBoard::getBoardIndexByCoords(int row, int col) {
	bool isMiddleColOfPocket = (col - (_pocketWidth + 1)) % _pocketWidth == 0;
	bool inBounds = col > _storeWidth && col < _cols - _storeWidth;
	bool isMiddleOfTopRow = row == _rows / 4;
	bool isMiddleOfBottomRow = row == _rows - (_rows / 4);

	if (isMiddleOfTopRow && isMiddleColOfPocket && inBounds) {
		// returns the board index for the top row
		return _NUMBER_OF_POCKETS - 1 - ((col - (_pocketWidth + 1)) / _pocketWidth);
	}
	else if (isMiddleOfBottomRow && isMiddleColOfPocket && inBounds) {
		// returns the board index for the bottom row
		return (col - (_pocketWidth + 1)) / _pocketWidth;
	}
	else {
		return -1;
	}
}

#pragma endregion
