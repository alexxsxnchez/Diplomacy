#include "HoldMove.h"
#include <iostream>
#include "Piece.h"

HoldMove::HoldMove(Piece * piece) : Move{piece} {}

void HoldMove::print(ostream & out) const {


}
		
void HoldMove::putIntoSet(unordered_set<HoldMove *> & holdMoves, unordered_set<MovementMove *> & movementMoves, unordered_set<SupportMove *> & supportMoves, unordered_set<ConvoyMove *> & convoyMoves) {
	holdMoves.insert(this);
}

bool HoldMove::isLegal(Graph * graph) const {
	return getPiece()->isHoldValid(this, graph);
}

void HoldMove::process(map<string, map<const Move *, float> > & attacks, 
			map<string, map<string, std::unordered_set<const SupportMove *> > > & supports, 
			map<string, map<string, string> > & convoys) const {
	auto it = attacks.find(getPiece()->getLocation());
	std::pair<const Move *, float> pair = std::make_pair(this, 1.5);
	if(it == attacks.end()) {
		std::cout << "new map used" << std::endl;
		std::map<const Move *, float> map;
		map.insert(pair);
		attacks.insert(make_pair(getPiece()->getLocation(), map));
	} else {
		(it->second).insert(pair);
	}
}
