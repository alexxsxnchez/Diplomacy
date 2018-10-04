#include "SupportMove.h"
#include <ostream>
#include "Piece.h"
#include "ArmyPiece.h"
#include "HoldMove.h"

using std::string;

SupportMove::SupportMove(Piece * piece, string source, string destination) : Move{piece}, source_{source}, destination_{destination} {}

void SupportMove::print(ostream & out) const {


}
		
void SupportMove::putIntoSet(unordered_set<HoldMove *> & holdMoves, unordered_set<MovementMove *> & movementMoves, unordered_set<SupportMove *> & supportMoves, unordered_set<ConvoyMove *> & convoyMoves) {
	supportMoves.insert(this);
}

bool SupportMove::isLegal(Graph * graph) const {
	return getPiece()->isSupportValid(this, graph);
}

bool SupportMove::determineSupportDecision(MoveProcessor & processor) {
	if(supportGiven_ != DecisionResult.UNDECIDED) {
		return true;
	}
	if(dislodged_) {
		supportGiven_ = DecisionResult.NO;
		return true;
	}
	std::unordered_set<string> & attacks = processor.getAttacks().at(this->getPiece()->getLocation());
	bool canStillBeGiven = true;
	for(const MovementMove * move : attacks) {
		if(move->getPiece()->getLocation() == destination_) {
			continue;
		}
		if(move->getAttackStrength().min > 0) {
			supportGiven_ = DecisionResult.NO;
			return true;
		}
		if(move->getAttackStrength().max > 0) {
			canStillBeGiven = false;
		}
	}
	if(canStillBeGiven) {
		supportGiven_ = DecisionResult.YES;
		return true;
	}
	return false;
}

bool SupportMove::process(MovementProcessor & processor) {

	calculateHoldStrength(processor);
	bool dislodgedDetermined = determineDislodgeDecision(processor);
	bool supportDetermined = determineSupportDecision(processor);
	
	return supportDetermined && dislodgedDetermined;
}

/*
void SupportMove::process(map<string, map<const Move *, float> > & attacks, 
			map<string, map<string, std::unordered_set<const SupportMove *> > > & supports, 
			map<string, map<string, string> > & convoys) const {
	
	/*
	// if not attacked, then add support functionality
	auto it = attacks.find(getPiece()->getLocation());

// PROBLEMS -- support can still be cut if only attack is from destination. but now instead of merely being attacked, if the support is dislodged, the support is only then cut

// possible solution: when attack from destination is detected, move supportmove into a queue, and process after all other supports are processed


	bool isAttackedFromNotDestination = false;
	if(it != attacks.end()) {
		for(auto it2 : it->second) {
			if(it2.first->getPiece()->getLocation() != destination_ && it2.first->getPiece()->getLocation() != getPiece()->getLocation()) {
				isAttackedFromNotDestination = true;
				break;		
			}
		}
	}
	// PROBLEMS -- should be good now
	if(it == attacks.end() || !isAttackedFromNotDestination) {
		it = attacks.find(destination_);
		if(it == attacks.end()) {
			// could be possibly defending another supporting unit that has not been processed yet
			std::map<const Move *, float> map;
			// insert temporary 
			map.insert(std::make_pair(new HoldMove(new ArmyPiece(Nation::ENGLAND, source_)), -1.0));
			attacks.insert(std::make_pair(destination_, map));
		} else {
			for(auto it2 = (it->second).begin(); it2 != (it->second).end(); it2++) {
				if(it2->first->getPiece()->getLocation() == source_) {
					if(it2->second < 0) {
						it2->second -= 1.0;
					} else {
						it2->second += 1.0;
					}
					break;
				}
			}
		}
	}
	
	
	auto itS = supports.find(destination_);
	if(itS == supports.end()) {
		std::map<string, std::unordered_set<const SupportMove *> > supportsToDestination;
		std::unordered_set<const SupportMove *> setOfMoves;
		setOfMoves.insert(this);
		supportsToDestination.insert(std::make_pair(source_, setOfMoves));
		supports.insert(std::make_pair(destination_, supportsToDestination));
	} else {
		auto it2 = itS->second.find(source_);
		if(it2 == itS->second.end()) {
			std::unordered_set<const SupportMove *> setOfMoves;
			setOfMoves.insert(this);
			itS->second.insert(std::make_pair(source_, setOfMoves));
		} else {
			//auto supportsFromSource = supports.at(source_); do I need this??? 
			it2->second.insert(this);
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

string SupportMove::getSource() const {
	return source_;
}

string SupportMove::getDestination() const {
	return destination_;
}

DecisionResult SupportMove::getSupportGiven() const {
	return supportGiven_;
}