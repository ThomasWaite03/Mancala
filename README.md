# Mancala
A mancala game built using C++.

## How to play
After compiling the project you can run the executable to play the game. Some important details about the interface for playing are listed below. The rules of the game are for a standard 12 pocket game board.

### Game Rules:
https://www.officialgamerules.org/mancala

### Interface
- Player 1 uses the bottom row and player 2 uses the top row.
- On each player's turn, they enter a number corresponding to the column number listed above the pocket they want to select.
- Each column number will align with a pocket on each row, but the program will only select the pocket on the current player's side.

## Setup
This project includes a powershell script that will download and install the necessary dependencies. Client urls for establishing a websocket connection must also be added at the top of the Mancala.cpp file. This project was built for use with Azure's Web PubSub service and the client urls can be generated on the Azure portal.