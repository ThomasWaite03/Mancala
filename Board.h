#pragma once
#include <map>
#include <string>

namespace Mancala {
	class Board {
	private:
		std::map<int, int> _board;
		int _playerOnePoints;
		int _playerTwoPoints;
		const int _NUMBER_OF_CUPS;
		const int _STONES_PER_CUP;
		const int _INVALID_POSITION;

		void dropStoneAtPosition(int position);

		int takeAllFromPosition(int position);

		void addPoint(std::string player);

	public:
		std::string PLAYER_1;
		std::string PLAYER_2;

		Board();

		void reset();

		bool gameOver();

		std::string getWinner();

		std::string getFinalScore();

		int getPlayerOneScore();

		int getPlayerTwoScore();

		int getCountAtPosition(int position);

		bool makeMoveAtPosition(int position, std::string player);
	};
}  // namespace Mancala
