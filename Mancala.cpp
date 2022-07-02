#include <iostream>

#include "board.h"

using namespace std;

int main() {
  cout << "Welcome to my mancala app!\n";
  Board board;

  bool playerOneTurn = true;
  while (!board.gameOver()) {
    // Determine the player for this turn
    string player;
    if (playerOneTurn) {
      player = board.PLAYER_1;
    } else {
      player = board.PLAYER_2;
    }

    // Get the position the user wants to select to start their move at
    int position;
    cout << player << ", enter a position to make a move: ";
    cin >> position;
    cout << "\n";

    // Make the move for the current player
    bool success = board.makeMoveAtPosition(position, player);

    // Change whose turn it is when move was successful
    if (success) {
      playerOneTurn = !playerOneTurn;
    }
  }

  cout << board.getWinner();
}