#include "Board.h"

#include <map>
#include <string>
#include <iostream>

using namespace Mancala;
using namespace std;

#pragma region Private

void Mancala::Board::dropStoneAtPosition(int position) {
	if (position >= 0) {
		_board[position % _NUMBER_OF_POCKETS]++;
	}
}

int Mancala::Board::takeAllFromPosition(int position) {
	if (position < _NUMBER_OF_POCKETS && position >= 0) {
		int count = getCountAtPosition(position);
		_board[position] = 0;
		return count;
	}
	else {
		return _INVALID_POSITION;
	}
}

void Mancala::Board::addPoints(string player, int points = 1) {
	if (player == PLAYER_1) {
		_playerOnePoints += points;
	}
	else if (player == PLAYER_2) {
		_playerTwoPoints += points;
	}
}

#pragma endregion

Mancala::Board::Board() : _playerOnePoints(0), _playerTwoPoints(0), _NUMBER_OF_POCKETS(12), _STONES_PER_POCKET(4), _INVALID_POSITION(-1) {
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

	// Initialize the board with all of the pockets containing 4 stones each
	for (int i = 0; i < _NUMBER_OF_POCKETS; i++) {
		_board.insert(pair<int, int>(i, _STONES_PER_POCKET));
	}
}

bool Mancala::Board::gameOver() {
	// See if a player's side is empty
	bool playerOneHasMove = false;
	bool playerTwoHasMove = false;
	for (int position = 0; position < _NUMBER_OF_POCKETS; position++) {
		if (position < _NUMBER_OF_POCKETS / 2) {
			playerOneHasMove = playerOneHasMove || getCountAtPosition(position);
		}
		else {
			playerTwoHasMove = playerTwoHasMove || getCountAtPosition(position);
		}
	}

	// When one side has all empty pockets, then the game is over and the other player captures the remaining stones
	int stonesLeft = (_NUMBER_OF_POCKETS * _STONES_PER_POCKET) - (getPlayerOneScore() + getPlayerTwoScore());
	if (!playerOneHasMove) {
		addPoints(PLAYER_2, stonesLeft);
		_board.clear();
		return true;
	}
	else if (!playerTwoHasMove) {
		addPoints(PLAYER_1, stonesLeft);
		_board.clear();
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

int Mancala::Board::getCountAtPosition(int position) {
	if (position >= 0) {
		return _board[position % _NUMBER_OF_POCKETS];
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
	bool isValidForPlayer1 = position < (_NUMBER_OF_POCKETS / 2);
	bool isValidForPlayer2 = position >= (_NUMBER_OF_POCKETS / 2);
	bool isValidForCurrentPlayer = (isValidForPlayer1 && player == PLAYER_1) ||
		(isValidForPlayer2 && player == PLAYER_2);

	if (position >= 0 && position < _NUMBER_OF_POCKETS &&
		isValidForCurrentPlayer) {
		int stonesInHand = takeAllFromPosition(position);
		position++;

		while (stonesInHand > 0) {
			int relPos = position % _NUMBER_OF_POCKETS;
			bool atGoal = (relPos == _NUMBER_OF_POCKETS / 2 && player == PLAYER_1) ||
				(relPos == 0 && player == PLAYER_2);

			if (atGoal && stonesInHand >= 2) {
				addPoints(player);
				dropStoneAtPosition(position);
				stonesInHand -= 2;
			}
			else if (atGoal) {
				// Last stone ends in the store
				addPoints(player);
				return false;
			}
			else if (stonesInHand == 1 && getCountAtPosition(position) == 0 &&
				((relPos < _NUMBER_OF_POCKETS / 2 && player == PLAYER_1) || (relPos >= _NUMBER_OF_POCKETS / 2 && player == PLAYER_2))) {
				// When the last stone is in an empty cup on the players side, then store the stone and those in the opposite cup
				int oppositePosition = _NUMBER_OF_POCKETS - 1 - relPos;
				addPoints(player, takeAllFromPosition(oppositePosition) + 1);
				return true;
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
		std::cout << "Invalid position for move. Please try again.\n\n";

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

