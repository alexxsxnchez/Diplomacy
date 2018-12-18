LocationPresentationNames = {
	"BarentsSea": "Barents Sea",
	"NorwegianSea": "Norwegian Sea",
	"NorthAtlantic": "North Atlantic Ocean",
	"Clyde": "Clyde",
	"Edinburgh": "Edinburgh",
	"NorthSea": "North Sea",
	"Norway": "Norway",
	"StPetersburg": "St. Petersburg",
	"Finland": "Finland",
	"Sweden": "Sweden",
	"GulfOfBothnia": "Gulf of Bothnia",
	"BalticSea": "Baltic Sea",
	"Livonia": "Livonia",
	"Denmark": "Denmark",
	"Skagerrak": "Skagerrak",
	"HelgolandBight": "Helgoland Bight",
	"Yorkshire": "Yorkshire",
	"Liverpool": "Liverpool",
	"IrishSea": "Irish Sea",
	"Wales": "Wales",
	"London": "London",
	"Moscow": "Moscow",
	"Warsaw": "Warsaw",
	"Prussia": "Prussia",
	"Berlin": "Berlin",
	"Kiel": "Kiel",
	"Holland": "Holland",
	"Belgium": "Belgium",
	"EnglishChannel": "English Channel",
	"MidAtlantic": "Mid-Atlantic Ocean",
	"Picardy": "Picardy",
	"Brest": "Brest",
	"Paris": "Paris",
	"Burgundy": "Burgundy",
	"Ruhr": "Ruhr",
	"Munich": "Munich",
	"Silesia": "Silesia",
	"Bohemia": "Bohemia",
	"Galicia": "Galicia",
	"Ukraine": "Ukraine",
	"Sevastopol": "Sevastopol",
	"Rumania": "Rumania",
	"Budapest": "Budapest",
	"Vienna": "Vienna",
	"Tyrolia": "Tyrolia",
	"Piedmont": "Piedmont",
	"Gascony": "Gascony",
	"Marseilles": "Marseilles",
	"Spain": "Spain",
	"Portugal": "Portugal",
	"NorthAfrica": "North Africa",
	"Tunis": "Tunis",
	"WesternMed": "Western Mediterranean",
	"GulfOfLyon": "Gulf of Lyon",
	"TyrrhenianSea": "Tyrrhenian Sea",
	"Tuscany": "Tuscany",
	"Venice": "Venice",
	"Trieste": "Trieste",
	"Serbia": "Serbia",
	"Albania": "Albania",
	"AdriaticSea": "Adriatic Sea",
	"Apulia": "Apulia",
	"Rome": "Rome",
	"Naples": "Naples",
	"IonianSea": "Ionian Sea",
	"Greece": "Greece",
	"Bulgaria": "Bulgaria",
	"Constantinople": "Constantinople",
	"BlackSea": "Black Sea",
	"Armenia": "Armenia",
	"Ankara": "Ankara",
	"Smyrna": "Smyrna",
	"AegeanSea": "Aegean Sea",
	"EasternMed": "Eastern Mediterranean",
	"Syria": "Syria"
}

function Presenter(mapView, gameView) {
	this.mapView = mapView;
	this.gameView = gameView;
}

Presenter.prototype.createCounts = function(territories, units) {
	var counts = {
		"AUSTRIA": {
			stars: 0,
			units: 0
		},
		"ENGLAND": {
			stars: 0,
			units: 0
		},
		"FRANCE": {
			stars: 0,
			units: 0
		},
		"GERMANY": {
			stars: 0,
			units: 0
		},
		"ITALY": {
			stars: 0,
			units: 0
		},
		"RUSSIA": {
			stars: 0,
			units: 0
		},
		"TURKEY": {
			stars: 0,
			units: 0
		},
		"NEUTRAL": {
			stars: 0,
			units: 0
		},
	}
	Object.keys(territories).forEach((key) => {
		if(territories[key].hasStar) {
			var nation = territories[key].nation;
			counts[nation].stars++;
		}
	});
	Object.keys(units).forEach((key) => {
		var nation = units[key].nation;
		counts[nation].units++;
	});
	return counts;
}

