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
		this.processMoves(gameState, function(territories, units, dislodgedUnits, moveDescriptions) {
			self.model.updateNewTurn(territories, units, dislodgedUnits, moveDescriptions);
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
		self.prepareNewGameState(gameState, results, function(territories, units, dislodgedUnits, moveDescriptions) {
			callback(territories, units, dislodgedUnits, moveDescriptions);
		});
	});
}

Game.prototype.prepareNewGameState = function(gameState, results, callback) {
	var territories = gameState.territories;
	var units = {};
	var dislodged = {};
	var descriptions = {};
	Object.keys(gameState.moves).forEach((key) => {
		console.log(key);
		// get descriptions
		descriptions[key] = results[key].description;
		
		var unit = gameState.moves[key].unit;
		// create dislodge list
		if(results[key].dislodged) {
			dislodged[key] = {};
			dislodged[key].unit = unit;
			dislodged[key].retreatOptions = results[key].retreatOptions;
			return;
		}
		// for all units not dislodged
		if(gameState.moves[key].moveType === 'MOVE' && results[key].success) {
			var newLocation = gameState.moves[key].secondLoc;
			units[newLocation] = unit;
		} else {
			units[key] = gameState.units[key];
		}
	});
	
	Object.keys(units).forEach((key) => {
		territories[key] = units[key].nation;
	});
	
	callback(territories, units, dislodged, descriptions);
}

module.exports = Game;
