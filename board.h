#include <map>
#include <compare>
#include <string>

using namespace std;

class Board {
private:
	map<int, int> _board;
	int _playerOnePoints = 0;
	int _playerTwoPoints = 0;
	const int _NUMBER_OF_CUPS = 12;
	const int _STONES_PER_CUP = 4;
	const int _EMPTY_CUP = -1;

public:
	const string PLAYER_1 = "PLAYER 1";
	const string PLAYER_2 = "PLAYER 2";

	Board() {
		reset();
	}

	void reset() {
		// Clear the board and reset the player's points
		_board.clear();
		_playerOnePoints = 0;
		_playerTwoPoints = 0;

		// Initialize the board with all of the cups containing 4 stones each
		for (int i = 0; i < _NUMBER_OF_CUPS; i++) {
			_board.insert(pair<int, int>(i, _STONES_PER_CUP));
		}
	}

	int getCountAtPosition(int position) {
		if (position < _NUMBER_OF_CUPS) {
			return _board[position];
		}
		else {
			return _EMPTY_CUP;
		}
	}

	void startAtPosition(int position, string player) {
		if (player == PLAYER_1) {
			std::cout << "Player 1";
		}
		else {
			std::cout << "Player 2";
		}
	}
};
