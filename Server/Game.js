var GameModel = require('./GameModel');

function Game(io, socket) {
	this.io = io;
	this.socket = socket;
	this.model = new GameModel();
	console.log("hello");
//	this.setupEvents();
}

Game.prototype.setupEvents = function() {
	this.socket.on('move', function(moveData) {
		console.log('move received: ' + moveData); 
		this.addMove(moveData);
	});
	this.socket.on('finalize', function(finalizeData) {
		this.playerFinalized(finalizeData);
	});
	this.socket.on('unfinalize', function(finalizeData) {
		this.playerUnfinalized(finalizeData);
	});
}

Game.prototype.start = function(move) {
	this.model.loadInitialConditions();
	this.io.emit('update', this.model.getGameState());
}

Game.prototype.addMove = function(move) {
	this.model.addMove(move);
}

Game.prototype.playerFinalized = function(player) {
	this.model.addFinalized(player);
	this.socket.emit('finalize');
	if(this.model.getIsAllFinalized()) {
		this.processMoves();
		this.io.emit('update', this.model.getGameState());
	}
}

Game.prototype.playerUnfinalized = function(player) {
	this.model.removeFinalized(player);
}

Game.prototype.processMoves = function(callback) {
	console.log('about to process moves');
}

module.exports = Game;
