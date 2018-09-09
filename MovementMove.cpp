#include "MovementMove.h"
#include <ostream>
#include "Piece.h"

using std::string;

MovementMove::MovementMove(Piece * piece, string destination) : Move{piece}, destination_{destination} {}

void MovementMove::print(ostream & out) const {


}

void MovementMove::putIntoSet(unordered_set<HoldMove *> & holdMoves, unordered_set<MovementMove *> & movementMoves, unordered_set<SupportMove *> & supportMoves, unordered_set<ConvoyMove *> & convoyMoves) {
	movementMoves.insert(this);
}

bool MovementMove::isLegal(Graph * graph) const {
	return getPiece()->isMovementValid(this, graph);
}

void MovementMove::process(map<string, map<const Move *, float> > & attacks, 
			map<string, map<string, std::unordered_set<const SupportMove *> > > & supports, 
			map<string, map<string, string> > & convoys) const {
	auto it = attacks.find(destination_);
	std::pair<const Move *, float> pair = std::make_pair(this, 1.0);
	if(it == attacks.end()) {
		std::map<const Move *, float> map;
		map.insert(pair);
		attacks.insert(make_pair(destination_, map));
	} else {
		(it->second).insert(pair);
	}
}

string MovementMove::getDestination() const {
	return destination_;
}
