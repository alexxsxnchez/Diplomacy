#include "MovementMove.h"
#include "ConvoyMove.h"
#include "SupportMove.h"
#include <ostream>
#include "Piece.h"
#include <unordered_set>
#include "Graph.h"
#include <iostream>
#include <queue>

using std::string;

MovementMove::MovementMove(Piece * piece, string destination, string coastSpecifier, bool viaConvoy) : Move{piece}, destination_{destination}, coastSpecifier_{coastSpecifier}, viaConvoy_{viaConvoy} {}

void MovementMove::print(ostream & out) const {
	out << *getPiece() << " MOVES to " << destination_ << std::endl;
}

bool MovementMove::isLegal(Graph * graph) const {
	return getPiece()->isMovementValid(this, graph);
}

bool MovementMove::isPartOfParadoxCore(MoveProcessor * processor) const {
//	std::cout << "ispartofparadoxcore" << std::endl;
	MoveProcessor::AttackMap & attacks = processor->getAttacks();
	for(auto it = attacks.begin(); it != attacks.end(); it++) {
		for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			if((*it2)->getPiece()->getLocation() == destination_) {
//				std::cout << "false" << std::endl;
				return false;
			}
		}
	}
//	std::cout << "true" << std::endl;
	return true;
}

Move * MovementMove::getParadoxDependency(MoveProcessor * processor) const {
	MoveProcessor::AttackMap & attacks = processor->getAttacks();
	for(auto it = attacks.begin(); it != attacks.end(); it++) {
		for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			if((*it2)->getPiece()->getLocation() == destination_) {
				std::cerr << getPiece()->getLocation() << destination_ << std::endl;
				return *it2;
			}
		} 
	}
	
	MoveProcessor::SupportMap & supports = processor->getSupports();
	string destination = coastSpecifier_ == "" ? destination_ : destination_ + "_" + coastSpecifier_; 
	auto it = supports.find(destination); // add coast specif
	if(it != supports.end()) {
		auto it2 = it->second.find(getPiece()->getLocation());
		if(it2 != it->second.end()) {
			for(auto it3 = it2->second.begin(); it3 != it2->second.end(); it3++) {
				if((*it3)->getSupportDecision() == UNDECIDED) {
					return *it3;
				}
			}
		}
	}
	
	if(viaConvoy_) {
		MoveProcessor::ConvoyMap & convoys = processor->getConvoys();
		auto it = convoys.find(destination_);
		if(it != convoys.end()) {
			auto it2 = it->second.find(getPiece()->getLocation());
			if(it2 != it->second.end()) {
				for(auto it3 = it2->second.begin(); it3 != it2->second.end(); it3++) {
					if((*it3)->getDislodgeDecision() == UNDECIDED) {
						return *it3;
					}
				}
			}
		}
	}
	
	return nullptr;
}