Presenter.prototype.update = function(year, phase, territories, units, dislodgedUnits, isFinalized) {
	this.mapView.update(territories, units, dislodgedUnits);
	this.gameView.update(year, phase, this.createCounts(territories, units), isFinalized);
	var moveListUnits;
	var isBuildPhase = false;
	if(phase === "WINTER") {
		isBuildPhase = true;
	} else if(phase === "FALL_RETREAT" || phase === "SPRING_RETREAT") {
		moveListUnits = {};
		Object.keys(dislodgedUnits).forEach((key) => {
			moveListUnits[key] = dislodgedUnits[key].unit;
		});
	} else {
		moveListUnits = units;
	}
	this.resetMoveList(moveListUnits, territories, isBuildPhase);
}

Presenter.prototype.updateIsFinalized = function(isFinalized) {
	this.gameView.updateFinalizeButton(isFinalized);
}

Presenter.prototype.resetMoveList = function(units, territories, isBuildPhase) {
	this.gameView.clearOrderLists();
	if(isBuildPhase) {
		
		return;
	}
	Object.keys(units).forEach((location) => {
		this.resetMove(units, location);
	});
}

Presenter.prototype.resetMove = function(units, location, isRetreatingUnit = false) {
	var unit = isRetreatingUnit ? units[location].unit : units[location];
	var unitType;
	if(unit.type === 'army') {
		unitType = "A";
	} else {
		unitType = "F";
	}
	var presenterLocation = LocationPresentationNames[location];
	if(unit.coast !== undefined && unit.coast !== null) {
		presenterLocation += " (";
		presenterLocation += unit.coast;
		presenterLocation += ")";
	}
	var description = unitType + " " + presenterLocation;
	this.gameView.updateMove(location, unit.nation, description);
}

Presenter.prototype.updateMoves = function(move, territories) {
	var description = "";
	var unitType;
	if(move.unit !== null && move.unit.type === 'army') {
		unitType = "A";
	} else {
		unitType = "F";
	}
	var firstLocation = LocationPresentationNames[move.firstLoc];
	if(move.unit !== null && move.unit.coast !== undefined && move.unit.coast !== null) {
		firstLocation += " (";
		firstLocation += move.unit.coast;
		firstLocation += ")";
	}
	var moveType;
	var isConvoyOrSupport = false;
	switch(move.moveType) {
		case MoveType.MOVE:
		case MoveType.MOVENC:
		case MoveType.MOVEEC:
		case MoveType.MOVESC:
		case MoveType.RETREAT:
			moveType = "-";
			break;
		case MoveType.HOLD:
			moveType = "H";
			break;
		case MoveType.SUPPORT:
			moveType = "Supports";
			isConvoyOrSupport = true;
			break;
		case MoveType.CONVOY:
			moveType = "Convoys";
			isConvoyOrSupport = true;
			break;
		case MoveType.DESTROY:
			description = "Destroy " + unitType + " " + firstLocation;
			break;
		case MoveType.BUILDARMY:
			description = "Build A " + firstLocation;
			break;
		case MoveType.BUILDFLEET:
			description = "Build F " + firstLocation;
			break;
		case MoveType.BUILDFLEETSC:
			description = "Build F " + firstLocation + " (SC)";
			break;
		case MoveType.BUILDFLEETNC:
			description = "Build F " + firstLocation + " (NC)";
			break;
	}
	if(description === "") {
		if(move.secondLoc !== null) {
			var secondLocation = LocationPresentationNames[move.secondLoc];
			if(move.thirdLoc !== null) {
				var thirdLocation = LocationPresentationNames[move.thirdLoc];
				if(move.secondLoc === move.thirdLoc && move.moveType === MoveType.SUPPORT) {
					description = unitType + " " + firstLocation + " " + moveType + " " + secondLocation + " to hold";
				} else if(move.coast !== null) {
					description = unitType + " " + firstLocation + " " + moveType + " " + secondLocation + " - " + thirdLocation + " (" + move.coast + ")";
				} else {
					description = unitType + " " + firstLocation + " " + moveType + " " + secondLocation + " - " + thirdLocation;
				}
			} else if(move.coast !== null) {
				description = unitType + " " + firstLocation + " " + moveType + " " + secondLocation + " (" + move.coast + ")";
			} else {
				description = unitType + " " + firstLocation + " " + moveType + " " + secondLocation;
			}
		} else {
			description = unitType + " " + firstLocation + " " + moveType;
		}
	}
	var nation;
	if(move.unit === null) {
		nation = territories[move.firstLoc].nation;
	} else {
		nation = move.unit.nation;
	}
	this.gameView.updateMove(move.firstLoc, nation, description, true);
}
