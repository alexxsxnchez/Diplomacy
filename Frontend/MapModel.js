function MapModel(presenter) {
	this.presenter = presenter;
	this.interactor = interactor;
	//this.loadGameState();
	//this.interactor.addLoadGameStateHandler(this.loadGameState.bind(this));
}

MapModel.prototype.addMoveCreatedHandler = function(handler) {
	this.moveCreatedHandler = handler;
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
	this.presenter.update(this.year, this.phase, this.territories, this.units, this.dislodgedUnits, this.isFinalized);
}

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
		/*,
		'Prussia': {
			type: 'army',
			nation: Nation.GERMANY
		},
		'Silesia': {
			type: 'army',
			nation: Nation.GERMANY
		},
		'Ruhr': {
			type: 'army',
			nation: Nation.GERMANY
		},
		'Wales': {
			type: 'army',
			nation: Nation.ENGLAND
		},
		'Yorkshire': {
			type: 'fleet',
			nation: Nation.ENGLAND
		},
		'Clyde': {
			type: 'army',
			nation: Nation.ENGLAND
		},
		'Burgundy': {
			type: 'army',
			nation: Nation.FRANCE
		},
		'Gascony': {
			type: 'fleet',
			nation: Nation.FRANCE
		},
		'Picardy': {
			type: 'army',
			nation: Nation.FRANCE
		},
		'Piedmont': {
			type: 'fleet',
			nation: Nation.ITALY
		},
		'Tuscany': {
			type: 'army',
			nation: Nation.ITALY
		},
		'Apulia': {
			type: 'army',
			nation: Nation.ITALY
		},
		'Tyrolia': {
			type: 'army',
			nation: Nation.AUSTRIA
		},
		'Bohemia': {
			type: 'army',
			nation: Nation.AUSTRIA
		},
		'Galicia': {
			type: 'army',
			nation: Nation.AUSTRIA
		},
		'Armenia': {
			type: 'fleet',
			nation: Nation.TURKEY
		},
		'Syria': {
			type: 'army',
			nation: Nation.TURKEY
		},
		'Rumania': {
			type: 'army',
			nation: Nation.TURKEY
		},
		'Ukraine': {
			type: 'army',
			nation: Nation.RUSSIA
		},
		'Livonia': {
			type: 'fleet',
			nation: Nation.RUSSIA
		},
		'Serbia': {
			type: 'army',
			nation: Nation.RUSSIA
		},
		'Finland': {
			type: 'army',
			nation: Nation.RUSSIA
		},
		'Albania': {
			type: 'fleet',
			nation: Nation.AUSTRIA
		},
		'Greece': {
			type: 'fleet',
			nation: Nation.AUSTRIA
		},
		'Bulgaria': {
			type: 'army',
			nation: Nation.TURKEY,
		},
		'Tunis': {
			type: 'army',
			nation: Nation.TURKEY
		},
		'NorthAfrica': {
			type: 'army',
			nation: Nation.TURKEY
		},
		'Portugal': {
			type: 'fleet',
			nation: Nation.RUSSIA
		},
		'Belgium': {
			type: 'fleet',
			nation: Nation.RUSSIA
		},
		'Holland': {
			type: 'fleet',
			nation: Nation.RUSSIA
		},
		'Norway': {
			type: 'army',
			nation: Nation.RUSSIA
		},
		'Sweden': {
			type: 'army',
			nation: Nation.RUSSIA
		},
		'Spain': {
			type: 'army',
			nation: Nation.RUSSIA
		},
		'Denmark': {
			type: 'army',
			nation: Nation.RUSSIA
		}
	}
	
	this.moves = [];
	this.presenter.update(this.territories, this.units);
}*/

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

MapModel.prototype.toggleIsFinalized = function() {
	this.isFinalized = !this.isFinalized;
	this.presenter.updateIsFinalized(this.isFinalized);
	this.finalizeHandler(this.isFinalized);
}

MapModel.prototype.getIsFinalized = function() {
	return this.isFinalized;
}
