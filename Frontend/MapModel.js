function MapModel(presenter) {
	this.presenter = presenter;
}

MapModel.prototype.addMoveCreatedHandler = function(handler) {
	this.moveCreatedHandler = handler;
}

MapModel.prototype.addMoveDeletedHandler = function(handler) {
	this.moveDeletedHandler = handler;
}

MapModel.prototype.addFinalizeHandler = function(handler) {
	this.finalizeHandler = handler;
}

MapModel.prototype.dataReceived = function(gameState) {
	this.year = gameState.year;
	this.phase = gameState.phase;
	this.territories = gameState.territories;
	console.log(gameState.territories);
	this.units = gameState.units;
	this.dislodgedUnits = gameState.dislodgedUnits;
	this.moves = gameState.moves;
	this.moveDescriptions = gameState.moveDescriptions;
	this.isFinalized = gameState.finalized.length > 0;
	this.presenter.update(this.year, this.phase, this.territories, this.units, this.dislodgedUnits, this.isFinalized, this.moves);
}

// Should be validated by servers
MapModel.prototype.getHomeCentres = function(nation) {
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

// Should be validated by server
MapModel.prototype.getBuildOptions = function(territory) {
	var options = [];
	switch(territory) {
		case 'StPetersburg':
			options.push(MoveType.BUILDFLEETSC);
			options.push(MoveType.BUILDFLEETNC);
			options.push(MoveType.BUILDARMY);
			break;
		case 'Marseilles':
		case 'Brest':
		case 'Liverpool':
		case 'London':
		case 'Edinburgh':
		case 'Kiel':
		case 'Berlin':
		case 'Sevastopol':
		case 'Ankara':
		case 'Constantinople':
		case 'Smyrna':
		case 'Trieste':
		case 'Venice':
		case 'Naples':
		case 'Rome':
			options.push(MoveType.BUILDFLEET);
		case 'Paris':
		case 'Munich':
		case 'Warsaw':
		case 'Moscow':
		case 'Vienna':
		case 'Budapest':
			options.push(MoveType.BUILDARMY);
	}
	return options;
}

MapModel.prototype.getDislodgedUnitAt = function(territory) {
	if(territory in this.dislodgedUnits) {
		return this.dislodgedUnits[territory].unit;
	} else {
		return null;
	}
}

MapModel.prototype.getUnitAt = function(territory) {
	if(territory in this.units) {
		return this.units[territory];
	} else {
		return null;
	}
}

MapModel.prototype.getStarToUnitCount = function(nation) {
	var starCount = Object.keys(this.territories).reduce((count, territory) => {
		if(this.territories[territory].hasStar && this.territories[territory].nation === nation) {
			return count + 1;
		} else {
			return count;
		}
	}, 0);

	var unitCount = Object.keys(this.units).reduce((count, unit) => {
		if(this.units[unit].nation === nation) {
			return count + 1;
		} else {
			return count;
		}
	}, 0);
	return starCount - unitCount;
}

MapModel.prototype.getPhase = function() {
	return this.phase;
}

// Should be validated by server
MapModel.prototype.createNewMove = function(unit, moveType, firstLocation, secondLocation, thirdLocation, coast, viaConvoy) {
	// send to server
	var s = 'move: ' + firstLocation + " " + moveType + " " + secondLocation + " " + thirdLocation + " " + coast + " withConvoy: " + viaConvoy;
	console.log(s);
	var move = {};
	move.unit = unit;
	move.moveType = moveType;
	move.firstLoc = firstLocation;
	move.secondLoc = secondLocation;
	move.thirdLoc = thirdLocation;
	move.coast = coast;
	move.viaConvoy = viaConvoy;
	this.moveCreatedHandler(move);
	this.presenter.updateMoves(move, this.territories);
}

MapModel.prototype.deleteMove = function(location) {
	this.moveDeletedHandler(location);
	if(this.phase != "WINTER") {
		if(this.phase == "SPRING" || this.phase == "FALL") {
			this.presenter.resetMove(this.units, location);
		} else {
			this.presenter.resetMove(this.dislodgedUnits, location, true);
		}
	}
}

MapModel.prototype.toggleIsFinalized = function() {
	this.isFinalized = !this.isFinalized;
	this.presenter.updateIsFinalized(this.isFinalized);
	this.finalizeHandler(this.isFinalized);
}

MapModel.prototype.getIsFinalized = function() {
	return this.isFinalized;
}
