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
	this.units = gameState.units;
	this.territories = gameState.territories;
	this.moves = gameState.moves;
	this.isFinalized = gameState.finalized.length > 0;
	this.presenter.update(this.territories, this.units, this.isFinalized);
}

/*MapModel.prototype.loadGameState = function() {
	console.log('game state loading');
	
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

MapModel.prototype.getUnitAt = function(territory) {
	if(territory in this.units) {
		return this.units[territory];
	} else {
		return null;
	}
}

MapModel.prototype.createNewMove = function(unit, moveType, firstLocation, secondLocation, thirdLocation) {
	// send to server
	var s = 'move: ' + firstLocation + " " + moveType + " " + secondLocation + " " + thirdLocation;
	console.log(s);
	
	this.moveCreatedHandler(s);
}

MapModel.prototype.toggleIsFinalized = function() {
	this.isFinalized = !this.isFinalized;
	this.presenter.updateIsFinalized(this.isFinalized);
	this.finalizeHandler(this.isFinalized);
}

MapModel.prototype.getIsFinalized = function() {
	return this.isFinalized;
}