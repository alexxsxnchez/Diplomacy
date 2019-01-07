"use strict";

var express = require('express');
var socket = require('socket.io');
var Game = require('./Game.js');
var Room = require('./Room.js');
var GameManager = require('./GameManager.js');

var app = express();
var port = 4000;

var server = app.listen(port, function() {
	console.log('listening for requests on port ' + port);
});

app.use(express.static('../Frontend'));
app.get('/game', function(req, res) {
	var id = req.query.gameId;
	if(id && !isNaN(id)) {
		
		var game = gameManager.getGameWithId(id);
		console.log(game);
		if(/*game*/true) {
			var path = require('path');
			res.sendFile(path.resolve(__dirname + "/../Frontend/try.html"));
			//res.send('hello worldafj;fhaha');
			//console.log(id);
			//next();
		} else {
			res.status(404);
			res.send();
		}
	} else {
		res.status(404);
		res.send();
	}
});


var io = socket(server);
var gameManager = new GameManager();
var room;

io.on('connection', function(socket) {
	console.log('made connection');
//	socket.join('room');
	//socket.emit('join');
	
	/*socket.on('move', function(move) {
		console.log('server received: ' + move);
	});*/
	
	/*socket.on('joinroom', function() {
		console.log('creating room');
		room = new Room(io, socket, 0);
	});*/
	gameManager.loadDefaultGameList(io, socket);
	
});
