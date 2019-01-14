function GameInteractor(gameModel) {
	this.setupServerConnection();
	this.registerGameModel(gameModel);
}

GameInteractor.prototype.registerGameModel = function(model) {
	model.addMoveCreatedHandler(this.onMoveCreated.bind(this));
	model.addMoveDeletedHandler(this.onMoveDeleted.bind(this));
	model.addFinalizeHandler(this.onFinalizedToggled.bind(this));
	
	this.socket.on('update', function(data) {
		console.log('received update data');
		model.dataReceived(data);
	});
	
	var url = window.location.href;
	console.log(url);
	var getQuery = url.split('?')[1];
	console.log(getQuery);
	var params = getQuery.split('&')[0];
	console.log(params);
	var gameId = params.split('=');
	console.log(gameId);
	this.socket.emit('requestgamedata', parseInt(gameId[1]), "userid");
}

GameInteractor.prototype.onMoveCreated = function(move) {
	console.log('emitting move');
	this.socket.emit('move', move);
}

GameInteractor.prototype.onMoveDeleted = function(location) {
	console.log('deleting move');
	this.socket.emit('deletemove', location);
}

GameInteractor.prototype.onFinalizedToggled = function(isFinalized) {
	if(isFinalized) {
		this.socket.emit('finalize', 'some nation');
	} else {
		this.socket.emit('unfinalize', 'some nation');
	}
}

GameInteractor.prototype.setupServerConnection = function() {
	this.socket = io.connect();
	var self = this;
	console.log('setting up server connection');
}

