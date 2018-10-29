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
}

MapController.prototype.onTerritorySelected = function(territory) {
	console.log(territory + " was selected");
	if(this.selectedMoveType === null) {
		console.log("selectedMoveType is null");
		if(this.firstLocation !== null) {
			console.log("firstLocation is not null thought BAD");
			this.view.onFailureToClickMoveMenu();
			return;
		}
		this.selectedUnit = this.model.getUnitAt(territory);
		if(this.selectedUnit !== null) {
			this.firstLocation = territory;
			this.view.showMoveMenu();
		}
	} else if(this.secondLocation === null) {
		this.secondLocation = territory;
		switch(this.selectedMoveType) {
			/*case MoveType.MOVE:
			case MoveType.RETREAT:*/
			case 'MOVE':
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
		/*case MoveType.HOLD:
		case MoveType.BUILD:
		case MoveType.DESTROY:*/
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
}