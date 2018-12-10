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

Presenter.prototype.update = function(year, phase, territories, units, dislodgedUnits, isFinalized) {
	this.mapView.update(territories, units, dislodgedUnits);
	this.gameView.update(year, phase, isFinalized);
	var moveListUnits;
	var isBuildPhase = false;
	if(phase === "WINTER") {
		isBuildPhase = true;
	} else if(phase === "RETREAT_FALL" || phase === "RETREAT_SPRING") {
		moveListUnits = dislodgeUnits;
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
		//TODO
		return;
	}
	Object.keys(units).forEach((location) => {
		var unitType;
		if(units[location].type === 'army') {
			unitType = "A";
		} else {
			unitType = "F";
		}
		var presenterLocation = LocationPresentationNames[location];
		if(units[location].coast !== undefined && units[location].coast !== null) {
			presenterLocation += " (";
			presenterLocation += units[location].coast;
			presenterLocation += ")";
		}
		var description = unitType + " " + presenterLocation;
		this.gameView.updateMove(location, units[location].nation, description);
	});
}

Presenter.prototype.updateMoves = function(move, territories) {
	/*var move = {};
	move.unit = unit;
	move.moveType = moveType;
	move.firstLoc = firstLocation;
	move.secondLoc = secondLocation;
	move.thirdLoc = thirdLocation;
	move.coast = coast;
	move.viaConvoy = viaConvoy;*/
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
	this.gameView.updateMove(move.firstLoc, nation, description);
}
