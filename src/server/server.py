#!/usr/bin/env python

import asyncio
from asyncio.windows_events import NULL
import websockets
import json

class Player:
  def __init__(self, websocket, name, gameID):
    self.websocket = websocket
    self.name = name
    self.gameID = gameID

class Game:
    def __init__(self, playerOne, playerTwo):
        self.playerOne = playerOne
        self.playerTwo = playerTwo

hosts = dict()
games = dict()

async def handler(websocket):
    while True:
        message = None
        try:
            message = await websocket.recv()
        except websockets.ConnectionClosedOK:
            print("Connection Closed OK")
            break
        except websockets.ConnectionClosed:
            print("Connection Closed")
            break
        
        if message is not None:
            jsonMessage = json.loads(message)
            name = jsonMessage["name"]
            if jsonMessage["status"] == "hosting":
                hosts[name] = Player(websocket, name, jsonMessage["gameID"])
            elif jsonMessage["status"] == "joining" and "hostName" in jsonMessage:
                host = hosts[jsonMessage["hostName"]]
                joiningPlayer = Player(websocket, jsonMessage["name"], host.gameID)
                games[host.gameID] = Game(host, joiningPlayer)
                del hosts[jsonMessage["hostName"]]

                joinedMessage = {
                    "gameID": host.gameID,
                    "hostName": host.name,
                    "joiningName": joiningPlayer.name
                }
                await host.websocket.send(json.dumps(joinedMessage))
                await websocket.send(json.dumps(joinedMessage))
            elif jsonMessage["status"] == "joining":
                await websocket.send(json.dumps({"hosts": list(hosts.keys())}))
            elif jsonMessage["status"] == "makingMove":
                game = games[jsonMessage["gameID"]]
                move = {
                    "move": jsonMessage["move"]
                }
                print('made it to makingMove!')
                if game.playerOne.name == jsonMessage["name"]:
                    print('sending move to player 2...')
                    print(game.playerTwo.websocket)
                    await game.playerTwo.websocket.send(json.dumps(move))
                else:
                    print('sending move to player 1...')
                    print(game.playerOne.websocket)
                    await game.playerOne.websocket.send(json.dumps(move))
        # websockets.broadcast(connected, message)
        print(f"Message received: {message}")


async def main():
    async with websockets.serve(handler, "", 8080):
        await asyncio.Future()  # run forever


if __name__ == "__main__":
    asyncio.run(main())