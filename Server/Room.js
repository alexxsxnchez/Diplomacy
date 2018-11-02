var Game = require('./Game.js');

function Room(io, socket, id) {
	this.io = io;
	this.socket = socket;
	this.id = id;
	this.loadGame(id);
}


Room.prototype.loadGame = function(id) {
	// get game from database
	var isNewGame = true;
	if(isNewGame) {
		var game = new Game(this.io, this.socket);
		game.start();
	}
};


module.exports = Room;
