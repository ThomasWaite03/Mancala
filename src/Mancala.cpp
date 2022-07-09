#include <iostream>

#include "AsciiBoard.h"

using namespace Mancala;
using namespace std;

void OriginalGameLoop(AsciiBoard board) {
	bool playerOneTurn = true;
	while (!board.gameOver()) {
		// Print the current state of the game board
		board.draw();

		// Determine the player for this turn
		string player;
		if (playerOneTurn) {
			player = board.PLAYER_1;
		}
		else {
			player = board.PLAYER_2;
		}

		// If there is a error message about the previous move it will display here or print an empty string
		cout << board.getMoveErrorMessage();

		// Get the position the user wants to select to start their move at
		int position;
		cout << player << ", enter the pocket number for your move: ";
		cin >> position;
		cout << "\n";

		// Convert the position entered that matches ASCII board labels to the actual board position
		position = board.labelToBoardPosition(position, player);

		// Make the move for the current player
		bool switchPlayer = board.makeMoveAtPosition(position, player);

		// Change whose turn it is when move was successful
		if (switchPlayer) {
			playerOneTurn = !playerOneTurn;
		}

		// Clear the screen after each turn
		system("cls");
	}

	board.draw();
	cout << board.getWinner();
}

int main() {
	cout << "Welcome to my mancala app!" << endl << endl;

	string menuError = "";

	bool exit = false;
	while (exit == false) {
		// Print menu
		cout << "Menu:" << endl << "------------------" << endl;
		cout << "(1) Host Game" << endl;
		cout << "(2) Join Game" << endl;
		cout << "(3) Play Locally" << endl;
		cout << "(4) Exit" << endl << endl;

		// If there's a menu error, print it here
		if (menuError != "") {
			cout << menuError << endl << endl;
			menuError = "";
		}

		// Get menu choice
		int menuChoice;
		cout << "Enter choice from menu: ";
		cin >> menuChoice;
		cout << endl;

		AsciiBoard board;
		system("cls");
		switch (menuChoice) {
			case 1:
				break;
			case 2:
				break;
			case 3:
				OriginalGameLoop(board);
				break;
			case 4:
				exit = true;
				break;
			default:
				menuError = "Please enter a valid menu option.";
				break;
		}

		// Clear screen, so menu can display again
		system("cls");
	}
}


