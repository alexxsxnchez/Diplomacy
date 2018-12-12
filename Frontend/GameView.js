function GameView() {

}

GameView.prototype.update = function(year, phase, counts, isFinalized) {
	document.getElementById('yearText').innerHTML = year;
	document.getElementById('phaseText').innerHTML = phase;
	this.updateStarsAndUnits(counts);
	this.updateFinalizeButton(isFinalized);
}

GameView.prototype.updateStarsAndUnits = function(counts) {
	document.getElementById('austria-stars').innerHTML = counts[Nation.AUSTRIA].stars + " Centers<br>";
	document.getElementById('england-stars').innerHTML = counts[Nation.ENGLAND].stars + " Centers<br>";
	document.getElementById('france-stars').innerHTML = counts[Nation.FRANCE].stars + " Centers<br>";
	document.getElementById('germany-stars').innerHTML = counts[Nation.GERMANY].stars + " Centers<br>";
	document.getElementById('italy-stars').innerHTML = counts[Nation.ITALY].stars + " Centers<br>";
	document.getElementById('russia-stars').innerHTML = counts[Nation.RUSSIA].stars + " Centers<br>";
	document.getElementById('turkey-stars').innerHTML = counts[Nation.TURKEY].stars + " Centers<br>";
	
	document.getElementById('austria-units').innerHTML = counts[Nation.AUSTRIA].units + " Units";
	document.getElementById('england-units').innerHTML = counts[Nation.ENGLAND].units + " Units";
	document.getElementById('france-units').innerHTML = counts[Nation.FRANCE].units + " Units";
	document.getElementById('germany-units').innerHTML = counts[Nation.GERMANY].units + " Units";
	document.getElementById('italy-units').innerHTML = counts[Nation.ITALY].units + " Units";
	document.getElementById('russia-units').innerHTML = counts[Nation.RUSSIA].units + " Units";
	document.getElementById('turkey-units').innerHTML = counts[Nation.TURKEY].units + " Units";
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