void MovementMove::settleParadox(bool isParadoxCore) {
	if(isParadoxCore) {
		attackStrength_.min = 0;
		attackStrength_.max = 0;
		preventStrength_.min = 0;
		preventStrength_.max = 0;
	} else {
		moved_ = YES;
	}
};

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
bool MovementMove::calculateAttackStrength(MoveProcessor & processor) {
	unsigned int attackStrengthMinPrev = attackStrength_.min;
	unsigned int attackStrengthMaxPrev = attackStrength_.max;
	
	string source = getPiece()->getLocation();
	Piece * defender = nullptr;
	MovementMove * leavingDefender = nullptr;
	auto it = processor.getNonAttacks().find(destination_); // no coast
	if(it != processor.getNonAttacks().end()) {
		defender = it->second->getPiece();
	} else {
		auto it2 = processor.getAttacks().find(source); // no coast
		if(it2 != processor.getAttacks().end()) {
			for(auto it3 = it2->second.begin(); it3 != it2->second.end(); it3++) {
				if((*it3)->getPiece()->getLocation() == destination_) {
					if(viaConvoy_ || (*it3)->getViaConvoy()) {
						leavingDefender = *it3;
					} else {
						defender = (*it3)->getPiece();
					}
					break;
				}
			}
		} else {
			for(auto it3 = processor.getAttacks().begin(); it3 != processor.getAttacks().end(); it3++) {
				for(auto it4 = it3->second.begin(); it4 != it3->second.end(); it4++) { 
					if((*it4)->getPiece()->getLocation() == destination_ && (*it4)->getMoveDecision() != YES) {
						leavingDefender = *it4;
						break;
					}
				}
			}
		}
	}

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
				attackStrength_.min = 1 + processor.calculateSupportStrength(source, destination_, coastSpecifier_, true, same->getNationality()); // should add coast specifier			
			}
		} else {
			attackStrength_.min = 1 + processor.calculateSupportStrength(source, destination_, coastSpecifier_, true); // true is only given // should add coast specifier
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
				attackStrength_.max = 1 + processor.calculateSupportStrength(source, destination_, coastSpecifier_, false, same->getNationality()); // add coast specif
				// possibly have calcsupport strength return list of dependencies, thru params	
			}
		} else {
			attackStrength_.max = 1 + processor.calculateSupportStrength(source, destination_, coastSpecifier_, false); // true is only given  // add coast specif
			// possibly have calcsupport strength return list of dependencies, thru params
		}
	}
	//	std::cerr << "attackStrength: max: " << attackStrength_.max << " min: " << attackStrength_.min << std::endl;
	return !(attackStrengthMinPrev == attackStrength_.min && attackStrengthMaxPrev == attackStrength_.max);
}

bool MovementMove::calculatePreventStrength(MoveProcessor & processor) {
	unsigned int preventStrengthMinPrev = preventStrength_.min;
	unsigned int preventStrengthMaxPrev = preventStrength_.max;
	
	// first min
	if(hasPath_ != YES) {
		preventStrength_.min = 0;
	} else {
		string source = getPiece()->getLocation();
		auto it = processor.getAttacks().find(source);
		bool headOnWon = false;
		if(it != processor.getAttacks().end()) {
			for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
				if((*it2)->getPiece()->getLocation() == destination_ && (*it2)->getMoveDecision() != NO && !viaConvoy_ && !(*it2)->getViaConvoy()) {
					preventStrength_.min = 0;
					headOnWon = true;
					break;
				}
			}
		}
		if(!headOnWon) {
			preventStrength_.min = 1 + processor.calculateSupportStrength(source, destination_, coastSpecifier_, true); // add coast specif
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
				if((*it2)->getPiece()->getLocation() == destination_ && (*it2)->getMoveDecision() == YES && !viaConvoy_ && !(*it2)->getViaConvoy()) {
					preventStrength_.max = 0;
					headOnWon = true;
					break;
				}
			}
		}
		if(!headOnWon) {
			preventStrength_.max = 1 + processor.calculateSupportStrength(source, destination_, coastSpecifier_, false); // add coast specif
			// possibly have calcsupport strength return list of dependencies, thru params
		}
	}
	return !(preventStrengthMinPrev == preventStrength_.min && preventStrengthMaxPrev == preventStrength_.max);
}

bool MovementMove::calculateDefendStrength(MoveProcessor & processor) {
	unsigned int defendStrengthMinPrev = defendStrength_.min;
	unsigned int defendStrengthMaxPrev = defendStrength_.max;
	defendStrength_.min = 1 + processor.calculateSupportStrength(getPiece()->getLocation(), destination_,  coastSpecifier_, true); // add coast specif
	defendStrength_.max = 1 + processor.calculateSupportStrength(getPiece()->getLocation(), destination_, coastSpecifier_, false); // add coast specif
	// possibly have calcsupport strength return list of dependencies, thru params
	return !(defendStrengthMinPrev == defendStrength_.min && defendStrengthMaxPrev == defendStrength_.max);
}

bool MovementMove::calculateHoldStrength(MoveProcessor & processor) {
	unsigned int holdStrengthMinPrev = holdStrength_.min;
	unsigned int holdStrengthMaxPrev = holdStrength_.max;
	switch(moved_) {
		case(YES):
			holdStrength_.max = 0;
			holdStrength_.min = 0;
			break;
		case(UNDECIDED):
			holdStrength_.min = 0;
			holdStrength_.max = 1;
			// add moved_ to dependency list
			break;
		case(NO):
			holdStrength_.min = 1;
			holdStrength_.max = 1;
			break;
	}
	return !(holdStrengthMinPrev == holdStrength_.min && holdStrengthMaxPrev == holdStrength_.max);
}

