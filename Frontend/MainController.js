function MainController() {
//	this.view = view;
	var self = this;
	document.getElementById('h').addEventListener('click', function() {
		self.onGameClicked(0);
	});
}

MainController.prototype.addGameClickedHandler = function(handler) {
	this.gameClickedHandler = handler;
}

MainController.prototype.onGameClicked = function(id) {
	this.gameClickedHandler(id);
	console.log("haglaj");
	document.location.href = "/game?gameId=" + id;
}

MainController.prototype.enterGame = function(gameData) {
	//var id = gameData.id;
	//var hasAccess = gameData.hasAccess;
}