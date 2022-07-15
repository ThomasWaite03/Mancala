#include "AsciiBoard.h"

#include <iostream>
#include <cpprest/ws_client.h>
#include <cpprest/json.h>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/lexical_cast.hpp>

using namespace Mancala;
using namespace std;
using namespace web;
using namespace web::websockets::client;

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
		cout << endl;

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

void MultiplayerGameLoop(AsciiBoard board, websocket_client client, string gameID, bool isPlayerOne) {
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

		// If it is the players turn, let them make a move.
		int position;
		if (playerOneTurn == isPlayerOne) {
			// Get the position the user wants to select to start their move at
			cout << player << ", enter the pocket number for your move: ";
			cin >> position;
			cout << endl;

			// Convert the position entered that matches ASCII board labels to the actual board position
			position = board.labelToBoardPosition(position, player);
		}
		else {
			// Otherwise get the remote player's move.
			string playerName = isPlayerOne ? board.PLAYER_1 : board.PLAYER_2;
			cout << "Waiting for " << playerName << " to finish turn." << endl;
			/*Concurrency::task<string> getMoveTask = client.receive().get().extract_string();
			string body = getMoveTask.get();
			getMoveTask.wait();
			position = std::stoi(body);*/

			Concurrency::task<string> getMoveTask = client.receive().then([](websocket_incoming_message msg) {
				return msg.extract_string();
			});
			getMoveTask.wait();
			string positionString = getMoveTask.get();
			position = std::stoi(positionString);
		}

		// Make the move for the current player
		bool switchPlayer = board.makeMoveAtPosition(position, player);

		// If the local player made an error free move, send a message
		if (playerOneTurn == isPlayerOne && board.getMoveErrorMessage() == "") {
			string playerName = isPlayerOne ? board.PLAYER_1 : board.PLAYER_2;
			string sendMoveRequest = "{\"gameID\": \"" + gameID + "\", \"name\": \"" + playerName + "\", \"move\": " + to_string(position) + ", \"status\": \"makingMove\"}";
			websocket_outgoing_message sendMoveMsg;
			sendMoveMsg.set_utf8_message(sendMoveRequest);
			client.send(sendMoveMsg).wait();
		}

		// Change whose turn it is when move was successful
		if (switchPlayer) {
			playerOneTurn = !playerOneTurn;
		}

		// Clear the screen after each turn
		system("cls");
	}

	board.draw();
	cout << board.getWinner();

	client.close();
}

void HostGame(AsciiBoard board) {
	// Generate a random unique game id
	boost::uuids::random_generator gen;
	boost::uuids::uuid gameUUID = gen();
	const std::string gameID = boost::lexical_cast<std::string>(gameUUID);

	// Create the websocket client and connect to the server
	websocket_client client;
	client.connect(U("ws://localhost:8080")).then([]() {
		cout << "Connected to server." << endl;
	}).wait();

	// Get player's name
	string playerName;
	cout << "Enter name: ";
	cin >> playerName;
	cout << endl;

	string startGameRequest = "{\"gameID\": \"" + gameID + "\", \"name\": \"" + playerName + "\", \"status\": \"hosting\"}";
	websocket_outgoing_message startGameRequestMsg;
	startGameRequestMsg.set_utf8_message(startGameRequest);
	client.send(startGameRequestMsg).wait();

	// Get the name of the other player once they join.
	Concurrency::task<string> getJoiningPlayerTask = client.receive().get().extract_string();
	string body = getJoiningPlayerTask.get();
	json::value joiningPlayerResponse = json::value::parse(body);
	json::object joiningPlayerJSON = joiningPlayerResponse.as_object();
	string joiningPlayer = utility::conversions::to_utf8string(joiningPlayerJSON.at(U("joiningName")).as_string());

	// Set player names
	board.PLAYER_1 = playerName;
	board.PLAYER_2 = joiningPlayer;

	// Start game loop
	MultiplayerGameLoop(board, client, gameID, true);
}

void JoinGame(AsciiBoard board) {
	// Create the websocket client and connect to the server
	websocket_client client;
	client.connect(U("ws://localhost:8080")).then([]() {
		cout << "Connected to server." << endl;
	}).wait();

	// get player's name
	string playerName;
	cout << "Enter name: ";
	cin >> playerName;
	cout << endl;

	// Send request to get a list of available hosts
	string getHostsRequest = "{\"name\": \"" + playerName + "\", \"status\": \"joining\"}";
	websocket_outgoing_message getHostsRequestMsg;
	getHostsRequestMsg.set_utf8_message(getHostsRequest);
	client.send(getHostsRequestMsg).wait();

	// Get the response
	Concurrency::task<string> getHostsArrayTask = client.receive().get().extract_string();
	string body = getHostsArrayTask.get();
	json::value hosts = json::value::parse(body);
	json::array hostsArray = hosts.as_array();

	// List the available hosts
	cout << "Live Hosts:" << endl << "----------------" << endl;
	for (int i = 0; i < hostsArray.size(); i++) {
		wcout << "(" << i + 1 << ") " << hostsArray.at(0).as_string() << endl;
	}

	// Get the host the user wants to connect with
	int hostIndex = 0;
	while (hostIndex < 1 || hostIndex > hostsArray.size()) {
		cout << endl << "Enter number of host to connect with: ";
		cin >> hostIndex;
		cout << endl;
	}
	string hostName = utility::conversions::to_utf8string(hostsArray.at(hostIndex - 1).as_string());

	// Send request to start game
	string startGameRequest = "{\"name\": \"" + playerName + "\", \"hostName\": \"" + hostName + "\", \"status\": \"joining\"}";
	websocket_outgoing_message startGameRequestMsg;
	startGameRequestMsg.set_utf8_message(startGameRequest);
	client.send(startGameRequestMsg).wait();

	Concurrency::task<string> getJoinResponseTask = client.receive().get().extract_string();
	body = getJoinResponseTask.get();
	json::value joinResponse = json::value::parse(body);
	json::object joinResponseJSON = joinResponse.as_object();
	string gameID = utility::conversions::to_utf8string(joinResponseJSON.at(U("gameID")).as_string());

	// Set player names
	board.PLAYER_1 = hostName;
	board.PLAYER_2 = playerName;

	// Start game loop
	MultiplayerGameLoop(board, client, gameID, false);
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
			HostGame(board);
			break;
		case 2:
			JoinGame(board);
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
		//system("cls");
	}
}


