function MainController() {
	var self = this;
	document.getElementById('h').addEventListener('click', function() {
		self.onGameClicked(0);
	});
}

MainController.prototype.onGameClicked = function(id) {
	document.location.href = "/game?gameId=" + id;
}