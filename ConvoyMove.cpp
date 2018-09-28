#include "ConvoyMove.h"
#include <ostream>
#include "Piece.h"

using std::string;
using std::map;
using std::pair;

ConvoyMove::ConvoyMove(Piece * piece, string source, string destination) : Move{piece}, source_{source}, destination_{destination} {}

void ConvoyMove::print(ostream & out) const {


}

void ConvoyMove::putIntoSet(unordered_set<HoldMove *> & holdMoves, unordered_set<MovementMove *> & movementMoves, unordered_set<SupportMove *> & supportMoves, unordered_set<ConvoyMove *> & convoyMoves) {
	convoyMoves.insert(this);
}

bool ConvoyMove::isLegal(Graph * graph) const {
	return getPiece()->isConvoyValid(this, graph);
}

void ConvoyMove::process(map<string, map<const Move *, float> > & attacks, 
			map<string, map<string, std::unordered_set<const SupportMove *> > > & supports, 
			map<string, map<string, std::unordered_set<const ConvoyMove *> > > & convoys) const {
			
	auto itC = convoys.find(destination_);
	if(itC == convoys.end()) {
		std::map<string, std::unordered_set<const ConvoyMove *> > convoysToDestination;
		std::unordered_set<const ConvoyMove *> setOfMoves;
		setOfMoves.insert(this);
		convoysToDestination.insert(std::make_pair(source_, setOfMoves));
		convoys.insert(std::make_pair(destination_, convoysToDestination));
	} else {
		auto it2 = itC->second.find(source_);
		if(it2 == itC->second.end()) {
			std::unordered_set<const ConvoyMove *> setOfMoves;
			setOfMoves.insert(this);
			itC->second.insert(std::make_pair(source_, setOfMoves));
		} else {
			//auto supportsFromSource = supports.at(source_);
			itC->second.insert(this);
		}
	}
			
	// act as hold move
	auto itA = attacks.find(getPiece()->getLocation());
	std::pair<const Move *, float> pair = std::make_pair(this, 1.5);
	if(itA == attacks.end()) {
		std::map<const Move *, float> map;
		map.insert(pair);
		attacks.insert(std::make_pair(getPiece()->getLocation(), map));
	} else {
		itA->second.insert(pair);
	}
}

string ConvoyMove::getSource() const {
	return source_;
}

string ConvoyMove::getDestination() const {
	return destination_;
}
