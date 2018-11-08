var Nation = require('./Nation.js');
var Phase = require('./Phase.js');
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
		var self = this;
		this.processTurn(function() {
			self.io.emit('update', self.model.getGameState());
		});
	}
}

Game.prototype.playerUnfinalized = function(player) {
	this.model.playerUnfinalized(player);
}

Game.prototype.processTurn = function(next) {
	var nextPhase;
	switch(this.model.getGameState().phase) {
		case Phase.SPRING_RETREAT:
		case Phase.FALL_RETREAT:
			var results = this.processRetreatMoves();
			this.prepareNewGameState(results, function() {
				next();
			});
			break;
		case Phase.SPRING:
		case Phase.FALL:
			var self = this;
			this.processAdjudicationMoves(function(results) {
				self.prepareNewGameState(results, function() {
					next();
				});
			});
			break;
		case Phase.WINTER:
			break;
	}
}

Game.prototype.processRetreatMoves = function() {
	var results = {};
	var moves = this.model.getGameState().moves;
	var alreadyDeclared = {};
	
	Object.keys(moves).forEach((key) => {
		results[key].success = true;
		results[key].description = '';
		results[key].dislodged = false;
		results[key].retreatOptions = [];
	
		if(moves[key].moveType === 'RETREAT') {
			var plannedDestination = moves[key].secondLoc;
			if(plannedDestination in Object.keys(alreadyDeclared)) {
				var description = 'Two or more units attempted to retreat to the same location.';
				results[key].success = false;
				results[key].description = description;
				var otherRetreater = alreadyDeclared[plannedDestination];
				results[otherRetreater].success = false;
				results[otherRetreater].description = description;
				return;
			}
			alreadyDeclared[plannedDestination] = key;
		} else {
			// do nothing for DISBAND
		}
	});
	return results;
}

Game.prototype.processAdjudicationMoves = function(next) {
	console.log('about to process moves');
	var execFile = ChildProcess.execFile;
	var program = "Adjudicator/build/Release/standalone";
	var inputFile = "Adjudicator/input.txt";
	var moves = this.model.getGameState().moves;
	var self = this;
	var child = execFile(program, [inputFile, JSON.stringify(moves)], function(error, stdout, stderr) {
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
		next(results);
	});
}

Game.prototype.prepareNewGameState = function(results, next) {
	var gameState = this.model.getGameState();
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
		if((gameState.moves[key].moveType === 'MOVE' || gameState.moves[key].moveType === 'RETREAT') && results[key].success) {
			var newLocation = gameState.moves[key].secondLoc;
			units[newLocation] = unit;
		} else {
			units[key] = gameState.units[key];
		}
	});
	
	// update territories
	var territories = gameState.territories;
	Object.keys(units).forEach((key) => {
		territories[key] = units[key].nation;
	});
	
	// update phase && year
	var year = gameState.year + 1;
	var phase = gameState.phase;
	switch(phase) {
		case Phase.SPRING:
			phase = Phase.SPRING_RETREAT;
			break;
		case Phase.SPRING_RETREAT:
			phase = Phase.FALL;
			break;
		case Phase.FALL:
			phase = Phase.SPRING_RETREAT;
			break;
		case Phase.FALL_RETREAT:
			this.checkIfGameOver(); // todo
			phase = Phase.WINTER;
			break;
		case Phase.WINTER:
			phase = Phase.SPRING;
	}
	
	this.model.updateNewTurn(year, phase, territories, units, dislodged, descriptions, function() {
		next();
	});
}

Game.prototype.checkIfGameOver = function() {
	// TODO

}

module.exports = Game;
