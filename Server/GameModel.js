var initialConditions = require('./initialConditions');

function GameModel() {
	this.loadFakeDB();
}

GameModel.prototype.loadFakeDB = function() {
	// get from database
	this.finalized = [];
	this.gameState = {};
}

GameModel.prototype.loadInitialConditions = function() {
	this.gameState.territories = initialConditions.territories;
	this.gameState.units = initialConditions.units;
	this.gameState.moves = [];
}

GameModel.prototype.updateNewTurn = function(territories, units) {
	this.gameState.territories = territories;
	this.gameState.units = units;
	//updateDb
}

GameModel.prototype.addMove = function(move) {
	this.gameState.moves.push(move);
}

GameModel.prototype.getGameState = function() {
	//console.log(this.gameState.territories);
	return this.gameState;
}

GameModel.prototype.playerFinalized = function(nation) {
	finalized.push(nation);
}

GameModel.prototype.playerUnfinalized = function(nation) {
	for(var i = 0; i < this.finalized.length - 1; i++){ 
	   if ( this.finalized[i] === nation) {
		 this.finalized.splice(i, 1); 
		 break;
	   }
	}
}

// TODO
GameModel.prototype.getIsAllFinalized = function() {
	return this.finalized.length > 0;
}

module.exports = GameModel;
