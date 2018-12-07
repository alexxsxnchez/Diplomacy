function GameView() {

}

GameView.prototype.update = function(year, phase, isFinalized) {
	document.getElementById('yearText').innerHTML = year;
	document.getElementById('phaseText').innerHTML = phase;
	this.updateFinalizeButton(isFinalized);
}

GameView.prototype.addFinalizeSelectedHandler = function(finalizeClickHandler) {
	document.getElementById('finalizebutton').addEventListener("click", function() {
		finalizeClickHandler();
	});
}

GameView.prototype.updateFinalizeButton = function(isFinalized) {
	var text = 'Finalize';
	if(isFinalized) {
		text = 'Unfinalize';
	}

	document.getElementById('finalizebutton').innerHTML = text;
}

