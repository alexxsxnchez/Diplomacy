#include "MovementMove.h"
#include "ConvoyMove.h"
#include <ostream>
#include "Piece.h"
#include <unordered_set>
#include "Graph.h"
#include <iostream>

using std::string;

MovementMove::MovementMove(Piece * piece, string destination, bool viaConvoy) : Move{piece}, destination_{destination}, viaConvoy_{viaConvoy} {}

void MovementMove::print(ostream & out) const {


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
void MovementMove::calculateAttackStrength(MoveProcessor & processor) {
	std::cout << "ok" << std::endl;
	string source = getPiece()->getLocation();
	Piece * defender = nullptr;
	MovementMove * leavingDefender = nullptr;
	auto it = processor.getNonAttacks().find(destination_);
	if(it != processor.getNonAttacks().end()) {
		defender = it->second->getPiece();
	} else {
		auto it2 = processor.getAttacks().find(source);
		if(it2 != processor.getAttacks().end()) {
			for(auto it3 = it2->second.begin(); it3 != it2->second.end(); it3++) {
				if((*it3)->getPiece()->getLocation() == destination_) {
					defender = (*it3)->getPiece();
					break;
				}
			}
		} else {
			for(auto it3 = processor.getAttacks().begin(); it3 != processor.getAttacks().end(); it3++) {
				for(auto it4 = it3->second.begin(); it4 != it3->second.end(); it4++) { 
					if((*it4)->getPiece()->getLocation() == destination_ && (*it4)->getMoveDecision() != YES) {
						std::cout << "YESSS" << std::endl;
						leavingDefender = *it4;
						break;
					}
				}
			}
		}
	}
	
	std::cout << "NOOO" << std::endl;

	// first min
	if(hasPath_ != YES) {
		attackStrength_.min = 0;
	} else {
		if(defender != nullptr || leavingDefender != nullptr) {
			Piece * same;
			if(defender == nullptr) {
				same = leavingDefender->getPiece();
			} else {
				same = defender;
			}
			if(same->getNationality() == this->getPiece()->getNationality()) {
				attackStrength_.min = 0;
			} else {
				attackStrength_.min = 1 + processor.calculateSupportStrength(source, destination_, true, same->getNationality());			
			}
		} else {
			attackStrength_.min = 1 + processor.calculateSupportStrength(source, destination_, true); // true is only given
		}
	
	}

	// now max
	if(hasPath_ == NO) {
			attackStrength_.max = 0;
	} else {
		if(leavingDefender != nullptr && leavingDefender->getMoveDecision() != NO) {
			leavingDefender = nullptr;
		}
		
		if(defender != nullptr || leavingDefender != nullptr) {
			Piece * same;
			if(defender == nullptr) {
				same = leavingDefender->getPiece();
			} else {
				same = defender;
			}
			if(same->getNationality() == this->getPiece()->getNationality()) {
				attackStrength_.max = 0;
			} else {
				attackStrength_.max = 1 + processor.calculateSupportStrength(source, destination_, false, same->getNationality());			
			}
		} else {
			attackStrength_.max = 1 + processor.calculateSupportStrength(source, destination_, false); // true is only given
		}
	}
	
	std::cout << "attackStrength: max: " << attackStrength_.max << " min: " << attackStrength_.min << std::endl;
}

void MovementMove::calculatePreventStrength(MoveProcessor & processor) {

	// first min
	if(hasPath_ != YES) {
		preventStrength_.min = 0;
	} else {
		string source = getPiece()->getLocation();
		auto it = processor.getAttacks().find(source);
		bool headOnWon = false;
		if(it != processor.getAttacks().end()) {
			for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
				if((*it2)->getPiece()->getLocation() == destination_ && (*it2)->getMoveDecision() != NO) {
					preventStrength_.min = 0;
					headOnWon = true;
					break;
				}
			}
		}
		if(!headOnWon) {
			preventStrength_.min = 1 + processor.calculateSupportStrength(source, destination_, true);
		}
	}
	
	// now max
	if(hasPath_ == NO) {
		preventStrength_.max = 0;
	} else {
		string source = getPiece()->getLocation();
		auto it = processor.getAttacks().find(source);
		bool headOnWon = false;

		if(it != processor.getAttacks().end()) {
			for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
				if((*it2)->getPiece()->getLocation() == destination_ && (*it2)->getMoveDecision() == YES) {
					preventStrength_.max = 0;
					headOnWon = true;
					break;
				}
			}
		}
		if(!headOnWon) {
			preventStrength_.max = 1 + processor.calculateSupportStrength(source, destination_, false);
		}
	}
}

void MovementMove::calculateDefendStrength(MoveProcessor & processor) {
	defendStrength_.min = 1 + processor.calculateSupportStrength(getPiece()->getLocation(), destination_, true);
	defendStrength_.max = 1 + processor.calculateSupportStrength(getPiece()->getLocation(), destination_, false);
}

