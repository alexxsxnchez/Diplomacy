var initialConditions = require('./InitialConditions.js');
var Phase = require('./Phase.js');
var Nation = require('./Nation.js');

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
	this.gameState.territories = JSON.parse(JSON.stringify(initialConditions.territories));
	this.gameState.units = JSON.parse(JSON.stringify(initialConditions.units));
	this.gameState.dislodgedUnits = {};
	this.gameState.moves = {};//this.generateDefaultMoves(this.gameState.units, this.gameState.dislodgedUnits);
	this.gameState.moveDescriptions = {};
	this.gameState.finalized = [];
}

GameModel.prototype.updateNewTurn = function(year, phase, territories, units, dislodgedUnits, moveDescriptions, next) {
	this.gameState.year = year;
	this.gameState.phase = phase;
	this.gameState.territories = territories;
	this.gameState.units = units;
	this.gameState.dislodgedUnits = dislodgedUnits;
	this.gameState.moves = {};//this.generateDefaultMoves(units, dislodgedUnits);
	this.gameState.moveDescriptions = moveDescriptions;
	this.gameState.finalized = [];
	//updateDb as well
	console.log('updating new turn with: ');
	console.log(JSON.stringify(this.gameState, null, '    '));
	next();
}

GameModel.prototype.fillInDefaultMoves = function() {
	var units = this.gameState.units;
	var dislodgedUnits = this.gameState.dislodgedUnits;
	var phase = this.gameState.phase;
	//var defaultMoves = {};
	if(phase === Phase.SPRING_RETREAT || phase === Phase.FALL_RETREAT) {
		Object.keys(dislodgedUnits).forEach((key) => {
			if(key in this.gameState.moves) {
				return;
			}
			var disbandMove = {};
			disbandMove.unit = dislodgedUnits[key];
			disbandMove.moveType = 'DISBAND';
			disbandMove.firstLoc = key;
			disbandMove.secondLoc = null;
			this.gameState.moves[key] = disbandMove;
		});
	} else if(phase === Phase.WINTER) { // TODO
	
	} else {
		Object.keys(units).forEach((key) => {
			if(key in this.gameState.moves) {
				return;
			}
			var holdMove = {};
			holdMove.unit = units[key];
			holdMove.moveType = 'HOLD';
			holdMove.firstLoc = key;
			holdMove.secondLoc = null;
			holdMove.thirdLoc = null; 
			holdMove.coast = null;
			this.gameState.moves[key] = holdMove;
		});
	}
}

GameModel.prototype.addMove = function(move) {
	var firstLocation = move.firstLoc;
	//delete move.firstLoc;
	this.gameState.moves[firstLocation] = move;
}

GameModel.prototype.deleteMove = function(location) {
	/*var defaultMove = {};
	var phase = this.gameState.phase;
	if(phase === Phase.SPRING_RETREAT || phase === Phase.FALL_RETREAT) {
		defaultMove.unit = this.gameState.dislodgedUnits[location];
		defaultMove.moveType = 'DISBAND';
	} else if(phase === Phase.WINTER) {
		delete this.gameState.moves[location];
		return;
	} else {
		defaultMove.unit = this.gameState.units[location];
		defaultMove.moveType = 'HOLD';
	}
	defaultMove.secondLoc = null;
	defaultMove.thirdLoc = null; 
	defaultMove.coast = null;
	this.gameState.moves[location] = defaultMove;*/
	delete this.gameState.moves[location];
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

GameModel.prototype.getHomeCentres = function(nation) {
	switch(nation) {
		case Nation.AUSTRIA:
			return ['Trieste', 'Budapest', 'Vienna'];
		case Nation.ENGLAND:
			return ['Liverpool', 'Edinburgh', 'London'];
		case Nation.FRANCE:
			return ['Marseilles', 'Paris', 'Brest'];
		case Nation.GERMANY:
			return ['Munich', 'Kiel', 'Berlin'];
		case Nation.ITALY:
			return ['Naples', 'Rome', 'Venice'];
		case Nation.RUSSIA:
			return ['StPetersburg', 'Sevastopol', 'Moscow', 'Warsaw'];
		case Nation.TURKEY:
			return ['Smyrna', 'Constantinople', 'Ankara'];
		default:
			return [];
	}
}

module.exports = GameModel;
