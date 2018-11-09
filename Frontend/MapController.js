function MapController(view, model) {
	this.view = view;
	this.model = model;
	////// abstract into move struct -> maybe?
	this.selectedUnit = null;
	this.selectedMoveType = null;
	this.firstLocation = null;
	this.secondLocation = null;
	this.thirdLocation = null;
	/////
	this.view.addTerritorySelectedHandler(this.onTerritorySelected.bind(this));
	this.view.addMoveTypeSelectedHandler(this.onMoveTypeSelected.bind(this));
	this.view.addFinalizeSelectedHandler(this.onFinalizeSelected.bind(this));
}

MapController.prototype.onTerritorySelected = function(territory) {
	console.log(territory + " was selected");
	if(this.selectedMoveType === null) {
		if(this.firstLocation !== null) {
			this.firstLocation = null;
			this.view.closeMoveMenu();
			this.onTerritorySelected(territory);
			return;
		}
		var phase = this.model.getPhase();
		var options = [];
		if(phase === 'SPRING_RETREAT' || phase === 'FALL_RETREAT') {
			this.selectedUnit = this.model.getDislodgedUnitAt(territory);
		} else {
			this.selectedUnit = this.model.getUnitAt(territory);
		}
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
	}
}

MapController.prototype.moveSelectionComplete = function() {
	this.model.createNewMove(this.selectedUnit, this.selectedMoveType, this.firstLocation, this.secondLocation, this.thirdLocation);
	this.selectedUnit = null;
	this.selectedMoveType = null;
	this.firstLocation = null;
	this.secondLocation = null;
	this.thirdLocation = null;
	if(this.model.getIsFinalized()) {
		this.model.toggleIsFinalized();
	}
}

MapController.prototype.onFinalizeSelected = function() {
	this.model.toggleIsFinalized();
}