bool MovementMove::determineMoveDecision(MoveProcessor & processor) {
	if(moved_ != UNDECIDED) {
		return false;
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
		moved_ = NO;
		return true;
	}
	
	bool couldBeMoved = attackStrength_.min > highestPreventStrengthMax;

	MovementMove * headOnDefenderMove = nullptr;
	it = processor.getAttacks().find(getPiece()->getLocation());
	if(it != processor.getAttacks().end()) {
		for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			if((*it2)->getPiece()->getLocation() == destination_) {
				if(!viaConvoy_ && !(*it2)->getViaConvoy()) {
					headOnDefenderMove = *it2;
				}
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
			return true;
		}
		// add either defendstrength of headondefender to dependency list or highestPreventStrength
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
		return false;
	}
	if(!viaConvoy_) {
		hasPath_ = YES;
		return true;
	}
	std::unordered_set<string> alreadySearched;
	hasPath_ = reachesPath(processor/*, this->getPiece()->getLocation(), alreadySearched, true*/);
	return hasPath_ != UNDECIDED;
}

DecisionResult MovementMove::reachesPath(MoveProcessor & processor) {
	std::unordered_set<string> alreadySearched;
	std::queue<std::pair<string, DecisionResult> > queue;
	alreadySearched.insert(getPiece()->getLocation());
	queue.push(std::make_pair(getPiece()->getLocation(), YES));
	
	MoveProcessor::ConvoyMap & convoyMap = processor.getConvoys();
	auto it = convoyMap.find(destination_);
	if(it == convoyMap.end()) {
		return NO;
	}
	auto it2 = it->second.find(getPiece()->getLocation());
	if(it2 == it->second.end()) {
		return NO;
	}
	std::unordered_set<ConvoyMove *> convoys = it2->second;
	bool firstSearch = true;
	DecisionResult bestResult = NO;
	while(!queue.empty()) {
		std::pair<string, DecisionResult> pair = queue.front();
		string currentSource = pair.first;
//		std::cerr << "currentSource: " << currentSource << std::endl;
		queue.pop();
		
		std::unordered_set<string> neighbours = processor.getMap()->getFleetNeighbours(currentSource); // no coasts
		std::unordered_set<string> doubleCoastNeighbours = processor.getMap()->getDoubleCoastNeighbours(currentSource);
		for(string doubleCoastNeighbour : doubleCoastNeighbours) {
			neighbours.insert(doubleCoastNeighbour);
		}
		
		/*if(currentSource.find("_EC") != string::npos || currentSource.find("_SC") != string::npos || currentSource.find("_NC") != string::npos) {
			currentSource.erase(currentSource.length() - 3);
			neighbours = processor.getMap()->getCoastConvoyNeighbours(currentSource);
		}*/
		for(string neighbour : neighbours) {
			if(neighbour.find("_EC") != string::npos || neighbour.find("_SC") != string::npos || neighbour.find("_NC") != string::npos) {
				neighbour.erase(neighbour.length() - 3);
			}
			if(neighbour == destination_) {
				if(firstSearch) {
					continue;
				}
//				std::cerr << "found destination from current source: " << currentSource << "with result: " << pair.second << std::endl;
				if(pair.second == YES) {
					return YES;
				} else {
					bestResult = pair.second;
				}
			}
			if(alreadySearched.count(neighbour) != 0) {
				continue;
			}
			
			for(ConvoyMove * convoyMove : convoys) {
				if(!convoyMove->getIsLegal()) {
					continue;
				}
//				std::cerr << "convoy move location: " << convoyMove->getPiece()->getLocation() << " neighbour: " << neighbour << std::endl;
				if(convoyMove->getPiece()->getLocation() == neighbour) {
					if(convoyMove->getDislodgeDecision() != YES) {
						DecisionResult currentResult;
						if(pair.second == YES && convoyMove->getDislodgeDecision() == NO) {
							currentResult = YES;
						} else {
							currentResult = UNDECIDED;
						}
						queue.push(std::make_pair(convoyMove->getPiece()->getLocation(), currentResult));
					}
					alreadySearched.insert(neighbour);
					break;
				}
			}
		}
		firstSearch = false;
	}
	return bestResult;
}





