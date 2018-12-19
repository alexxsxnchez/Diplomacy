#include "SupportMove.h"
#include <iostream>
#include "Piece.h"
#include "ArmyPiece.h"
#include "HoldMove.h"
#include "MovementMove.h"

using std::string;

SupportMove::SupportMove(Piece * piece, string source, string destination, string coastSpecifier) : Move{piece}, source_{source}, destination_{destination}, coast_{coastSpecifier} {}

void SupportMove::print(ostream & out) const {
	out << *getPiece() << " SUPPORTS " << source_ << " to " << destination_ << std::endl;
}

void SupportMove::calculateIsLegal(Graph * graph) {
	isLegal_ = getPiece()->isSupportValid(this, graph);
}

bool SupportMove::isPartOfParadoxCore(MoveProcessor * processor) const {
	return true;
}

Move * SupportMove::getParadoxDependency(MoveProcessor * processor) const {
	MoveProcessor::AttackMap attacks = processor->getAttacks();
	auto it = attacks.find(getPiece()->getLocation());
	for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
//		std::cerr << "ok: " << (*it2)->getPiece()->getLocation() << std::endl;
		//if((*it2)->getMoveDecision() == UNDECIDED) {
			return *it2;
		//}
	}
	return nullptr;
}

void SupportMove::settleParadox(bool isParadoxCore) {
	//TODO
};


void SupportMove::forceFail() {
	supportGiven_ = NO;
	description_ = "Illegal move. " + getPiece()->getLocation() + " cannot support any unit to " + destination_ + ".";
}





// not critical TODO: currently says support is successful when there is no unit to receive the support

bool SupportMove::determineSupportDecision(MoveProcessor & processor) {
	//std::cerr << "about to process support decision" << std::endl;
	if(supportGiven_ != UNDECIDED) {
		return false;
	}
	if(dislodged_ == YES) {
		supportGiven_ = NO;
		return true;
	}
	bool canStillBeGiven = true;
	try {
		std::unordered_set<MovementMove *> attacks = processor.getAttacks().at(this->getPiece()->getLocation());
		for(MovementMove * move : attacks) {
			//std::cerr << "Attack on support at " << getPiece()->getLocation() << " from " << move->getPiece()->getLocation() << " while destination is " << destination_ << std::endl;
			if(move->getPiece()->getLocation() == destination_) {
				continue;
			}
			//std::cerr << move->getPiece()->getLocation() << ": attackStrength: " << move->getAttackStrength().min << " to " << move->getAttackStrength().max << std::endl;
			if(move->getAttackStrength().min > 0) {
				supportGiven_ = NO;
				std::cerr << "Support given is NO ";
				return true;
			}
			if(move->getAttackStrength().max > 0) {
				canStillBeGiven = false;
				// add move's attack strength to dependencies
			}
		}
	} catch(std::out_of_range) {}
	if(canStillBeGiven && dislodged_ == NO) {
	
		supportGiven_ = YES;
		std::cerr << "Support given is YES ";
		return true;
	}
	return false;
}

bool SupportMove::process(MoveProcessor & processor) {

	bool holdStrengthUpdated = calculateHoldStrength(processor);
	bool dislodgedUpdated = determineDislodgeDecision(processor);
	bool supportUpdated = determineSupportDecision(processor);
	return supportUpdated || dislodgedUpdated || holdStrengthUpdated;
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

string SupportMove::getCoast() const {
	return coast_;
}

DecisionResult SupportMove::getSupportDecision() const {
	return supportGiven_;
}

bool SupportMove::isCompletelyDecided() const {
	return dislodged_ != UNDECIDED && supportGiven_ != UNDECIDED;
}