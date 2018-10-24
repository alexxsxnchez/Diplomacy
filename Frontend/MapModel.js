function MapModel(presenter) {
	this.presenter = presenter;
	this.loadGameState();
}

MapModel.prototype.loadGameState = function() {
	// get from server
	this.units = {
		'Kiel': 'Fleet',
		'Berlin': 'Army',
		'Munich': 'Army'
	}
	this.territories = {
		'Kiel': Nation.GERMANY,
		'Berlin': Nation.GERMANY
	}
	this.moves = [];
}

MapModel.prototype.getUnitAt = function(territory) {
	if(territory in this.units) {
		return this.units[territory];
	} else {
		return null;
	}
}

MapModel.prototype.createNewMove = function(unit, moveType, firstLocation, secondLocation, thirdLocation) {
	// send to server
	console.log('move: ' + firstLocation + " " + moveType + " " + secondLocation + " " + thirdLocation);
}