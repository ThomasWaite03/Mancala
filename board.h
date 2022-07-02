#include <map>
#include <string>

using namespace std;

class Board {
 private:
  map<int, int> _board;
  int _playerOnePoints = 0;
  int _playerTwoPoints = 0;
  const int _NUMBER_OF_CUPS = 12;
  const int _STONES_PER_CUP = 4;
  const int _INVALID_POSITION = -1;

  int getCountAtPosition(int position) {
    if (position >= 0) {
      return _board[position % _NUMBER_OF_CUPS];
    } else {
      return _INVALID_POSITION;
    }
  }

  void dropStoneAtPosition(int position) {
    if (position >= 0) {
      _board[position % _NUMBER_OF_CUPS]++;
    }
  }

  int takeAllFromPosition(int position) {
    if (position < _NUMBER_OF_CUPS && position >= 0) {
      int count = getCountAtPosition(position);
      _board[position] = 0;
      return count;
    } else {
      return _INVALID_POSITION;
    }
  }

  void addPoint(string player) {
    if (player == PLAYER_1) {
      _playerOnePoints++;
    } else if (player == PLAYER_2) {
      _playerTwoPoints++;
    }
  }

 public:
  const string PLAYER_1 = "PLAYER 1";
  const string PLAYER_2 = "PLAYER 2";

  Board() { reset(); }

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

  bool gameOver() {
    if (_playerOnePoints + _playerTwoPoints ==
        _STONES_PER_CUP * _NUMBER_OF_CUPS) {
      return true;
    } else {
      return false;
    }
  }

  string getWinner() {
    if (_playerOnePoints > _playerTwoPoints) {
      return PLAYER_1 + " won!\n";
    } else if (_playerOnePoints < _playerTwoPoints) {
      return PLAYER_2 + " won!\n";
    } else {
      return "It's a tie!\n";
    }
  }

  string getFinalScore() { 
      return "The final score was " + to_string(_playerOnePoints) + "-" +
           to_string(_playerTwoPoints) + "\n";
  }

  bool makeMoveAtPosition(int position, string player) {
    bool isValidForPlayer1 = position < (_NUMBER_OF_CUPS / 2);
    bool isValidForPlayer2 = position >= (_NUMBER_OF_CUPS / 2);
    bool isValidForCurrentPlayer = (isValidForPlayer1 && player == PLAYER_1) ||
                                   (isValidForPlayer2 && player == PLAYER_2);

    if (position >= 0 && position < _NUMBER_OF_CUPS && isValidForCurrentPlayer) {
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
        } else if (atGoal) {
          addPoint(player);
          stonesInHand--;
        } else {
          dropStoneAtPosition(position);
          stonesInHand--;
        }

        position++;
      }

      for (int i = 0; i < _board.size(); i++) {
        std::cout << "Position " << i << ": " << _board[i] << "\n";
      }

      return true;
    } else {
      std::cout << "Invalid position for move. Please try again.\n";

      return false;
    }
  }
};
