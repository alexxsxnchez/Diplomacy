function MainController() {
//	this.view = view;
	var self = this;
	document.getElementById('h').addEventListener('click', function() {
		self.onGameClicked(0);
	});
}

MainController.prototype.onGameClicked = function(id) {
	document.location.href = "/game?gameId=" + id;
}

MainController.prototype.enterGame = function(gameData) {
	//var id = gameData.id;
	//var hasAccess = gameData.hasAccess;
}