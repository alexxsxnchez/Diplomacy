var Game = require('./Game.js');

function GameManager() {
	this.gameList = {};
	this.loadedGameList = false;
}

GameManager.prototype.loadDefaultGameList = function(io, socket) {
	if(this.loadedGameList) {
		this.gameList[0].socket = socket;
		this.gameList[0].setupEvents();
		return;
	}
	var game = new Game(io, socket);
	game.start();
	this.gameList[0] = game;
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