/*DecisionResult MovementMove::reachesPath(MoveProcessor & processor, string currentSource, std::unordered_set<string> & alreadySearched, bool firstSearch) {
	DecisionResult currentBest = NO;
	try {
		std::unordered_set<ConvoyMove *> & convoys = processor.getConvoys().at(destination_).at(this->getPiece()->getLocation());
		const std::unordered_set<string> neighbours = processor.getMap()->getFleetNeighbours(currentSource);
		for(string neighbour : neighbours) {
			if(neighbour == destination_) {
				if(firstSearch) {
					continue;
				}
				std::cerr << "found destination from current source: " << currentSource << std::endl;
				return YES;
			}
		
			if(alreadySearched.count(neighbour) != 0) {
				continue;
			}
			bool found = false;
			for(ConvoyMove * move : convoys) {
				std::cerr << "convoy move location: " << move->getPiece()->getLocation() << " neighbour: " << neighbour << std::endl;
				if(move->getPiece()->getLocation() == neighbour) {
					std::cerr << "HEEELLO" << std::endl;
				 	if(move->getDislodgeDecision() != YES) {
				 		std::cerr << "hello 2" << std::endl;
						found = true;
						if(move->getDislodgeDecision() == UNDECIDED) {
							currentBest = UNDECIDED; // dependency
						} else {
							currentBest = YES;
						}
					}
					alreadySearched.insert(neighbour);
					break;
				}
			}
			if(!found) {
				std::cerr << "LOL2" << std::endl;
				continue;
			}
			
			DecisionResult futurePath = reachesPath(processor, neighbour, alreadySearched);
			if(futurePath == UNDECIDED) {
				currentBest = UNDECIDED; // dependency ??
			} else if(futurePath == YES && currentBest == YES) {
				return YES;
			}
		}
	} catch(std::out_of_range) {
		std::cerr << "LOL" << std::endl;
	}
	
	std::cerr << "LOL3\n\n" << std::endl;
	std::cerr << "path decision for " << getPiece()->getLocation() << " is " << currentBest << std::endl;
	return currentBest;
}*/

bool MovementMove::determineDislodgeDecision(MoveProcessor & processor) {
	if(dislodged_ != UNDECIDED) {
		return false;
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
					dislodgedFrom_ = move->getPiece()->getLocation();
					dislodged_ = YES; // dependency
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
//	std::cerr << "movementmove processing started" << std::endl;
	bool pathUpdated = determinePathDecision(processor);
	bool attackStrengthUpdated = calculateAttackStrength(processor);
	bool preventStrengthUpdated = calculatePreventStrength(processor);
	bool defendStrengthUpdated = calculateDefendStrength(processor);
	bool moveUpdated = determineMoveDecision(processor);
	bool holdStrengthUpdated = calculateHoldStrength(processor);
	bool dislodgedUpdated = determineDislodgeDecision(processor);
	
	return pathUpdated || moveUpdated || dislodgedUpdated || holdStrengthUpdated || defendStrengthUpdated || attackStrengthUpdated || preventStrengthUpdated;
}

void MovementMove::forceFail() {
	attackStrength_.min = 0;
	attackStrength_.max = 0;
	preventStrength_.min = 0;
	preventStrength_.max = 0;
	defendStrength_.min = 0;
	defendStrength_.max = 0;
	moved_ = NO;
	hasPath_ = NO;
	description_ = "Illegal move. " + getPiece()->getLocation() + " cannot move to " + destination_ + ".";
}

string MovementMove::getDestination() const {
	return destination_;
}

string MovementMove::getCoast() const {
	return coastSpecifier_;
}

bool MovementMove::getViaConvoy() const {
	return viaConvoy_;
}

void MovementMove::setConvoyStatus(bool status) {
	viaConvoy_ = status;
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

bool MovementMove::isCompletelyDecided() const {
	return dislodged_ != UNDECIDED && moved_ != UNDECIDED && hasPath_ != UNDECIDED;
}
