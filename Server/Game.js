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
		this.processMoves(gameState, function(territories, units) {
			self.model.updateNewTurn(territories, units);
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
	var self = this;
	var child = execFile(program, [inputFile, JSON.stringify(gameState.moves)], function(error, stdout, stderr) {
		console.log('done processing');
		console.log('error:' + error);
		console.log('stderr:' + stderr);
		console.log('stdout:' + stdout);
		console.log('-- done outputting');
		var results = {};
		try {
			results = JSON.parse(stdout);
		} catch(error) {
			console.log(error);
		}
		self.prepareNewGameState(gameState, results, function(territories, units) {
			callback(territories, units);
		});
	});
}

Game.prototype.prepareNewGameState = function(gameState, results, callback) {
	var territories = gameState.territories;
	var units = {};
	Object.keys(gameState.moves).forEach((key) => {
		if(gameState.moves[key].moveType === 'MOVE') {
			if(results[key].success) {
				var unit = gameState.moves[key].unit;
				var newLocation = gameState.moves[key].secondLoc;
				units[newLocation] = unit;
			}
		}
	});
	
	Object.keys(units).forEach((key) => {
		territories[key] = units[key].nation;
	});
	
	callback(territories, units);
}

module.exports = Game;
