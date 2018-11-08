#include "ArmyPiece.h"
#include "Graph.h"
#include "Territory.h"
#include "HoldMove.h"
#include "MovementMove.h"
#include "SupportMove.h"
#include "ConvoyMove.h"

using std::endl;

ArmyPiece::ArmyPiece(Nation nationality, string startingLocation) : Piece{nationality, startingLocation} {}
/*
vector<Move> ArmyPiece::findPossibleMoves(const Graph * territories) const {
	vector<Move> moves;
	unordered_set<string> neighbours = territories->getNeighbours(getLocation());
	for(string neighbourString : neighbours) {
		Territory * neighbour = territories->getTerritory(neighbourString);
		if(neighbour->getType() == Territory::Type::INLAND || neighbour->getType() == Territory::Type::COAST) {
			Move move{this, getLocation(), neighbourString, Move::MOVE};
			moves.push_back(move);
		}
	}
	moves.push_back(Move{this, getLocation(), getLocation(), Move::HOLD});
	return moves;
}
*/
bool ArmyPiece::isMovementValid(const MovementMove * move, Graph * graph) const {
	string destinationString = move->getDestination();
	if(getLocation() == destinationString) {
		return false;
	}
	Territory * destination = graph->getTerritory(destinationString);
	unordered_set<string> neighbours = graph->getArmyNeighbours(getLocation());
	bool isDestinationInNeighbours = neighbours.find(destinationString) != neighbours.end();

	return (destination->getType() == Territory::Type::COAST && graph->getTerritory(getLocation())->getType() == Territory::Type::COAST && move->getViaConvoy()) || isDestinationInNeighbours;
}

bool ArmyPiece::isSupportValid(const SupportMove * move, Graph * graph) const {
	string sourceString = move->getSource();
	string destinationString = move->getDestination();
	if(getLocation() == destinationString || getLocation() == sourceString) {
		return false;
	}
	unordered_set<string> neighbours = graph->getArmyNeighbours(getLocation());
	return neighbours.find(destinationString) != neighbours.end();
}

bool ArmyPiece::isConvoyValid(const ConvoyMove * move, Graph * graph) const {
	return false;
}

std::unordered_set<string> ArmyPiece::getNeighbours(Graph * graph) const {
	return graph->getArmyNeighbours(getLocation());
}

void ArmyPiece::print(ostream & out) const {
	out << "Piece at: " << getLocation() << " is a " << getNationality() << " ARMY" << endl;
}
