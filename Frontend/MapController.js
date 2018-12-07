var SelectPhase = {
	NONE: 'NONE',
	FIRST: 'FIRST',
	MOVEMENU: 'MOVEMENU',
	SECOND: 'SECOND',
	CONVOYMENU: 'CONVOYMENU',
	COAST1: 'COAST1',
	THIRD: 'THIRD',
	COAST2: 'COAST2'
}


function MapController(mapView, gameView, model) {
	this.mapView = mapView;
	this.gameView = gameView;
	this.model = model;
	////// abstract into move struct -> maybe?
	this.selectPhase = SelectPhase.NONE;
	this.selectedUnit = null;
	this.selectedMoveType = null;
	this.firstLocation = null;
	this.secondLocation = null;
	this.thirdLocation = null;
	this.coast = null;
	this.viaConvoy = null;
	/////
	this.mapView.addTerritorySelectedHandler(this.onTerritorySelected.bind(this));
	this.mapView.addMoveTypeSelectedHandler(this.onMoveTypeSelected.bind(this));
	this.mapView.addCoastSelectedHandler(this.onCoastSpecifierSelected.bind(this));
	this.gameView.addFinalizeSelectedHandler(this.onFinalizeSelected.bind(this));
	this.mapView.addRouteSelectedHandler(this.onRouteSelected.bind(this));
}

MapController.prototype.onTerritorySelected = function(territory) {
	switch(this.selectPhase) {
		case SelectPhase.NONE:
			this.firstTerritorySelected(territory);
			break;
		case SelectPhase.MOVEMENU:
			this.moveMenuOptionNotSelected(territory);
			break;
		case SelectPhase.FIRST:
			this.secondTerritorySelected(territory);
			break;
		case SelectPhase.CONVOYMENU:
			this.coastMenuOptionNotSelected();
			break;
		case SelectPhase.COAST1:
			this.coastMenuOptionNotSelected();
			break;
		case SelectPhase.SECOND:
			this.thirdTerritorySelected(territory);
			break;
		case SelectPhase.COAST2:
			this.coastMenuOptionNotSelected();
			break;
	}
}

MapController.prototype.firstTerritorySelected = function(territory) {
	var phase = this.model.getPhase();
	var selectedUnit;
	if(phase === 'SPRING_RETREAT' || phase === 'FALL_RETREAT') {
		selectedUnit = this.model.getDislodgedUnitAt(territory);
	} else {
		selectedUnit = this.model.getUnitAt(territory);
	}
	var options = [];
	if(selectedUnit !== null) {
		this.mapView.highlightTerritory(territory);
		if(phase === 'WINTER') {
			if(this.model.getStarToUnitCount(selectedUnit.nation) < 0) {
				options.push(MoveType.DESTROY);
			} else {
				return;
			}
		} else if(phase === 'SPRING_RETREAT' || phase === 'FALL_RETREAT') {
			options.push(MoveType.RETREAT);
			options.push(MoveType.DESTROY);
		} else {
			options.push(MoveType.HOLD);
			options.push(MoveType.SUPPORT);
			options.push(MoveType.MOVE);
			if(selectedUnit.type === 'fleet') {
				options.push(MoveType.CONVOY);
			}
		}
	} else if(phase === "WINTER") {
		this.mapView.highlightTerritory(territory);
		// check if home centre and then we can show build option
		if(this.model.territories[territory] === undefined) {
			return;
		}
		var nation = this.model.territories[territory].nation;
		var homeCentres = this.model.getHomeCentres(nation);
		if(homeCentres.includes(territory) && this.model.getStarToUnitCount(nation) > 0) {
			options = this.model.getBuildOptions(territory);
		} else {
			return;
		}
	} else {
		return;
	}
	this.firstLocation = territory;
	this.selectedUnit = selectedUnit;
	this.selectPhase = SelectPhase.MOVEMENU;
	this.mapView.showMoveMenu(options, territory);
}

MapController.prototype.moveMenuOptionNotSelected = function(territory) {
	this.mapView.closeMoveMenu();
	this.resetMoveSelection();
	this.firstTerritorySelected(territory);
}

