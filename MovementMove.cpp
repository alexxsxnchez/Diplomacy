#include "MovementMove.h"
#include <ostream>
#include "Piece.h"

using std::string;

MovementMove::MovementMove(Piece * piece, string destination, bool viaConvoy) : Move{piece}, destination_{destination}, viaConvoy_{viaConvoy}, isValid_{true} {}

void MovementMove::print(ostream & out) const {


}

void MovementMove::putIntoSet(unordered_set<HoldMove *> & holdMoves, unordered_set<MovementMove *> & movementMoves, unordered_set<SupportMove *> & supportMoves, unordered_set<ConvoyMove *> & convoyMoves) {
	movementMoves.insert(this);
}

bool MovementMove::isLegal(Graph * graph) const {
	return getPiece()->isMovementValid(this, graph);
}

void MovementMove::process(map<string, map<const Move *, float> > & attacks,
			map<string, map<const MovementMove *, float> > & attacksViaConvoy, 
			map<string, map<string, std::unordered_set<const SupportMove *> > > & supports, 
			map<string, map<string, string> > & convoys) const {
			
	if(viaConvoy_) {
		auto it = attacksViaConvoy.find(destination_);
		std::pair<const MovementMove *, float> pair = std::make_pair(this, 1.0);
		if(it == attacksViaConvoy.end()) {
			std::map<const MovementMove *, float> map;
			map.insert(pair);
			attacksViaConvoy.insert(make_pair(destination_, map));
		} else {
			(it->second).insert(pair);
		}
		return;
	}
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

bool MovementMove::calculateAttackStrength(MovementProcessor & processor) {
	if(!hasPath_ == DecisionResult.YES) {
		if(hasPath_ == DecisionResult.NO) {
			attackStrength_.max = 0;
		}
		attackStrength_.min = 0;
	} else {
		string source = getPiece()->getLocation();
		Piece * defender = nullptr;
		Piece * leavingDefender = nullptr;
		auto it = processor.getHolds().find(destination_);
		if(it != nullptr) {
			defender = (*it)->getPiece();
		} else {
			auto it2 = processor.getAttacks().find(std::make_pair(destination_, source));
			if(it2 != nullptr) {
				defender = (*it2)->getPiece();
			} else {
				auto it3 = processor.getMoves().find(destination_);
				if(it3 != nullptr && (*it3)->getPiece()->getMoveDecision() != DecisionResult.YES) {
					leavingDefender = (*it3)->getPiece();
				}
			}
		}
		
		if(defender != nullptr || leavingDefender != nullptr) {
			Piece * same;
			if(defender == nullptr) {
				same = leavingDefender;
			} else {
				same = defender;
			}
			if(same->getNationality() == this->getNationality()) {
				attackStrength_.min = 0;
				if(leavingDefender == nullptr || leavingDefender->getMoveDecision() == DecisionResult.NO) {
					attackStrength_.max = 0;
				}
			} else {
				attackStrength_.min = 1 + processor.calculateSupportStrength(source, destination_, true, same->getNationality());
				if(leavingDefender == nullptr || leavingDefender->getMoveDecision() == DecisionResult.NO) {
					attackStrength_.max = 1 + processor.calculateSupportStrength(source, destination_, false, same->getNationality());
				}			
			}
		} else {
			attackStrength_.min = 1 + processor.calculateSupportStrength(source, destination_, true); // true is only given
			attackStrength_.max = 1 + processor.calculateSupportStrength(source, destination_, false);
		}
	}
}

void MovementMove::calculatePreventStrength(MovementProcessor & processor) {
	if(!hasPath_ == DecisionResult.YES) {
		if(hasPath_ == DecisionResult.NO) {
			preventStrength_.max = 0;
		}
		preventStrength_.min = 0;
	} else {
		string source = getPiece()->getLocation();
		auto it = processor.getAttacks().find(std::make_pair(destination_, source);
		if(it != nullptr) {
			Piece * headOnDefender = (*it)->getPiece();
			if(headOnDefender->getMoveDecision() != DecisionResult.NO) {
				preventStrength_.min = 0;
				if(headOnDefender->getMoveDecision() == DecisionResult.YES) {
					preventStrength_.max = 0;
				}
				return;
			}
		}
		preventStrength_.min = 1 + processor.calculateSupportStrength(source, destination_, true);
		preventStrength_.max = 1 + processor.calculateSupportStrength(source, destination_, false);
	}
}

void MovementMove::calculateDefendStrength(MovementProcessor & processor) {
	defendStrength_.min = 1 + processor.calculateSupportStrength(getPiece()->getLocation(), destination_, true);
	defendStrength_.max = 1 + processor.calculateSupportStrength(getPiece()->getLocation(), destination_, false);
}

void MovementMove::calculateHoldStrength(MovementProcessor & processor) override {
	switch(moved_) {
		case(DecisionResult.YES):
			holdStrength_.max = 0;
			holdStrength_.min = 0;
			break;
		case(DecisionResult.UNDECIDED):
			holdStrength_.min = 0;
			holdStrength_.max = 1;
			break;
		case(DecisionResult.NO):
			holdStrength_.min = 1;
			holdStrength_.max = 1;
			break;
	}
}

bool MovementMove::determineMoveDecision(MoveProcessor & processor) {
	if(moved_ != DecisionResult.UNDECIDED) {
		return true;
	}
	auto it = processor.getAttacks().find(destination_);
	unsigned int highestPreventStrengthMax = 0;
	unsigned int highestPreventStrengthMin = 0;
	for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
		unsigned int preventStrengthMax = (*it2)->getPreventStrength().max;
		unsigned int preventStrengthMin = (*it2)->getPreventStrength().min;
		if(preventStrengthMax > highestPreventStrengthMax) {
			highestPreventStrengthMax = preventStrengthMax;
		}
		if(preventStrengthMin > highestPreventStrengthMin) {
			highestPreventStrengthMin = preventStrengthMin;
		}
	}
	
	if(attackStrength_.max <= highestPreventStrengthMin) {
		moved_ = DecisionResult.NO;
		return true;
	}
	
	bool couldBeMoved = attackStrength_.min > highestPreventStrengthMax;

	MovementMove * headOnDefenderMove = nullptr;
	it = processor.getAttacks().find(getPiece()->getLocation());
	if(it != nullptr) {
		auto it2 = it->second.find(destination_);
		if(it2 != nullptr) {
			headOnDefenderMove = *it2;
		}
	}
	if(headOnDefenderMove != nullptr) {
		couldBeMoved = couldBeMoved && attackStrength_.min > headOnDefenderMove->getDefendStrength().max;
		if(couldBeMoved) {
			moved_ = DecisionResult.YES;
			return true;
		} else if(attackStrength_.max <= headOnDefenderMove->getDefendStrength.min) {
			moved_ = DecisionResult.NO;
			return true;
		}
		return false;
	} else {
		auto holdIt = processor.getHolds().find(destination_);
		if(holdIt != nullptr) {
			couldBeMoved = couldBeMoved && attackStrength_.min > (*holdIt)->getHoldStrength().max;
			if(couldBeMoved) {
				moved_ = DecisionResult.YES;
				return true;
			} else if(attackStrength_.max <= (*holdIt)->getHoldStrength().min) {
				moved_ = DecisionResult.NO;
				return true;
			}
			return false;
		} else {
			if(couldBeMoved) {
				moved_ = DecisionResult.YES;
				return true;
			}
			return false;
		}
	}
}

bool MovementMove::determinePathDecision() {
	if(hasPath_ != DecisionResult.UNDECIDED) {
		return true;
	}
	if(!viaConvoy_) {
		hasPath = DecisionResult.YES;
		return true;
	}
	
	
}

bool MovementMove::reachesPath(MoveProcessor & processor, string currentSource) {
	try {
		unordered_set<const ConvoyMove *> & convoys = processor.getConvoys().at(destination_).at(this->getPiece()->getLocation());
		const unordered_set<string> neighbours = processor.getMap().getFleetNeighbours(currentSource);
		
	} catch(std::out_of_range) {
	
	}
}

bool MovmementMove::determineDislodgeDecision(MoveProcessor & processor) override {
	if(dislodged_ != DecisionResult.UNDECIDED) {
		return true;
	}
	if(moved_ == DecisionResult.YES) {
		dislodged_ = DecisionResult.NO;
		return true;
	}
	auto it = processor.getAttacks().find(destination_);
	bool canBecomeSustained = true;
	if(it != nullptr) {
		for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			const MovementMove * move = *it2;
			if(move->getMoveDecision() != DecisionResult.NO) {
				canBecomeSustained = false;
				if(move->getMoveDecision() == DecisionResult.YES && moved_ == DecisionResult.NO) {
					dislodged_ = DecisionResult.YES;
					return true;
				}
			}
		}
	} else if(canBecomeSustained) {
		dislodged_ = DecisionResult.NO;
		return true;
	}
	return false;
}

bool MovementMove::process(MovementProcessor & processor) {
	bool pathDetermined = determinePathDecision(processor);
	calculateAttackStrength(processor);
	calculatePreventStrength(processor);
	calculateDefendStrength(processor);
	bool moveDetermined = determineMoveDecision(processor);
	calculateHoldStrength(processor);
	bool dislodgedDetermined = determineDislodgeDecision(processor);
	
	return pathDetermined && moveDetermined && dislodgedDetermined);
}

string MovementMove::getDestination() const {
	return destination_;
}

bool MovementMove::getViaConvoy() const {
	return viaConvoy_;
}
