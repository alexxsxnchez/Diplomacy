function MapModel(presenter) {
	this.presenter = presenter;
	this.loadGameState();
}

MapModel.prototype.loadGameState = function() {
	// get from server
	this.units = {
		'Kiel': {
			type: 'fleet',
			nation: Nation.GERMANY
		},
		'Berlin': {
			type: 'army',
			nation: Nation.GERMANY
		},
		'Munich': {
			type: 'army',
			nation: Nation.GERMANY
		},
		'London': {
			type: 'fleet',
			nation: Nation.ENGLAND
		},
		'Liverpool': {
			type: 'army',
			nation: Nation.ENGLAND
		},
		'Edinburgh': {
			type: 'fleet',
			nation: Nation.ENGLAND
		},
		'Paris': {
			type: 'army',
			nation: Nation.FRANCE
		},
		'Marseilles': {
			type: 'army',
			nation: Nation.FRANCE
		},
		'Brest': {
			type: 'fleet',
			nation: Nation.FRANCE
		},
		'Venice': {
			type: 'army',
			nation: Nation.ITALY
		},
		'Rome': {
			type: 'army',
			nation: Nation.ITALY
		},
		'Naples': {
			type: 'fleet',
			nation: Nation.ITALY
		},
		'Trieste': {
			type: 'fleet',
			nation: Nation.AUSTRIA
		},
		'Vienna': {
			type: 'army',
			nation: Nation.AUSTRIA
		},
		'Budapest': {
			type: 'army',
			nation: Nation.AUSTRIA
		},
		'Constantinople': {
			type: 'army',
			nation: Nation.TURKEY
		},
		'Smyrna': {
			type: 'army',
			nation: Nation.TURKEY
		},
		'Ankara': {
			type: 'fleet',
			nation: Nation.TURKEY
		},
		'Sevastopol': {
			type: 'fleet',
			nation: Nation.RUSSIA
		},
		'Moscow': {
			type: 'army',
			nation: Nation.RUSSIA
		},
		'Warsaw': {
			type: 'army',
			nation: Nation.RUSSIA
		},
		'StPetersburg': {
			type: 'fleet',
			nation: Nation.RUSSIA,
			coast: 'SC'
		}
	}
	this.territories = {
		'Kiel': Nation.GERMANY,
		'Berlin': Nation.GERMANY
	}
	this.moves = [];
	this.presenter.update(this.units);
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