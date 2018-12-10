function GameView() {

}

GameView.prototype.update = function(year, phase, isFinalized) {
	document.getElementById('yearText').innerHTML = year;
	document.getElementById('phaseText').innerHTML = phase;
	this.updateFinalizeButton(isFinalized);
}

GameView.prototype.addFinalizeSelectedHandler = function(finalizeClickHandler) {
	document.getElementById('finalizebutton').addEventListener("click", function() {
		finalizeClickHandler();
	});
}

GameView.prototype.updateFinalizeButton = function(isFinalized) {
	var text = 'Finalize';
	if(isFinalized) {
		text = 'Unfinalize';
	}

	document.getElementById('finalizebutton').innerHTML = text;
}

GameView.prototype.clearOrderLists = function() {
	var austriaList = document.getElementById("austria-orders");
	while (austriaList.firstChild) {
    	austriaList.removeChild(austriaList.firstChild);
	}
	var englandList = document.getElementById("england-orders");
	while (englandList.firstChild) {
    	englandList.removeChild(englandList.firstChild);
	}
	var franceList = document.getElementById("france-orders");
	while (franceList.firstChild) {
    	franceList.removeChild(franceList.firstChild);
	}
	var germanyList = document.getElementById("germany-orders");
	while (germanyList.firstChild) {
    	germanyList.removeChild(germanyList.firstChild);
	}
	var italyList = document.getElementById("italy-orders");
	while (italyList.firstChild) {
    	italyList.removeChild(italyList.firstChild);
	}
	var russiaList = document.getElementById("russia-orders");
	while (russiaList.firstChild) {
    	russiaList.removeChild(russiaList.firstChild);
	}
	var turkeyList = document.getElementById("turkey-orders");
	while (turkeyList.firstChild) {
    	turkeyList.removeChild(turkeyList.firstChild);
	}
}

GameView.prototype.updateMove = function(location, nation, description) {
	var idName;
	switch(nation) {
		case Nation.AUSTRIA:
			idName = "austria-orders";
			break;
		case Nation.ENGLAND:
			idName = "england-orders";
			break;
		case Nation.FRANCE:
			idName = "france-orders";
			break;
		case Nation.GERMANY:
			idName = "germany-orders";
			break;
		case Nation.ITALY:
			idName = "italy-orders";
			break;
		case Nation.RUSSIA:
			idName = "russia-orders";
			break;
		case Nation.TURKEY:
			idName = "turkey-orders";
			break;
	}
	var list = document.getElementById(idName);
	var oldOrder = document.getElementById('order_' + location);
	var orderExists = oldOrder !== undefined && oldOrder !== null;
	var order = document.createElement('p');
	order.setAttribute('id', 'order_' + location);
	var text = document.createTextNode(description);
	order.appendChild(text);
	if(orderExists) {
		list.replaceChild(order, oldOrder);
	} else {
		list.appendChild(order);
	}	
}

