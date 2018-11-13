var SelectPhase = {
	NONE: 'NONE',
	FIRST: 'FIRST',
	MOVEMENU: 'MOVEMENU',
	SECOND: 'SECOND',
	COAST1: 'COAST1'
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
	this.view.addCoastSeletedHandler(this.onCoastSpecifierSelected.bind(this));
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
	this.secondLocation = territory;
	if((territory === 'Spain' || territory === 'Bulgaria' || territory === 'StPetersburg') && this.coast === null) {
		if(territory === 'Bulgaria') {
			options.push(MoveType.MOVEEC);
		} else {
			options.push(MoveType.MOVENC);
		}
		options.push(MoveType.MOVESC);
		this.selectPhase = SelectPhase.COAST1;
		this.view.showMoveMenu();
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
	this.thirdLocation = territory;
	if((territory === 'Spain' || territory === 'Bulgaria' || territory === 'StPetersburg') && this.coast === null) {
		if(territory === 'Bulgaria') {
			options.push(MoveType.MOVEEC);
		} else {
			options.push(MoveType.MOVENC);
		}
		options.push(MoveType.MOVESC);
		this.selectPhase = SelectPhase.COAST2;
		this.view.showMoveMenu();
	} else {
		this.moveSelectionComplete();
	}
}



/*


	console.log(territory + " was selected");
	if(this.selectedMoveType === null) {
		if(this.firstLocation !== null) {
			this.firstLocation = null;
			this.view.closeMoveMenu();
			this.onTerritorySelected(territory);
			return;
		}
		setSelectedUnit(territory);
		var options = [];
		if(this.selectedUnit !== null) {
			if(phase === 'WINTER') {
				options.push(MoveType.DESTROY);
			} else if(phase === 'SPRING_RETREAT' || phase === 'FALL_RETREAT') {
				options.push(MoveType.RETREAT);
				options.push(MoveType.DESTROY);
			} else {
				options.push(MoveType.HOLD);
				options.push(MoveType.SUPPORT);
				options.push(MoveType.MOVE);
				if(this.selectedUnit.type === 'fleet') {
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
		this.view.showMoveMenu(options);
	} else if(this.secondLocation === null) {
		if((territory === 'Spain' || territory === 'Bulgaria' || territory === 'StPetersburg') && this.coast === null) {
			if(territory === 'Bulgaria') {
				options.push(MoveType.MOVEEC);
			} else {
				options.push(MoveType.MOVENC);
			}
			options.push(MoveType.MOVESC);
			this.view.showMoveMenu();
		} else {
			this.secondLocation = territory;
			this.coast1 = 'NONE';
			switch(this.selectedMoveType) {
				case 'MOVE':
				case 'RETREAT':
					this.moveSelectionComplete();
			}
		}
	} else if(this.coast1 === null) {
		this.resetTerritorySelected();
		this.onTerritorySelected(territory);
	} else if(this.thirdLocation === null) {
		this.thirdLocation = territory;
		this.moveSelectionComplete();	
	} else if(this.coast2 === null) {
		this.resetTerritorySelected();
		this.onTerritorySelected(territory);
	}
	
	
	
	
	
	
	else if(this.secondLocation === null) {
		this.secondLocation = territory;
		
		
		if(!this.isDoubleCoastTerr(territory)) {
			switch(this.selectedMoveType) {
				case 'MOVE':
				case 'RETREAT':
					this.moveSelectionComplete();
			}
		}
	} else 
		
		
	
	
		if((territory === 'Spain' || territory === 'Bulgaria' || territory === 'StPetersburg') && this.coast === null) {
			if(territory === 'Bulgaria') {
				options.push(MoveType.MOVEEC);
			} else {
				options.push(MoveType.MOVENC);
			}
			options.push(MoveType.MOVESC);
			this.view.showMoveMenu();
		} else if(this.secondLocation === null) {
			this.secondLocation = territory;
			switch(this.selectedMoveType) {
				case 'MOVE':
				case 'RETREAT':
					this.moveSelectionComplete();
			}
		} else {
			this.thirdLocation = territory;
			this.moveSelectionComplete();	
		}
	}
}
*/
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
	switch(coastSpecifier) {
		case 'MOVENC':
			this.coast = 'NC';
			break;
		case 'MOVESC':
			this.coast = 'SC';
			break;
		case 'MOVEEC':
			this.coast = 'EC';
			break;
	}
	this.view.closeMoveMenu();
	if(this.selectPhase === SelectPhase.COAST1) {
		this.secondTerritorySelected(this.secondLocation);
	} else {
		this.thirdTerritorySelected(this.thirdLocation);
	}
}

MapController.prototype.moveSelectionComplete = function() {
	this.model.createNewMove(this.selectedUnit, this.selectedMoveType, this.firstLocation, this.secondLocation, this.thirdLocation);
	this.resetMoveSelection();
	if(this.model.getIsFinalized()) {
		this.model.toggleIsFinalized();
	}
}

MapController.prototype.onFinalizeSelected = function() {
	this.model.toggleIsFinalized();
}