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
	Territory * destination = graph->getTerritory(destinationString);
	
	
	string currentLocation = coast_ == "" ? getLocation() : getLocation() + "_" + coast_;
	if(currentLocation == destinationString || getLocation() == destinationString) {
		return false;
	}

	unordered_set<string> neighbours = graph->getFleetNeighbours(currentLocation);
	if(destinationString == "StPetersburg" || destinationString == "Spain" || destination == "Bulgaria") {
		string coastSpecifier = move->getCoastSpecifier();
		if(destinationString == "Bulgaria") {
			if(coastSpecifier != "EC" && coastSpecifier != "SC") {
				return false; // invalid coast
			}
		} else {
			if(coastSpecifier != "NC" && coastSpecifier != "SC") {
				return false; // invalid coast
			}
		}
		
	}
	
	return neighbours.find(destinationString) != neighbours.end();
}

bool FleetPiece::isSupportValid(const SupportMove * move, Graph * graph) const {
	string sourceString = move->getSource();
	string destinationString = move->getDestination();
	//string currentLocation = coast_ == "" ? getLocation() : getLocation() + "_" + coast_;
	string currentLocation = getLocation(); // don't include coast
	if(currentLocation == destinationString || currentLocation == sourceString) {
		return false;
	}
	unordered_set<string> neighbours = graph->getFleetNeighbours(currentLocation);
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
	return graph->getFleetNeighbours(getLocation());
}

void FleetPiece::print(ostream & out) const {
	out << "Piece at: " << getLocation();
	if(coast_ != "") {
 		out << " " << coast_;
	}
	out << " is a " << getNationality() << " FLEET" << endl;
}
