var initialConditions = require('./initialConditions');

function GameModel() {
	this.loadFakeDB();
}

GameModel.prototype.loadFakeDB = function() {
	// get from database
	this.gameState = {};
}

GameModel.prototype.loadInitialConditions = function() {
	this.gameState.territories = initialConditions.territories;
	this.gameState.units = initialConditions.units;
	this.gameState.dislodgedUnits = {};
	this.gameState.moves = this.generateDefaultMoves(this.gameState.units);
	console.log(this.gameState.moves);
	this.gameState.moveDescriptions = {};
	this.gameState.finalized = [];
}

GameModel.prototype.updateNewTurn = function(territories, units, dislodgedUnits, moveDescriptions) {
	this.gameState.territories = territories;
	this.gameState.units = units;
	this.gameState.dislodgedUnits = dislodgedUnits;
	this.gameState.moves = this.generateDefaultMoves(units);
	this.gameState.moveDescriptions = moveDescriptions;
	this.gameState.finalized = [];
	//updateDb
}

GameModel.prototype.generateDefaultMoves = function(units) {
	var defaultMoves = {};
	Object.keys(units).forEach((key) => {
		var holdMove = {};
		holdMove.unit = units[key];
		holdMove.moveType = 'HOLD';
		holdMove.secondLoc = null;
		holdMove.thirdLoc = null; 
		defaultMoves[key] = holdMove;
	});
	return defaultMoves;
}

GameModel.prototype.addMove = function(move) {
	var firstLocation = move.firstLoc;
	delete move.firstLoc;
	this.gameState.moves[firstLocation] = move;
}

GameModel.prototype.getGameState = function() {
	//console.log(this.gameState.territories);
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
