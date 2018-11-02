"use strict";

var express = require('express');
var socket = require('socket.io');
var Game = require('./Game.js');
var Room = require('./Room.js');

var app = express();
var port = 4000;

var server = app.listen(port, function() {
	console.log('listening for requests on port ' + port);
});

app.use(express.static('../Frontend'));


var io = socket(server);
var room;

io.on('connection', function(socket) {
	console.log('made connection');
//	socket.join('room');
	socket.emit('join');
	
	/*socket.on('move', function(move) {
		console.log('server received: ' + move);
	});*/
	
	socket.on('joinroom', function() {
		room = new Room(io, socket, 0);
	});
});
