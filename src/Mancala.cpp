#include "AsciiBoard.h"

#pragma comment(lib, "bcrypt.lib")

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

// Azure WebPubSub user access urls
const utility::string_t CONNECTION_1 = U("");
const utility::string_t CONNECTION_2 = U("");

void JoinGroup(websocket_client client) {
	// Join group
	json::value joinGroupJSON = json::value::object();
	joinGroupJSON[U("type")] = json::value::string(U("joinGroup"));
	joinGroupJSON[U("group")] = json::value::string(U("group1"));

	websocket_outgoing_message joinGroupMsg;
	joinGroupMsg.set_utf8_message(utility::conversions::to_utf8string(joinGroupJSON.serialize()));
	client.send(joinGroupMsg).wait();

	bool received = false;
	while (!received) {
		string responseString = "";
		try {
			client.receive().then([](websocket_incoming_message msg) {
				return msg.extract_string();
				}).then([&responseString](string body) {
					responseString = body;
					}).wait();
		}
		catch (websockets::client::websocket_exception ex) {
			std::cout << "Error message: " << ex.what() << endl;
			std::cout << "Error code: " << ex.error_code() << endl;
		}

		if (responseString != "") {
			std::cout << responseString << endl;
			received = true;
		}
	}
}

json::object WaitForNextMove(websocket_client client, string playerToListenFor) {
	while (true) {
		// try to receive a message
		string responseString;
		client.receive().then([](websocket_incoming_message msg) {
			return msg.extract_string();
			}).then([&responseString](string body) {
				responseString = body;
				}).wait();

				// If message is valid break loop
				try {
					json::value response = json::value::parse(responseString);

					string sender = utility::conversions::to_utf8string(response.at(U("fromUserId")).as_string());
					if (sender == playerToListenFor) {
						return response.as_object();
					}
				}
				catch (json::json_exception ex) {}
	}
}

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
		std::cout << board.getMoveErrorMessage();

		// Get the position the user wants to select to start their move at
		bool userHasMadeMove = false;
		int position;
		while (!userHasMadeMove) {
			string positionString;
			std::cout << player << ", enter the pocket number for your move: ";
			std::getline(cin, positionString);
			std::cout << endl;

			try {
				position = std::stoi(positionString);
				userHasMadeMove = true;
			}
			catch (std::invalid_argument const& ex) {
				std::system("cls");
				board.draw();
				std::cout << "Invalid input, please type in a number that represents the board location you want to select." << endl << endl;
			}
		}

		// Convert the position entered that matches ASCII board labels to the actual board position
		position = board.labelToBoardPosition(position, player);

		// Make the move for the current player
		bool switchPlayer = board.makeMoveAtPosition(position, player);

		// Change whose turn it is when move was successful
		if (switchPlayer) {
			playerOneTurn = !playerOneTurn;
		}

		// Clear the screen after each turn
		std::system("cls");
	}

	board.draw();
	std::cout << board.getWinner();

	// Press any key to continue...
	std::system("pause");
}

void OnlineGameLoop(AsciiBoard board, websocket_client client, string gameID, bool isPlayerOne) {
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
		std::cout << board.getMoveErrorMessage();

		// If it is the players turn, let them make a move.
		int position;
		if (playerOneTurn == isPlayerOne) {
			// Get the position the user wants to select to start their move at
			bool userHasMadeMove = false;
			while (!userHasMadeMove) {
				string positionString;
				std::cout << player << ", enter the pocket number for your move: ";
				std::getline(cin, positionString);
				std::cout << endl;

				try {
					position = std::stoi(positionString);
					userHasMadeMove = true;
				}
				catch (std::invalid_argument const& ex) {
					std::system("cls");
					board.draw();
					std::cout << "Invalid input, please type in a number that represents the board location you want to select." << endl << endl;
				}
			}

			// Convert the position entered that matches ASCII board labels to the actual board position
			position = board.labelToBoardPosition(position, player);
		}
		else {
			// Otherwise get the remote player's move.
			std::cout << "Waiting for " << player << " to finish turn." << endl;

			json::object moveJSON = WaitForNextMove(client, player);
			position = moveJSON.at(U("data")).at(U("move")).as_integer();
		}

		// Make the move for the current player
		bool switchPlayer = board.makeMoveAtPosition(position, player);

		// If the local player made an error free move, send a message
		if (playerOneTurn == isPlayerOne && board.getMoveErrorMessage() == "") {
			json::value data = json::value::object();
			data[U("move")] = json::value::number(position);

			// Create json object that stores a send message request and the latest move
			json::value sendMoveRequest = json::value::object();
			sendMoveRequest[U("type")] = json::value::string(U("sendToGroup"));
			sendMoveRequest[U("group")] = json::value::string(U("group1"));
			sendMoveRequest[U("dataType")] = json::value::string(U("json"));
			sendMoveRequest[U("data")] = data;

			websocket_outgoing_message sendMoveMsg;
			sendMoveMsg.set_utf8_message(utility::conversions::to_utf8string(sendMoveRequest.serialize()));
			client.send(sendMoveMsg).wait();
		}

		// Change whose turn it is when move was successful
		if (switchPlayer) {
			playerOneTurn = !playerOneTurn;
		}

		// Clear the screen after each turn
		std::system("cls");
	}

	board.draw();
	std::cout << board.getWinner();

	client.close();

	// Press any key to continue...
	std::system("pause");
}

