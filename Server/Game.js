var Nation = require('./Nation.js');
var GameModel = require('./GameModel.js');
var ChildProcess = require('child_process');

function Game(io, socket) {
	this.io = io;
	this.socket = socket;
	this.model = new GameModel();
	console.log("hello");
	this.setupEvents();
}

Game.prototype.setupEvents = function() {
	var self = this;
	this.socket.on('move', function(moveData) {
		console.log('move received: ' + moveData); 
		self.addMove(moveData);
	});
	this.socket.on('finalize', function(finalizeData) {
		console.log('player finalized: ' + finalizeData);
		self.playerFinalized(finalizeData);
	});
	this.socket.on('unfinalize', function(finalizeData) {
		console.log('player unfinalized: ' + finalizeData);
		self.playerUnfinalized(finalizeData);
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
	this.model.playerFinalized(player);
	this.socket.emit('finalize');
	if(this.model.getIsAllFinalized()) {
		var gameState = this.model.getGameState();
		var self = this;
		this.processMoves(gameState, function(output) {
			console.log('stdout:' + output);
			console.log('-- done outputting');
			self.model.updateNewTurn(gameState.territories, gameState.units);
			self.io.emit('update', self.model.getGameState());
		});
	}
}

Game.prototype.playerUnfinalized = function(player) {
	this.model.playerUnfinalized(player);
}

Game.prototype.processMoves = function(gameState, callback) {
	console.log('about to process moves');
	/*this.model.getGameState().units["Tuscany"] = {
		type: 'fleet',
		nation: Nation.ITALY
	}*/
	var execFile = ChildProcess.execFile;
	var program = "Adjudicator/build/Release/standalone";
	var inputFile = "Adjudicator/input.txt";
	var child = execFile(program, [inputFile, gameState.moves], function(error, stdout, stderr) {
		console.log('done processing');
		console.log('error:' + error);
		console.log('stderr:' + stderr); 
		callback(stdout);
	});
}

module.exports = Game;
