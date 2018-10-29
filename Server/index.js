var express = require('express');
var socket = require('socket.io');

var app = express();
var server = app.listen(4000, function() {
	console.log('listening for requests on port ' + app.port);
});

app.use(express.static('../Frontend'));


var io = socket(server);

io.on('connection', function(socket) {
	console.log('made connection');
	socket.join('room');
	socket.emit('join');
	
	socket.on('move', function(move) {
		console.log('server received: ' + move);
	});
});