MapController.prototype.secondTerritorySelected = function(territory) {
	var options = [];
	this.secondLocation = territory;
	var movingPieceIsFleet = this.selectedUnit.type === 'fleet' && (this.selectedMoveType === MoveType.MOVE || this.selectedMoveType === MoveType.RETREAT);
	if((territory === 'Spain' || territory === 'Bulgaria' || territory === 'StPetersburg') && this.coast === null && movingPieceIsFleet) {
		if(territory === 'Bulgaria') {
			options.push(MoveType.MOVEEC);
		} else {
			options.push(MoveType.MOVENC);
		}
		options.push(MoveType.MOVESC);
		this.selectPhase = SelectPhase.COAST1;
		this.mapView.showMoveMenu(options, territory);
		return;
	}
	console.log('haha');
	if(this.selectedUnit.type === 'army' && this.selectedMoveType === MoveType.MOVE && this.viaConvoy === null) { 
		console.log('lolol');
		var canConvoyToAdjacent = ConvoyPairs.reduce((alreadyFound, pair) => {
			if(alreadyFound) {
				return true;
			}
			var otherUnit;
			if(this.firstLocation === pair.coast1 && this.secondLocation === pair.coast2) {
				otherUnit = this.model.getUnitAt(pair.coast2);
			} else if(this.firstLocation === pair.coast2 && this.secondLocation === pair.coast1) {
				otherUnit = this.model.getUnitAt(pair.coast1);
			} else {
				return false;
			}
			if(otherUnit !== null) {
				console.log(JSON.stringify(otherUnit));
				return pair.waters.reduce((existsAFleet, water) => {
					if(existsAFleet) {
						return true;
					}
					return this.model.getUnitAt(water) !== null;
				}, false);
			}
			return false;	
		}, false);
		
		if(canConvoyToAdjacent) {
			this.selectPhase = SelectPhase.CONVOYMENU;
			options.push(MoveType.LANDROUTE);
			options.push(MoveType.VIACONVOY);
			this.mapView.showMoveMenu(options, territory);
			return;
		}
	}
	
	switch(this.selectedMoveType) {
		case MoveType.MOVE:
		case MoveType.RETREAT:
			this.moveSelectionComplete();
			break;
		default:
			this.selectPhase = SelectPhase.SECOND;
	}
}

MapController.prototype.coastMenuOptionNotSelected = function() {
	this.mapView.closeMoveMenu();
	this.resetMoveSelection();
}

MapController.prototype.thirdTerritorySelected = function(territory) {
	var options = [];
	this.thirdLocation = territory;
	var movingPiece = this.model.getUnitAt(this.secondLocation);
	var movingPieceIsFleet = (movingPiece !== null && movingPiece.type === 'fleet');
	console.log(territory + this.secondLocation);
	if((territory === 'Spain' || territory === 'Bulgaria' || territory === 'StPetersburg') && this.coast === null && movingPieceIsFleet && territory !== this.secondLocation) {
		if(territory === 'Bulgaria') {
			options.push(MoveType.MOVEEC);
		} else {
			options.push(MoveType.MOVENC);
		}
		options.push(MoveType.MOVESC);
		this.selectPhase = SelectPhase.COAST2;
		this.mapView.showMoveMenu(options, territory);
	} else {
		this.moveSelectionComplete();
	}
}

MapController.prototype.onMoveTypeSelected = function(moveType) {
	console.log(moveType + " was selected");
	this.selectedMoveType = moveType;
	this.mapView.closeMoveMenu();
	switch(moveType) {
		case MoveType.BUILDARMY:
		case MoveType.BUILDFLEET:
		case MoveType.BUILDFLEETNC:
		case MoveType.BUILDFLEETSC:
		case MoveType.DESTROY:
		case MoveType.HOLD:
			this.moveSelectionComplete();
			break;
		default:
			this.selectPhase = SelectPhase.FIRST;
	}
}

MapController.prototype.onCoastSpecifierSelected = function(coastSpecifier) {
	var coast;
	switch(coastSpecifier) {
		case MoveType.MOVENC:
			coast = 'NC';
			break;
		case MoveType.MOVESC:
			coast = 'SC';
			break;
		case MoveType.MOVEEC:
			coast = 'EC';
			break;
	}
	this.mapView.closeMoveMenu();
	this.coast = coast;
	if(this.selectPhase === SelectPhase.COAST1) {
		this.secondTerritorySelected(this.secondLocation);
	} else {
		this.thirdTerritorySelected(this.thirdLocation);
	}
}

MapController.prototype.onRouteSelected = function(route) {
	if(route === MoveType.VIACONVOY) {
		this.viaConvoy = true;
	} else {
		this.viaConvoy = false;
	}
	this.mapView.closeMoveMenu();
	this.secondTerritorySelected(this.secondLocation);
}

MapController.prototype.resetMoveSelection = function() {
	this.selectPhase = SelectPhase.NONE;
	this.selectedUnit = null;
	this.selectedMoveType = null;
	this.firstLocation = null;
	this.secondLocation = null;
	this.thirdLocation = null;
	this.coast = null;
	this.viaConvoy = null;
	this.mapView.unhighlightTerritory();
}

MapController.prototype.moveSelectionComplete = function() {
	if(this.viaConvoy === null) {
		this.viaConvoy = false;
	}
	this.model.createNewMove(this.selectedUnit, this.selectedMoveType, this.firstLocation, this.secondLocation, this.thirdLocation, this.coast, this.viaConvoy);
	this.resetMoveSelection();
	if(this.model.getIsFinalized()) {
		this.model.toggleIsFinalized();
	}
}

MapController.prototype.onFinalizeSelected = function() {
	this.model.toggleIsFinalized();
	this.coastMenuOptionNotSelected();
}
