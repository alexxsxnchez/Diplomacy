function Interactor(model) {
	this.model = model;
	this.setupServerConnection();
	this.model.addMoveCreatedHandler(this.onMoveCreated.bind(this));
	this.model.addFinalizeHandler(this.onFinalizedToggled.bind(this));
}

Interactor.prototype.onMoveCreated = function(move) {
	console.log('emitting move');
	this.socket.emit('move', move);
}

Interactor.prototype.onFinalizedToggled = function(isFinalized) {
	if(isFinalized) {
		this.socket.emit('finalize', 'some nation');
	} else {
		this.socket.emit('unfinalize', 'some nation');
	}
}

Interactor.prototype.setupServerConnection = function() {
	this.socket = io.connect();
	var self = this;
	this.socket.on('join', function(data) {
		console.log('I have joined room');
		//self.model.loadGameState();
	});
	
	this.socket.on('update', function(data) {
		console.log('received update data');
		self.model.dataReceived(data);
	});
	
	this.socket.emit('joinroom');
}

