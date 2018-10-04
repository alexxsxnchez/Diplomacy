#include "MovementMove.h"
#include <ostream>
#include "Piece.h"

using std::string;

MovementMove::MovementMove(Piece * piece, string destination, bool viaConvoy) : Move{piece}, destination_{destination}, viaConvoy_{viaConvoy} {}

void MovementMove::print(ostream & out) const {


}

void MovementMove::putIntoSet(unordered_set<HoldMove *> & holdMoves, unordered_set<MovementMove *> & movementMoves, unordered_set<SupportMove *> & supportMoves, unordered_set<ConvoyMove *> & convoyMoves) {
	movementMoves.insert(this);
}

bool MovementMove::isLegal(Graph * graph) const {
	return getPiece()->isMovementValid(this, graph);
}
/*
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
*/
void MovementMove::calculateAttackStrength(MovementProcessor & processor) {

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

	// first min
	if(hasPath_ != DecisionResult.YES) {
		attackStrength_.min = 0;
	} else {
		if(defender != nullptr || leavingDefender != nullptr) {
			Piece * same;
			if(defender == nullptr) {
				same = leavingDefender;
			} else {
				same = defender;
			}
			if(same->getNationality() == this->getNationality()) {
				attackStrength_.min = 0;
			} else {
				attackStrength_.min = 1 + processor.calculateSupportStrength(source, destination_, true, same->getNationality());			
			}
		} else {
			attackStrength_.min = 1 + processor.calculateSupportStrength(source, destination_, true); // true is only given
		}
	
	}

	// now max
	if(hasPath_ == DecisionResult.NO) {
			attackStrength_.max = 0;
	} else {
		if(leavingDefender != nullptr && leavingDefender->getMoveDecision() != DecisionResult.NO) {
			leavingDefender = nullptr;
		}
		
		if(defender != nullptr || leavingDefender != nullptr) {
			Piece * same;
			if(defender == nullptr) {
				same = leavingDefender;
			} else {
				same = defender;
			}
			if(same->getNationality() == this->getNationality()) {
				attackStrength_.max = 0;
			} else {
				attackStrength_.max = 1 + processor.calculateSupportStrength(source, destination_, false, same->getNationality());			
			}
		} else {
			attackStrength_.max = 1 + processor.calculateSupportStrength(source, destination_, false); // true is only given
		}
	}
}

void MovementMove::calculatePreventStrength(MovementProcessor & processor) {

	// first min
	if(hasPath_ != DecisionResult.YES) {
		preventStrength_.min = 0;
	} else {
		string source = getPiece()->getLocation();
		auto it = processor.getAttacks().find(std::make_pair(destination_, source);
		bool headOnWon = false;
		if(it != nullptr) {
			Piece * headOnDefender = (*it)->getPiece();
			if(headOnDefender->getMoveDecision() != DecisionResult.NO) {
				preventStrength_.min = 0;
				headOnWon = true;
			}
		}
		if(!headOnWon) {
			preventStrength_.min = 1 + processor.calculateSupportStrength(source, destination_, true);
		}
	}
	
	// now max
	if(hasPath_ == DecisionResult.NO) {
		preventStrength_.max = 0;
	} else {
		string source = getPiece()->getLocation();
		auto it = processor.getAttacks().find(std::make_pair(destination_, source);
		bool headOnWon = false;
		if(it != nullptr) {
			Piece * headOnDefender = (*it)->getPiece();
			if(headOnDefender->getMoveDecision() == DecisionResult.YES) {
				preventStrength_.max = 0;
				headOnWon = true;
			}
		}
		if(!headOnWon) {
			preventStrength_.max = 1 + processor.calculateSupportStrength(source, destination_, false);
		}
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

bool MovementMove::determinePathDecision(MoveProcessor & processor) {
	if(hasPath_ != DecisionResult.UNDECIDED) {
		return true;
	}
	if(!viaConvoy_) {
		hasPath_ = DecisionResult.YES;
		return true;
	}
	std::unordered_set<string> alreadySearched;
	hasPath_ = reachesPath(processor, this->getPiece()->getLocation(), alreadySearched, true);
	return hasPath_ != DecisionResult.UNDECIDED;
}

DecisionResult MovementMove::reachesPath(MoveProcessor & processor, string currentSource, std::unordered_set<string> & alreadySearched, bool firstSearch = false) {
	DecisionResult currentBest = DecisionResult.NO;
	try {
		std::unordered_set<const ConvoyMove *> & convoys = processor.getConvoys().at(destination_).at(this->getPiece()->getLocation());
		const std::unordered_set<string> neighbours = processor.getMap().getFleetNeighbours(currentSource);
		for(string neighbour : neighbours) {
			if(neighbour == destination_) {
				if(firstSearch) {
					continue;
				}
				return DecisionResult.YES;
			}
		
			if(alreadySearched.count(neighbour) != 0) {
				continue;
			}
			bool found = false;
			for(const ConvoyMove * move : convoys) {
				if(move->getPiece()->getLocation() == neighbour) {
				 	if(move->getDislodgeDecision() != DecisionResult.YES) {
						found = true;
						//currentBest = DecisionResult.UNDECIDED;
						if(move->getDislodgeDecision() == DecisionResult.UNDECIDED) {
							currentBest = DecisionResult.UNDECIDED;
						}
					}
					alreadySearched.insert(neighbour);
					break;
				}
			}
			if(!found) {
				continue;
			}
			
			DecisionResult futurePath = reachesPath(processor, neighbour, alreadySearched);
			if(futurePath == DecisionResult.UNDECIDED) {
				currentBest = DecisionResult.UNDECIDED);
			} else if(futurePath == DecisionResult.YES && move->getDislodgeDecision() == DecisionResult.NO) {
				return DecisionResult.YES;
			}
		}
	} catch(std::out_of_range) {}
	return currentBest;
}

bool MovmementMove::determineDislodgeDecision(MoveProcessor & processor) override {
	if(dislodged_ != DecisionResult.UNDECIDED) {
		return true;
	}
	if(moved_ == DecisionResult.YES) {
		dislodged_ = DecisionResult.NO;
		return true;
	}
	auto it = processor.getAttacks().find(this->getPiece()->getLocation());
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
	} else {
		dislodged_ = DecisionResult.NO;
		return true;
	}
	
	if(canBecomeSustained) {
		dislodged_ = DecisionResult.YES;
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
	
	return pathDetermined && moveDetermined && dislodgedDetermined;
}

string MovementMove::getDestination() const {
	return destination_;
}

bool MovementMove::getViaConvoy() const {
	return viaConvoy_;
}

DecisionResult MovementMove::getMoveDecision() const {
	return moved_;
}

DecisionResult MovementMove::getPathDecision() const {
	return hasPath_;
}

Strength MovementMove::getAttackStrength() const {
	return attackStrength_;
}
		
Strength MovementMove::getPreventStrength() const {
	return preventStrength_;
}

Strength MovementMove::getDefendStrength() const {
	return defendStrength_;
}
