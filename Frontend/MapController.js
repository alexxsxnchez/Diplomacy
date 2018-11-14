var SelectPhase = {
	NONE: 'NONE',
	FIRST: 'FIRST',
	MOVEMENU: 'MOVEMENU',
	SECOND: 'SECOND',
	COAST1: 'COAST1',
	THIRD: 'THIRD',
	COAST2: 'COAST2'
}


function MapController(view, model) {
	this.view = view;
	this.model = model;
	////// abstract into move struct -> maybe?
	this.selectPhase = SelectPhase.NONE;
	this.selectedUnit = null;
	this.selectedMoveType = null;
	this.firstLocation = null;
	this.secondLocation = null;
	this.thirdLocation = null;
	this.coast1 = null;
	this.coast2 = null;
	/////
	this.view.addTerritorySelectedHandler(this.onTerritorySelected.bind(this));
	this.view.addMoveTypeSelectedHandler(this.onMoveTypeSelected.bind(this));
	this.view.addCoastSelectedHandler(this.onCoastSpecifierSelected.bind(this));
	this.view.addFinalizeSelectedHandler(this.onFinalizeSelected.bind(this));
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
		if(phase === 'WINTER') {
			options.push(MoveType.DESTROY);
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
		// check if home centre and then we can show build option
		if(this.model.territories[territory] === undefined) {
			return;
		}
		var nation = this.model.territories[territory].nation;
		var homeCentres = this.model.getHomeCentres(nation);
		if(homeCentres.includes(territory)) {
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
	this.view.showMoveMenu(options);
}

MapController.prototype.moveMenuOptionNotSelected = function(territory) {
	this.view.closeMoveMenu();
	this.resetMoveSelection();
	this.firstTerritorySelected(territory);
}

MapController.prototype.secondTerritorySelected = function(territory) {
	var options = [];
	this.secondLocation = territory;
	if((territory === 'Spain' || territory === 'Bulgaria' || territory === 'StPetersburg') && this.coast1 === null && this.selectedUnit.type === 'fleet') {
		if(territory === 'Bulgaria') {
			options.push(MoveType.MOVEEC);
		} else {
			options.push(MoveType.MOVENC);
		}
		options.push(MoveType.MOVESC);
		this.selectPhase = SelectPhase.COAST1;
		this.view.showMoveMenu(options);
	} else {
		switch(this.selectedMoveType) {
			case 'MOVE':
			case 'RETREAT':
				this.moveSelectionComplete();
				break;
			default:
				this.selectPhase = SelectPhase.SECOND;
		}
	}
}

MapController.prototype.coastMenuOptionNotSelected = function() {
	this.view.closeMoveMenu();
	this.resetMoveSelection();
}

MapController.prototype.thirdTerritorySelected = function(territory) {
	var options = [];
	this.thirdLocation = territory;
	var movingPiece = this.model.getUnitAt(this.secondLocation);
	var movingPieceIsFleet = (movingPiece !== null && movingPiece.type === 'fleet');
	if((territory === 'Spain' || territory === 'Bulgaria' || territory === 'StPetersburg') && this.coast2 === null && movingPieceIsFleet) {
		if(territory === 'Bulgaria') {
			options.push(MoveType.MOVEEC);
		} else {
			options.push(MoveType.MOVENC);
		}
		options.push(MoveType.MOVESC);
		this.selectPhase = SelectPhase.COAST2;
		this.view.showMoveMenu(options);
	} else {
		this.moveSelectionComplete();
	}
}

MapController.prototype.onMoveTypeSelected = function(moveType) {
	console.log(moveType + " was selected");
	this.selectedMoveType = moveType;
	this.view.closeMoveMenu();
	switch(moveType) {
		case 'BUILDARMY':
		case 'BUILDFLEET':
		case 'BUILDFLEETNC':
		case 'BUILDFLEETSC':
		case 'DESTROY':
		case 'HOLD':
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
	this.view.closeMoveMenu();
	if(this.selectPhase === SelectPhase.COAST1) {
		this.coast1 = coast;
		this.secondTerritorySelected(this.secondLocation);
	} else {
		this.coast2 = coast;
		this.thirdTerritorySelected(this.thirdLocation);
	}
}

MapController.prototype.resetMoveSelection = function() {
	this.selectPhase = SelectPhase.NONE;
	this.selectedUnit = null;
	this.selectedMoveType = null;
	this.firstLocation = null;
	this.secondLocation = null;
	this.thirdLocation = null;
	this.coast1 = null;
	this.coast2 = null;
}

MapController.prototype.moveSelectionComplete = function() {
	this.model.createNewMove(this.selectedUnit, this.selectedMoveType, this.firstLocation, this.secondLocation, this.coast1, this.thirdLocation, this.coast2);
	this.resetMoveSelection();
	if(this.model.getIsFinalized()) {
		this.model.toggleIsFinalized();
	}
}

MapController.prototype.onFinalizeSelected = function() {
	this.model.toggleIsFinalized();
}