#include "FleetPiece.h"
#include "Graph.h"
#include "Territory.h"
#include "HoldMove.h"
#include "MovementMove.h"
#include "SupportMove.h"
#include "ConvoyMove.h"

using std::endl;

FleetPiece::FleetPiece(Nation nationality, string startingLocation, string coastSpecifier) : Piece{nationality, startingLocation}, coast_{coastSpecifier} {}

/*vector<Move> FleetPiece::findPossibleMoves(const Graph * territories) const {
	vector<Move> moves;
	unordered_set<string> neighbours = territories->getNeighbours(getLocation());
	for(string neighbourString : neighbours) {
		Territory * neighbour = territories->getTerritory(neighbourString);
		if(neighbour->getType() == Territory::Type::WATER || neighbour->getType() == Territory::Type::COAST) {
			Move move{this, getLocation(), neighbourString, Move::MOVE};
			moves.push_back(move);
		}
	}
	moves.push_back(Move{this, getLocation(), getLocation(), Move::HOLD});
	return moves;
}*/

bool FleetPiece::isMovementValid(const MovementMove * move, Graph * graph) const {
	if(move->getViaConvoy()) {
		return false;
	}
	string destinationString = move->getDestination();
	
	
	string currentLocation = coast_ == "" ? getLocation() : getLocation() + "_" + coast_;
	if(currentLocation == destinationString || getLocation() == destinationString) {
		return false;
	}

	unordered_set<string> neighbours = graph->getFleetNeighbours(currentLocation); // with coasts
	
	if(destinationString == "StPetersburg" || destinationString == "Spain" || destinationString == "Bulgaria") {
		string coastSpecifier = move->getCoast();
		if(destinationString == "Bulgaria") {
			if(coastSpecifier != "EC" && coastSpecifier != "SC") {
				return false; // invalid coast
			}
		} else {
			if(coastSpecifier != "NC" && coastSpecifier != "SC") {
				return false; // invalid coast
			}
		}
		destinationString = destinationString + "_" + coastSpecifier;
	}
	
	return neighbours.find(destinationString) != neighbours.end();
}

bool FleetPiece::isSupportValid(const SupportMove * move, Graph * graph) const {
	string sourceString = move->getSource();
	string destinationString = move->getDestination();
	string destinationCoast = move->getCoast();
	string locationWithCoast = coast_ == "" ? getLocation() : getLocation() + "_" + coast_;
	//string currentLocation = getLocation(); // don't include coast
	if(getLocation() == destinationString || getLocation() == sourceString) {
		return false;
	}
	
	unordered_set<string> neighbours = graph->getFleetNeighbours(locationWithCoast); // with coasts
	if(coast_ == "") {
		std::unordered_set<string> doubleCoastNeighbours = graph->getDoubleCoastNeighbours(getLocation());
		for(string doubleCoastNeighbour : doubleCoastNeighbours) {
			neighbours.insert(doubleCoastNeighbour);
		}
	}
	return neighbours.find(destinationString) != neighbours.end();
}

bool FleetPiece::isConvoyValid(const ConvoyMove * move, Graph * graph) const {
	string sourceString = move->getSource();
	string destinationString = move->getDestination();
	if(getLocation() == destinationString || getLocation() == sourceString) {
		return false;
	}
	Territory * location = graph->getTerritory(getLocation());
	Territory * source = graph->getTerritory(sourceString);
	Territory * destination = graph->getTerritory(destinationString);
	return source->getType() == Territory::Type::COAST && destination->getType() == Territory::Type::COAST && location->getType() == Territory::Type::WATER;
}

std::unordered_set<string> FleetPiece::getNeighbours(Graph * graph) const {
	string locationWithCoast = coast_ == "" ? getLocation() : getLocation() + "_" + coast_;
	return graph->getFleetNeighbours(locationWithCoast);
}

void FleetPiece::determineConvoyStatus(MovementMove * move, Graph * graph) {
	move->setConvoyStatus(false);
}

string FleetPiece::getCoast() const {
	return coast_;
}

void FleetPiece::print(ostream & out) const {
	out << "Piece at: " << getLocation();
	if(coast_ != "") {
 		out << " " << coast_;
	}
	out << " is a " << getNationality() << " FLEET" << endl;
}
