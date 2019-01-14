var Game = require('./Game.js');

function GameManager(io) {
	this.gameList = [];
	this.loadedGameList = false;
	this.loadDefaultGameList(io);
}

GameManager.prototype.addGameRequestListener = function(socket) {
	var self = this;
	socket.on('requestgamedata', function(gameId, userId) {
		if(userId === undefined || gameId === null) {
			return;
		}
		if(gameId !== undefined && gameId !== null && !isNaN(gameId) && gameId >= 0 && gameId < self.gameList.length) {
			self.gameList[gameId].addUserSocketToRoom(socket, userId);
		}
	});
}

GameManager.prototype.loadDefaultGameList = function(io) {
	
	/*if(this.loadedGameList) {
		this.gameList[0].socket = socket;
		this.gameList[0].setupEvents();
		return;
	}*/
	var game0 = new Game(io, 0);
	game0.start();
	this.gameList[0] = game0;
	var game1 = new Game(io, 1);
	game1.start();
	this.gameList[1] = game1;
	this.loadedGameList = true;
}

GameManager.prototype.getGameWithId = function(id) {
	if(this.gameList[id] == undefined) {
		return null;
	} else {
		return this.gameList[id];
	}
}

module.exports = GameManager;