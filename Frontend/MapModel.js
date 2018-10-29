function MapModel(presenter) {
	this.presenter = presenter;
	this.loadGameState();
}

MapModel.prototype.loadGameState = function() {
	// get from server
	this.territories = {
		'Kiel': Nation.GERMANY,
		'Berlin': Nation.GERMANY,
		'Munich': Nation.GERMANY,
		'Prussia': Nation.GERMANY,
		'Silesia': Nation.GERMANY,
		'Ruhr': Nation.GERMANY,
		'Warsaw': Nation.RUSSIA,
		'Livonia': Nation.RUSSIA,
		'Moscow': Nation.RUSSIA,
		'StPetersburg': Nation.RUSSIA,
		'Sevastopol': Nation.RUSSIA,
		'Ukraine': Nation.RUSSIA,
		'Ankara': Nation.TURKEY,
		'Smyrna': Nation.TURKEY,
		'Syria': Nation.TURKEY,
		'Armenia': Nation.TURKEY,
		'Constantinople': Nation.TURKEY,
		'Tyrolia': Nation.AUSTRIA,
		'Budapest': Nation.AUSTRIA,
		'Galicia': Nation.AUSTRIA,
		'Bohemia': Nation.AUSTRIA,
		'Vienna': Nation.AUSTRIA,
		'Trieste': Nation.AUSTRIA,
		'Venice': Nation.ITALY,
		'Apulia': Nation.ITALY,
		'Piedmont': Nation.ITALY,
		'Tuscany': Nation.ITALY,
		'Rome': Nation.ITALY,
		'Naples': Nation.ITALY,
		'Marseilles': Nation.FRANCE,
		'Burgundy': Nation.FRANCE,
		'Paris': Nation.FRANCE,
		'Gascony': Nation.FRANCE,
		'Picardy': Nation.FRANCE,
		'Brest': Nation.FRANCE,
		'London': Nation.ENGLAND,
		'Yorkshire': Nation.ENGLAND,
		'Wales': Nation.ENGLAND,
		'Liverpool': Nation.ENGLAND,
		'Clyde': Nation.ENGLAND,
		'Edinburgh': Nation.ENGLAND
	}
	
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
		}*/
	}
	
	this.moves = [];
	this.presenter.update(this.territories, this.units);
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