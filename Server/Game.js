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
			var results = this.processWinterMoves();
			this.prepareNewGameState(results, function() {
				next();
			});
			break;
	}
}

Game.prototype.processRetreatMoves = function() {
	var results = {};
	var moves = this.model.getGameState().moves;
	var dislodged = this.model.getGameState().dislodgedUnits;
	var alreadyDeclared = {};
	
	Object.keys(moves).forEach((key) => {
		results[key] = {};
		results[key].success = true;
		results[key].description = '';
		results[key].dislodged = false;
		results[key].retreatOptions = [];
	
		if(moves[key].moveType === 'RETREAT') {
			var plannedDestination = moves[key].secondLoc;
			if(!dislodged[key].retreatOptions.includes(plannedDestination)) {
				results[key].success = false;
				results[key].description = "Illegal move. Destination is not a location this unit can retreat to.";
				return;
			}
			
			if(plannedDestination in Object.keys(alreadyDeclared)) {
				var description = 'Two or more units attempted to retreat to the same location.';
				results[key].success = false;
				results[key].description = description;
				console.log(description);
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

Game.prototype.processWinterMoves = function(next) {
	var results = {};
	var gameState = this.model.getGameState();
	var counts = this.calculateStarAndUnitCount();
	var differences = {};
	
	Object.keys(counts).forEach((nation) => {
		var diff = counts[nation].stars - counts[nation].units;
		differences[nation] = diff;
	});
	
	Object.keys(gameState.moves).forEach((key) => {
		var move = gameState.moves[key];
		var nation = gameState.territories[key].nation;
		var diff = differences[nation];
		
		results[key].success = true;
		results[key].description = 'Successful';
		
		if(diff < 0) {
			if(move.moveType !== 'DESTROY') {
				results[key].success = false;
				results[key].description = 'Illegal build.';
			} else {
				differences[nation]++;
			}
		} else if(diff > 0) {
			if(move.moveType === 'DESTROY') {
				results[key].success = false;
				results[key].description = 'Can\'t disband!';
			} else {
				differences[nation]--;
			}
		} else {
			results[key].success = false;
			if(move.moveType === 'DESTROY') {
				results[key].description = 'Too many disbands!';
			} else {
				results[key].description = 'Too many builds!';
			}
		}
	});
	
	// check if user did not enter enough disbands
	Object.keys(differences).forEach((nation) => {
		var diff = differences[nation];
		if(diff < 0) {
			// must auto delete
			
		}
	}); 
	return results;
}

Game.prototype.prepareNewGameState = function(results, next) {
	var gameState = this.model.getGameState();
	var units = {};
	if(gameState.phase !== "SPRING" && gameState.phase !== "FALL") {
		units = gameState.units;
	}
	var dislodged = {};
	var descriptions = {};
	
	Object.keys(results).forEach((key) => {
		// prepare descriptions
		descriptions[key] = results[key].description;
		
		var unit = gameState.moves[key].unit;
		// prepare dislodge list
		if(results[key].dislodged) {
			dislodged[key] = {};
			dislodged[key].unit = unit;
			dislodged[key].retreatOptions = results[key].retreatOptions;
			return;
		}
		// prepare units that have moves
		var moveType = gameState.moves[key].moveType
		if((moveType === 'MOVE' || moveType === 'RETREAT') && results[key].success) {
			var newLocation = gameState.moves[key].secondLoc;
			units[newLocation] = unit;
		} else if(moveType === 'BUILD' || moveType === 'DESTROY') {
		
		
		} else {
			units[key] = gameState.units[key];
		}
	});
	
	// update territories
	var territories = gameState.territories;
	console.log(territories);
	Object.keys(units).forEach((key) => {
		if(Object.keys(territories).includes(key)) {
			territories[key].nation = units[key].nation;
		}
	});
	
	// update phase && year
	var yearAndPhase = this.decideNextPhase(gameState.year, gameState.phase, dislodged);
	var year = yearAndPhase.year;
	var phase = yearAndPhase.phase;
	this.model.updateNewTurn(year, phase, territories, units, dislodged, descriptions, function() {
		next();
	});
}

Game.prototype.decideNextPhase = function(year, phase, dislodgedUnits) {
	switch(phase) {
		case Phase.SPRING:
			if(Object.keys(dislodgedUnits).length === 0) {
				phase = Phase.FALL;
			} else {
				phase = Phase.SPRING_RETREAT;
			}
			break;
		case Phase.SPRING_RETREAT:
			phase = Phase.FALL;
			break;
		case Phase.FALL:
			if(Object.keys(dislodgedUnits).length === 0) {
				this.checkIfGameOver();
				// if there are builds to be made
				var counts = this.calculateStarAndUnitCount();
				/*Object.keys(counts).reduce((nation) => {
					if(
				}
				*/
				
				phase = Phase.WINTER;
			} else {
				phase = Phase.FALL_RETREAT;
			}
			break;
		case Phase.FALL_RETREAT:
			this.checkIfGameOver();
			// if there are builds to be made
			phase = Phase.WINTER;
			break;
		case Phase.WINTER:
			phase = Phase.SPRING;
			year++;
	}
	
	var returnObj = {};
	returnObj.year = year;
	returnObj.phase = phase;
	return returnObj;
}

Game.prototype.checkIfGameOver = function() {
	var starCounts = this.calculateStarAndUnitCount();
	Object.keys(starCounts).forEach((nation) => {
		var count = starCounts[nation].stars;
		if(count >= 18) {
			//TODO
			console.log(nation + ' has won with ' + count + ' supply centers!!');
		}
	});
}

Game.prototype.calculateStarAndUnitCount = function() {
	var counts = {
		"AUSTRIA": {
			stars: 0,
			units: 0
		},
		"ENGLAND": {
			stars: 0,
			units: 0
		},
		"FRANCE": {
			stars: 0,
			units: 0
		},
		"GERMANY": {
			stars: 0,
			units: 0
		},
		"ITALY": {
			stars: 0,
			units: 0
		},
		"RUSSIA": {
			stars: 0,
			units: 0
		},
		"TURKEY": {
			stars: 0,
			units: 0
		},
		"NEUTRAL": {
			stars: 0,
			units: 0
		},
	}
	var territories = this.model.getGameState().territories;
	Object.keys(territories).forEach((key) => {
		if(territories[key].hasStar) {
			var nation = territories[key].nation;
			counts[nation].stars++;
		}
	});
	var units = this.model.getGameState().units;
	Object.keys(units).forEach((key) => {
		var nation = units[key].nation;
		counts[nation].units++;
	});
	return counts;
}

module.exports = Game;