void MovementMove::calculateHoldStrength(MoveProcessor & processor) {
	switch(moved_) {
		case(YES):
			holdStrength_.max = 0;
			holdStrength_.min = 0;
			break;
		case(UNDECIDED):
			holdStrength_.min = 0;
			holdStrength_.max = 1;
			break;
		case(NO):
			holdStrength_.min = 1;
			holdStrength_.max = 1;
			break;
	}
}

bool MovementMove::determineMoveDecision(MoveProcessor & processor) {
	if(moved_ != UNDECIDED) {
		return true;
	}
	auto it = processor.getAttacks().find(destination_);
	unsigned int highestPreventStrengthMax = 0;
	unsigned int highestPreventStrengthMin = 0;
	for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
		if((*it2) == this) {
			continue;
		}
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
		std::cout << "FUCK" << std::endl;
		moved_ = NO;
		return true;
	}
	
	bool couldBeMoved = attackStrength_.min > highestPreventStrengthMax;

	MovementMove * headOnDefenderMove = nullptr;
	it = processor.getAttacks().find(getPiece()->getLocation());
	if(it != processor.getAttacks().end()) {
		for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			if((*it2)->getPiece()->getLocation() == destination_) {
				headOnDefenderMove = *it2;
				break;
			}
		}
	}
	if(headOnDefenderMove != nullptr) {
		couldBeMoved = couldBeMoved && attackStrength_.min > headOnDefenderMove->getDefendStrength().max;
		if(couldBeMoved) {
			moved_ = YES;
			return true;
		} else if(attackStrength_.max <= headOnDefenderMove->getDefendStrength().min) {
			moved_ = NO;
			std::cout << "FUCK2" << std::endl;
			return true;
		}
		return false;
	} else {
		for(auto holdIt = processor.getMoves().begin(); holdIt != processor.getMoves().end(); holdIt++) {
			if((*holdIt)->getPiece()->getLocation() == destination_) {
				couldBeMoved = couldBeMoved && attackStrength_.min > (*holdIt)->getHoldStrength().max;
				if(couldBeMoved) {
					moved_ = YES;
					return true;
				} else if(attackStrength_.max <= (*holdIt)->getHoldStrength().min) {
					moved_ = NO;
					std::cout << "FUCK3" << std::endl;
					return true;
				}
				return false;
			}
		}
		if(couldBeMoved) {
			moved_ = YES;
			return true;
		}
		return false;
	}
}

bool MovementMove::determinePathDecision(MoveProcessor & processor) {
	if(hasPath_ != UNDECIDED) {
		return true;
	}
	if(!viaConvoy_) {
		hasPath_ = YES;
		return true;
	}
	std::unordered_set<string> alreadySearched;
	hasPath_ = reachesPath(processor, this->getPiece()->getLocation(), alreadySearched, true);
	return hasPath_ != UNDECIDED;
}

DecisionResult MovementMove::reachesPath(MoveProcessor & processor, string currentSource, std::unordered_set<string> & alreadySearched, bool firstSearch) {
	DecisionResult currentBest = NO;
	try {
		std::unordered_set<ConvoyMove *> & convoys = processor.getConvoys().at(destination_).at(this->getPiece()->getLocation());
		const std::unordered_set<string> neighbours = processor.getMap()->getFleetNeighbours(currentSource);
		for(string neighbour : neighbours) {
			if(neighbour == destination_) {
				if(firstSearch) {
					continue;
				}
				return YES;
			}
		
			if(alreadySearched.count(neighbour) != 0) {
				continue;
			}
			bool found = false;
			for(ConvoyMove * move : convoys) {
				if(move->getPiece()->getLocation() == neighbour) {
				 	if(move->getDislodgeDecision() != YES) {
						found = true;
						//currentBest = DecisionResult.UNDECIDED;
						if(move->getDislodgeDecision() == UNDECIDED) {
							currentBest = UNDECIDED;
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
			if(futurePath == UNDECIDED) {
				currentBest = UNDECIDED;
			} else if(futurePath == YES && this->getDislodgeDecision() == NO) {
				return YES;
			}
		}
	} catch(std::out_of_range) {}
	return currentBest;
}

bool MovementMove::determineDislodgeDecision(MoveProcessor & processor) {
	if(dislodged_ != UNDECIDED) {
		return true;
	}
	if(moved_ == YES) {
		dislodged_ = NO;
		return true;
	}
	auto it = processor.getAttacks().find(this->getPiece()->getLocation());
	bool canBecomeSustained = true;
	if(it != processor.getAttacks().end()) {
		for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			MovementMove * move = *it2;
			if(move->getMoveDecision() != NO) {
				canBecomeSustained = false;
				if(move->getMoveDecision() == YES && moved_ == NO) {
					dislodged_ = YES;
					return true;
				}
			}
		}
	} else {
		dislodged_ = NO;
		return true;
	}
	
	if(canBecomeSustained) {
		dislodged_ = NO;
		return true;
	} 
	return false;
}

bool MovementMove::process(MoveProcessor & processor) {
	std::cout << "movementmove processing started" << std::endl;
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
