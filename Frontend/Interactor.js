function GameInteractor(gameModel) {
	this.setupServerConnection();
	this.registerGameModel(gameModel);
	
//	this.mainController = mainController;
//	this.mainController.addGameClickedHandler(this.requestGameAccess.bind(this));
	
}

GameInteractor.prototype.registerGameModel = function(model) {
	model.addMoveCreatedHandler(this.onMoveCreated.bind(this));
	model.addMoveDeletedHandler(this.onMoveDeleted.bind(this));
	model.addFinalizeHandler(this.onFinalizedToggled.bind(this));
	
	this.socket.on('update', function(data) {
		console.log('received update data');
		model.dataReceived(data);
	});
	this.socket.emit('requestgamedata');
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
	/*this.socket.on('join', function(data) {
		console.log('I have joined room');
		//self.model.loadGameState();
	});*/
	
	
	
}

/*Interactor.prototype.requestGameAccess = function(id) {
	this.socket.emit('gameaccess', id);
	var self = this;
	this.socket.on('gameaccess', function(gameData) {
		self.mainController.enterGame(gameData);		
	});
}*/

