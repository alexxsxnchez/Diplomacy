function Interactor(model) {
	this.model = model;
	this.setupServerConnection();
	this.model.addMoveCreatedHandler(this.onMoveCreated.bind(this));
}

Interactor.prototype.onMoveCreated = function(move) {
	console.log('emitting move');
	this.socket.emit('move', move);
}

Interactor.prototype.setupServerConnection = function() {
	this.socket = io.connect();
	var self = this;
	this.socket.on('join', function(data) {
		console.log('I have joined room');
		self.model.loadGameState();
	});
}

