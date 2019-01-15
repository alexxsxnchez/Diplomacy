function Validator() {}

Validator.prototype.validateMoveData = function(moveData) {
	if(moveData.hasOwnProperty('unit') &&
		moveData.hasOwnProperty('moveType') &&
		moveData.hasOwnProperty('firstLoc') &&
		moveData.hasOwnProperty('secondLoc') && 
		moveData.hasOwnProperty('thirdLoc') &&
		moveData.hasOwnProperty('coast') &&
		moveData.hasOwnProperty('viaConvoy') &&
		
		this.validateMoveType(moveData.moveType, moveData.firstLoc, moveData.secondLoc, moveData.thirdLoc) &&
		this.validateLocation(moveData.firstLoc) &&
		this.validateLocation(moveData.secondLoc) &&
		this.validateLocation(moveData.thirdLoc) &&
		this.validateUnit(moveData.moveType, moveData.unit) && 
		this.validateCoast(moveData.coast) &&
		this.validateViaConvoy(moveData.viaConvoy))
	{
		console.log("MOVEVALIDATED");
		return true;
		
	}
	console.log("MOVENOT ACCEPTED");
	return false;
}

Validator.prototype.validateMoveType = function(moveType, firstLoc, secondLoc, thirdLoc) {
	if(typeof moveType != 'string') {
		return false;
	}
	switch(moveType) {
		case 'SUPPORT':
		case 'CONVOY':
			if(thirdLoc === null) {
				return false;
			}
		case 'MOVE':
		case 'RETREAT':
			if(secondLoc === null) {
				return false;
			}
		case 'HOLD':
		case 'BUILDARMY':
		case 'BUILDFLEET':
		case 'BUILDFLEETSC':
		case 'BUILDFLEETNC':
		case 'DESTROY':
			if(firstLoc === null) {
				return false;
			}
			if(moveType === 'BUILDARMY' || moveType === 'BUILDFLEET' || moveType === 'BUILDFLEETSC' || moveType === 'BUILDFLEETNC') {
				return this.validateBuild(moveType, firstLoc);
			}
			break;
		default:
			return false;
	}
	return true;
}

Validator.prototype.validateBuild = function(moveType, territory) {
	switch(territory) {
		case 'StPetersburg':
			if(moveType === 'BUILDARMY' || moveType === 'BUILDFLEETSC' || moveType === 'BUILDFLEETNC') {
				return true;
			}
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
			if(moveType === 'BUILDFLEET') {
				return true;
			}
		case 'Paris':
		case 'Munich':
		case 'Warsaw':
		case 'Moscow':
		case 'Vienna':
		case 'Budapest':
			if(moveType === 'BUILDARMY') {
				return true;
			}
		default:
			return false;
	}
	return false;
}

Validator.prototype.validateLocation = function(location) {
	if(location === null) {
		return true;
	} else if(typeof location != 'string') {
		return false;
	}
	var validLocations = [
		'NorthAtlantic',
		'MidAtlantic',
		'IrishSea',
		'NorthSea',
		'HelgolandBight',
		'Skagerrak',
		'BalticSea',
		'GulfOfBothnia',
		'NorwegianSea',
		'BarentsSea',
		'EnglishChannel',
		'WesternMed',
		'GulfOfLyon',
		'TyrrhenianSea',
		'IonianSea',
		'AdriaticSea',
		'AegeanSea',
		'EasternMed',
		'BlackSea',
		'Clyde',
		'Edinburgh',
		'Yorkshire',
		'Liverpool',
		'Wales',
		'London',
		'Brest',
		'Picardy',
		'Burgundy',
		'Paris',
		'Gascony',
		'Marseilles',
		'Spain',
		'Portugal',
		'NorthAfrica',
		'Tunis',
		'Piedmont',
		'Tuscany',
		'Rome',
		'Naples',
		'Apulia',
		'Venice',
		'Belgium',
		'Holland',
		'Ruhr',
		'Munich',
		'Kiel',
		'Berlin',
		'Silesia',
		'Prussia',
		'Denmark',
		'Sweden',
		'Norway',
		'Finland',
		'StPetersburg',
		'Moscow',
		'Livonia',
		'Warsaw',
		'Ukraine',
		'Sevastopol',
		'Bohemia',
		'Galicia',
		'Vienna',
		'Tyrolia',
		'Trieste',
		'Budapest',
		'Serbia',
		'Albania',
		'Greece',
		'Bulgaria',
		'Rumania',
		'Constantinople',
		'Ankara',
		'Armenia',
		'Syria',
		'Smyrna'
	];
	return validLocations.includes(location);
}

Validator.prototype.validateUnit = function(moveType, unit) {
	if(moveType === 'BUILDARMY' || moveType === 'BUILDFLEET' || moveType === 'BUILDFLEETSC' || moveType === 'BUILDFLEETNC') {
		return unit === null;
	} else if(unit === null) {
		return false;
	}
	
	if(unit.hasOwnProperty('type') && 
		unit.hasOwnProperty('nation') && 
		typeof unit.type === 'string' && 
		(unit.type === 'army' || unit.type === 'fleet') && 
		typeof unit.nation === 'string') {
		
		switch(unit.nation) {
			case 'AUSTRIA':
			case 'ENGLAND':
			case 'FRANCE':
			case 'GERMANY':
			case 'ITALY':
			case 'RUSSIA':
			case 'TURKEY':
				return true;
			default:
				return false;
		}
	}
	return false;
}

Validator.prototype.validateCoast = function(coast) {
	if(coast === null) {
		return true;
	}
	return typeof coast === 'string' && (coast === 'EC' || coast === 'SC' || coast === 'NC');
}

Validator.prototype.validateViaConvoy = function(viaConvoy) {
	return typeof viaConvoy === 'boolean';
}

module.exports = Validator;

/*
var move = {};
	move.unit = unit;
	move.moveType = moveType;
	move.firstLoc = firstLocation;
	move.secondLoc = secondLocation;
	move.thirdLoc = thirdLocation;
	move.coast = coast;
	move.viaConvoy = viaConvoy;
	this.moveCreatedHandler(move);
	
	*/