void HostGame(AsciiBoard board) {
	// Generate a random unique game id
	boost::uuids::random_generator gen;
	boost::uuids::uuid gameUUID = gen();
	const std::string gameID = boost::lexical_cast<std::string>(gameUUID);

	// Create the websocket client and connect to the server
	websocket_client_config config;
	config.set_validate_certificates(false);
	config.add_subprotocol(utility::conversions::to_string_t("json.webpubsub.azure.v1"));
	websocket_client client(config);
	client.connect(CONNECTION_1).then([]() {
		std::cout << "Connected to server." << endl;
		}).wait();

		JoinGroup(client);

		// Set player names
		board.PLAYER_1 = "Player 1";
		board.PLAYER_2 = "Player 2";

		// Start game loop
		OnlineGameLoop(board, client, gameID, true);
}

void JoinGame(AsciiBoard board) {
	// Create the websocket client and connect to the server
	websocket_client_config config;
	config.set_validate_certificates(false);
	config.add_subprotocol(utility::conversions::to_string_t("json.webpubsub.azure.v1"));
	websocket_client client(config);
	client.connect(CONNECTION_2).then([]() {
		std::cout << "Connected to server." << endl;
		}).wait();

		JoinGroup(client);

		// Set player names
		board.PLAYER_1 = "Player 1";
		board.PLAYER_2 = "Player 2";

		// Start game loop
		OnlineGameLoop(board, client, "", false);
}

void testRecv() {
	websocket_client_config config;
	config.set_validate_certificates(false);
	config.add_subprotocol(utility::conversions::to_string_t("json.webpubsub.azure.v1"));
	websocket_client client(config);
	client.connect(CONNECTION_1).then([]() {
		std::cout << "Connected to server." << endl;
		}).wait();

		JoinGroup(client);

		while (true) {
			string responseString = "";
			try {
				client.receive().then([](websocket_incoming_message msg) {
					return msg.extract_string();
					}).then([&responseString](string body) {
						responseString = body;
						}).wait();
			}
			catch (websockets::client::websocket_exception ex) {
				std::cout << "Error message: " << ex.what() << endl;
				std::cout << "Error code: " << ex.error_code() << endl;
			}

			if (responseString != "") {
				std::cout << responseString << endl;
			}
		}
}

void testSend() {
	websocket_client_config config;
	config.set_validate_certificates(false);
	config.add_subprotocol(utility::conversions::to_string_t("json.webpubsub.azure.v1"));
	websocket_client client(config);
	client.connect(CONNECTION_2).then([]() {
		std::cout << "Connected to server." << endl;
		}).wait();

		JoinGroup(client);

		while (true) {
			string user_msg;
			std::cout << "Enter message to send: ";
			std::getline(cin, user_msg);
			std::cout << endl;

			try {
				json::value data = json::value::object();
				data[U("msg")] = json::value::string(utility::conversions::to_string_t(user_msg));

				json::value sendMessageRequest = json::value::object();
				sendMessageRequest[U("type")] = json::value::string(U("sendToGroup"));
				sendMessageRequest[U("group")] = json::value::string(U("group1"));
				sendMessageRequest[U("dataType")] = json::value::string(U("json"));
				sendMessageRequest[U("data")] = data;

				websocket_outgoing_message msg;
				msg.set_utf8_message(utility::conversions::to_utf8string(sendMessageRequest.serialize()));
				client.send(msg).wait();
				std::cout << "Message sent." << endl;

				string responseString;
				client.receive().then([](websocket_incoming_message msg) {
					return msg.extract_string();
					}).then([&responseString](string body) {
						responseString = body;
						}).wait();
						std::cout << responseString << endl;
			}
			catch (websockets::client::websocket_exception ex) {
				std::cout << "Error message: " << ex.what() << endl;
				std::cout << "Error code: " << ex.error_code() << endl;
			}
		}
}

int main() {
	std::cout << "Welcome to my mancala app!" << endl << endl;

	string menuError = "";

	bool exit = false;
	while (exit == false) {
		// Print menu
		std::cout << "Menu:" << endl << "------------------" << endl;
		std::cout << "(1) Host Game" << endl;
		std::cout << "(2) Join Game" << endl;
		std::cout << "(3) Play Locally" << endl;
		std::cout << "(4) Exit" << endl << endl;
		std::cout << "(5) Test Receiving" << endl;
		std::cout << "(6) Test Sending" << endl << endl;

		// If there's a menu error, print it here
		if (menuError != "") {
			std::cout << menuError << endl << endl;
			menuError = "";
		}

		// Get menu choice
		string menuChoiceString;
		std::cout << "Enter choice from menu: ";
		std::getline(cin, menuChoiceString);
		std::cout << endl;
		int menuChoice = std::stoi(menuChoiceString);

		AsciiBoard board;
		std::system("cls");
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
		case 5:
			testRecv();
			break;
		case 6:
			testSend();
			break;
		default:
			menuError = "Please enter a valid menu option.";
			break;
		}

		// Clear screen, so menu can display again
		std::system("cls");
	}
}


