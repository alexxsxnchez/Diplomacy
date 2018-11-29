#include "ConvoyMove.h"
#include <iostream>
#include "Piece.h"
#include "MovementMove.h"

using std::string;
using std::map;
using std::pair;

ConvoyMove::ConvoyMove(Piece * piece, string source, string destination) : Move{piece}, source_{source}, destination_{destination} {}

void ConvoyMove::print(ostream & out) const {
	out << *getPiece() << " CONVOYS " << source_ << " to " << destination_ << std::endl;
}

bool ConvoyMove::isLegal(Graph * graph) const {
	return getPiece()->isConvoyValid(this, graph);
}

bool ConvoyMove::process(MoveProcessor & processor) {
	bool holdStrengthUpdated = calculateHoldStrength(processor);
	bool dislodgedUpdated = determineDislodgeDecision(processor);
	return dislodgedUpdated || holdStrengthUpdated;
}


bool ConvoyMove::isPartOfParadoxCore(MoveProcessor * processor) const {
	return true;
}

Move * ConvoyMove::getParadoxDependency(MoveProcessor * processor) const {
	MoveProcessor::AttackMap attacks = processor->getAttacks();
	auto it = attacks.find(getPiece()->getLocation());
	for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
		if((*it2)->getMoveDecision() == UNDECIDED) {
			return *it2;
		}
	}
	return nullptr;
}

void ConvoyMove::settleParadox(bool isParadoxCore) {
	//TODO
};

void ConvoyMove::forceFail() {
	isLegal_ = false;
	description_ = "Illegal move. " + getPiece()->getLocation() + " cannot convoy any unit from " + source_ + " to " + destination_ + ".";
}



/*void ConvoyMove::process(map<string, map<const Move *, float> > & attacks, 
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
*/

string ConvoyMove::getSource() const {
	return source_;
}

string ConvoyMove::getDestination() const {
	return destination_;
}

bool ConvoyMove::getIsLegal() const {
	return isLegal_;
}

bool ConvoyMove::isCompletelyDecided() const {
//	std::cerr << "convoy dislodged?: " << dislodged_ << std::endl;
	return dislodged_ != UNDECIDED;
}
