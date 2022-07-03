#include "Board.h"

#include <map>
#include <string>
#include <iostream>

using namespace Mancala;
using namespace std;

#pragma region Private

void Mancala::Board::dropStoneAtPosition(int position) {
	if (position >= 0) {
		_board[position % _NUMBER_OF_CUPS]++;
	}
}

int Mancala::Board::takeAllFromPosition(int position) {
	if (position < _NUMBER_OF_CUPS && position >= 0) {
		int count = getCountAtPosition(position);
		_board[position] = 0;
		return count;
	}
	else {
		return _INVALID_POSITION;
	}
}

void Mancala::Board::addPoint(string player) {
	if (player == PLAYER_1) {
		_playerOnePoints++;
	}
	else if (player == PLAYER_2) {
		_playerTwoPoints++;
	}
}

#pragma endregion

Mancala::Board::Board() : _playerOnePoints(0), _playerTwoPoints(0), _NUMBER_OF_CUPS(12), _STONES_PER_CUP(4), _INVALID_POSITION(-1) {
	this->PLAYER_1 = "PLAYER 1";
	this->PLAYER_2 = "PLAYER 2";
	reset();
}

#pragma region Public

void Mancala::Board::reset() {
	// Clear the board and reset the player's points
	_board.clear();
	_playerOnePoints = 0;
	_playerTwoPoints = 0;

	// Initialize the board with all of the cups containing 4 stones each
	for (int i = 0; i < _NUMBER_OF_CUPS; i++) {
		_board.insert(pair<int, int>(i, _STONES_PER_CUP));
	}
}

bool Mancala::Board::gameOver() {
	if (_playerOnePoints + _playerTwoPoints ==
		_STONES_PER_CUP * _NUMBER_OF_CUPS) {
		return true;
	}
	else {
		return false;
	}
}

string Mancala::Board::getWinner() {
	if (_playerOnePoints > _playerTwoPoints) {
		return PLAYER_1 + " won!\n";
	}
	else if (_playerOnePoints < _playerTwoPoints) {
		return PLAYER_2 + " won!\n";
	}
	else {
		return "It's a tie!\n";
	}
}

string Mancala::Board::getFinalScore() {
	return "The final score was " + to_string(_playerOnePoints) + "-" +
		to_string(_playerTwoPoints) + "\n";
}

int Mancala::Board::getCountAtPosition(int position) {
	if (position >= 0) {
		return _board[position % _NUMBER_OF_CUPS];
	}
	else {
		return _INVALID_POSITION;
	}
}

int Mancala::Board::getPlayerOneScore() {
	return _playerOnePoints;
}

int Mancala::Board::getPlayerTwoScore() {
	return _playerTwoPoints;
}

bool Mancala::Board::makeMoveAtPosition(int position, string player) {
	bool isValidForPlayer1 = position < (_NUMBER_OF_CUPS / 2);
	bool isValidForPlayer2 = position >= (_NUMBER_OF_CUPS / 2);
	bool isValidForCurrentPlayer = (isValidForPlayer1 && player == PLAYER_1) ||
		(isValidForPlayer2 && player == PLAYER_2);

	if (position >= 0 && position < _NUMBER_OF_CUPS &&
		isValidForCurrentPlayer) {
		int stonesInHand = takeAllFromPosition(position);
		position++;

		while (stonesInHand > 0) {
			int relPos = position % _NUMBER_OF_CUPS;
			bool atGoal = (relPos == _NUMBER_OF_CUPS / 2 && player == PLAYER_1) ||
				(relPos == 0 && player == PLAYER_2);

			if (atGoal && stonesInHand >= 2) {
				addPoint(player);
				dropStoneAtPosition(position);
				stonesInHand -= 2;
			}
			else if (atGoal) {
				addPoint(player);
				stonesInHand--;
			}
			else {
				dropStoneAtPosition(position);
				stonesInHand--;
			}

			position++;
		}

		return true;
	}
	else {
		std::cout << "Invalid position for move. Please try again.\n";

		return false;
	}
}

#pragma endregion

// void printBoard() {
//   const int rows = 10;
//   const int cols = 50;

//  for (int i = 0; i < rows; i++) {
//    cout << char(179);
//    for (int j = 0; j < cols; j++) {
//      cout << char(196);
//    }
//    cout << char(179);
//    cout << "\n";
//  }
//}

