function Presenter(mapView, gameView) {
	this.mapView = mapView;
	this.gameView = gameView;
}

Presenter.prototype.update = function(year, phase, territories, units, dislodgedUnits, isFinalized) {
	this.mapView.update(territories, units, dislodgedUnits);
	this.gameView.update(year, phase, isFinalized);
}

Presenter.prototype.updateIsFinalized = function(isFinalized) {
	this.gameView.updateFinalizeButton(isFinalized);
}