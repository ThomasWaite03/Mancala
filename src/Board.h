#pragma once
#include <map>
#include <string>

namespace Mancala {
	class Board {
	protected:
		const int _NUMBER_OF_POCKETS;
		const int _STONES_PER_POCKET;
	private:
		std::map<int, int> _board;
		int _playerOnePoints;
		int _playerTwoPoints;
		std::string _moveErrorMsg;
		const int _INVALID_POSITION;

		void dropStoneAtPosition(int position);

		int takeAllFromPosition(int position);

		void addPoints(std::string player, int points);

	public:
		std::string PLAYER_1;
		std::string PLAYER_2;

		Board();

		void reset();

		bool gameOver();

		std::string getWinner();

		int getPlayerOneScore();

		int getPlayerTwoScore();

		int getCountAtPosition(int position);

		std::string getMoveErrorMessage();

		bool makeMoveAtPosition(int position, std::string player);
	};
}  // namespace Mancala
