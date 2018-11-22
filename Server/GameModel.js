var initialConditions = require('./InitialConditions.js');
var Phase = require('./Phase.js');

function GameModel() {
	this.loadFakeDB();
}

GameModel.prototype.loadFakeDB = function() {
	// get from database
	this.gameState = {};
}

GameModel.prototype.loadInitialConditions = function() {
	this.gameState.year = initialConditions.year;
	this.gameState.phase = initialConditions.phase;
	this.gameState.territories = initialConditions.territories;
	this.gameState.units = initialConditions.units;
	this.gameState.dislodgedUnits = {};
	this.gameState.moves = this.generateDefaultMoves(this.gameState.units, this.gameState.dislodgedUnits);
	console.log(this.gameState.moves);
	this.gameState.moveDescriptions = {};
	this.gameState.finalized = [];
}

GameModel.prototype.updateNewTurn = function(year, phase, territories, units, dislodgedUnits, moveDescriptions, next) {
	this.gameState.year = year;
	this.gameState.phase = phase;
	this.gameState.territories = territories;
	this.gameState.units = units;
	this.gameState.dislodgedUnits = dislodgedUnits;
	this.gameState.moves = this.generateDefaultMoves(units, dislodgedUnits);
	this.gameState.moveDescriptions = moveDescriptions;
	this.gameState.finalized = [];
	//updateDb as well
	console.log('updating new turn with: ');
	console.log(JSON.stringify(this.gameState, null, '    '));
	next();
}

GameModel.prototype.generateDefaultMoves = function(units, dislodgedUnits) {
	var phase = this.gameState.phase;
	var defaultMoves = {};
	if(phase === Phase.SPRING_RETREAT || phase === Phase.FALL_RETREAT) {
		Object.keys(dislodgedUnits).forEach((key) => {
			var disbandMove = {};
			disbandMove.unit = dislodgedUnits[key];
			disbandMove.moveType = 'DISBAND';
			disbandMove.secondLoc = null;
			defaultMoves[key] = disbandMove;
		});
	} else if(phase === Phase.WINTER) { // TODO
	
	} else {
		Object.keys(units).forEach((key) => {
			var holdMove = {};
			holdMove.unit = units[key];
			holdMove.moveType = 'HOLD';
			holdMove.secondLoc = null;
			holdMove.thirdLoc = null; 
			holdMove.coast = null;
			defaultMoves[key] = holdMove;
		});
	}
	return defaultMoves;
}

GameModel.prototype.addMove = function(move) {
	var firstLocation = move.firstLoc;
	delete move.firstLoc;
	this.gameState.moves[firstLocation] = move;
}

GameModel.prototype.getGameState = function() {
	return this.gameState;
}

GameModel.prototype.playerFinalized = function(nation) {
	this.gameState.finalized.push(nation);
}

GameModel.prototype.playerUnfinalized = function(nation) {
	for(var i = 0; i < this.gameState.finalized.length - 1; i++){ 
	   if(this.gameState.finalized[i] === nation) {
		 this.gameState.finalized.splice(i, 1); 
		 break;
	   }
	}
}

// TODO
GameModel.prototype.getIsAllFinalized = function() {
	return this.gameState.finalized.length > 0;
}

module.exports = GameModel;
