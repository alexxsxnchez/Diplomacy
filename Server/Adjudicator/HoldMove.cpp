#include "HoldMove.h"
#include <iostream>
#include "Piece.h"

HoldMove::HoldMove(Piece * piece) : Move{piece} {}

void HoldMove::print(ostream & out) const {
	out << *getPiece() << " HOLDS" << std::endl;
}

void HoldMove::calculateIsLegal(Graph * graph) {
	isLegal_ = getPiece()->isHoldValid(this, graph);
}

bool HoldMove::process(MoveProcessor & processor) {
	bool holdStrengthUpdated = calculateHoldStrength(processor);
	bool dislodgedUpdated = determineDislodgeDecision(processor);
	return dislodgedUpdated || holdStrengthUpdated;
}

bool HoldMove::isPartOfParadoxCore(MoveProcessor * processor) const {
	return true;
}

Move * HoldMove::getParadoxDependency(MoveProcessor * processor) const {
	return nullptr;
}

void HoldMove::settleParadox(bool isParadoxCore) {
	//TODO
};

void HoldMove::forceFail() {}


/*
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
*/

bool HoldMove::isCompletelyDecided() const {
	return dislodged_ != UNDECIDED;
}