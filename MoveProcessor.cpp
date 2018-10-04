#include "MoveProcessor.h"
#include "Move.h"
#include "HoldMove.h"
#include "MovementMove.h"
#include "SupportMove.h"
#include "ConvoyMove.h"
#include "Piece.h"
#include <vector>
#include <string>
#include <iostream>

MoveProcessor::MoveProcessor() {}

MoveProcessor::~MoveProcessor() {
	for(Move * move : moves_) {
		delete move;
	}
}

void MoveProcessor::addMove(HoldMove * move) {
	moves_.insert(move);
	nonAttacks_.insert(std::make_pair(move->getPiece()->getLocation(), move));
}

void MoveProcessor::addMove(MovementMove * move) {
	moves_.insert(move);
	auto it = attacks_.find(move->getDestination());
	if(it == attacks_.end()) {
		std::unordered_set<MovementMove *> moves;
		moves.insert(move);
		attacks_.insert(std::make_pair(destination_, moves));
	} else {
		(it->second).insert(move);
	}
}

void MoveProcessor::addMove(ConvoyMove * move) {
	moves_.insert(move);
	auto itC = convoys_.find(move->getDestination());
	if(itC == convoys_.end()) {
		std::map<string, std::unordered_set<ConvoyMove *> > convoysToDestination;
		std::unordered_set<ConvoyMove *> setOfMoves;
		setOfMoves.insert(move);
		convoysToDestination.insert(std::make_pair(move->getSource(), setOfMoves));
		convoys_.insert(std::make_pair(move->getDestination(), convoysToDestination));
	} else {
		auto it2 = itC->second.find(move->getSource());
		if(it2 == itC->second.end()) {
			std::unordered_set<ConvoyMove *> setOfMoves;
			setOfMoves.insert(move);
			itC->second.insert(std::make_pair(move->getSource(), setOfMoves));
		} else {
			itC->second.insert(move);
		}
	}
			
	// act as hold move
	nonAttacks_.insert(std::make_pair(move->getPiece()->getLocation(), move);
}

void MoveProcessor::addMove(SupportMove * move) {
	moves_.insert(move);
	auto itS = supports_.find(move->getDestination());
	if(itS == supports_.end()) {
		std::map<string, std::unordered_set<SupportMove *> > supportsToDestination;
		std::unordered_set<SupportMove *> setOfMoves;
		setOfMoves.insert(move);
		supportsToDestination.insert(std::make_pair(move->getSource(), setOfMoves));
		supports_.insert(std::make_pair(move->getDestination(), supportsToDestination));
	} else {
		auto it2 = itS->second.find(move->getSource());
		if(it2 == itS->second.end()) {
			std::unordered_set<SupportMove *> setOfMoves;
			setOfMoves.insert(move);
			itS->second.insert(std::make_pair(move->getSource(), setOfMoves));
		} else {
			it2->second.insert(move);
		}
	}
	
	// act as hold move
	nonAttacks_.insert(std::make_pair(move->getPiece()->getLocation(), move);
}

// ALGO

// 1. first do division of move types into different collections
// 2. then process movement first
// 3. then process convoys and remove movements that require convoys that don't work (also add convoy to attack as if it was hold)
// 4. then process supports and remove any that were target of successful movements. add successful supports to attacks and add support location as hold value in attacks
// then process holds in attacks

// 5. evaluate -> any standoffs requires reprocessing by first replacing failed movements with holds. There will also be something to look at if any won standoffs were in a location of a fleet doing a convoy. would have to remove the convoy and reprocess movement moves that require convoy.

// 6. repeat step 5 until no more standoffs. Return successful movements

// throws out_of_range exception
unsigned int MoveProcessor::calculateSupportStrength(string source, string destination, bool onlyGiven, Nation nationality = Nation.INVALID) const {
	unsigned int count = 0;
	try {
		auto supportToDestination = supports_.at(destination);
		auto supportFromSource = supportToDestination.at(source);
		for(const SupportMove * move : supportFromSource) {
			if(move->getPiece()->getNationality() == nationality) {
				continue;
			}
			if(move->getSupportDecision() != DecisionResult.NO && !onlyGiven) {
				count++;
			} else if(move->getSupportDecision() == DecisionResult.YES && onlyGiven) {
				count++;
			}
		}
	} catch(std::out_of_range) {
		return 0;
	}
	return count;
}

/*

// convoy does not cut certain supports rule is similar to supporting unit getting attacked
// by unit from destination
// for when convoy does cut via alternate route, the way it should be handled is any convoy
// that is "dislodged" by rule above, cannot participate in convoy. Whether or not that fleet does
// actually get dislodged will be handled with the support logic (already taken care of). 

void MoveProcessor::processMovesByConvoys() {
// remove convoys that do not match any moves
// nvm don't need to do the above
}

// ALGO
// 1. check if convoy is attacked with support greater than itself.
// if so remove itself from convoy map -> except we don't know if that support is cut by 
// something else entirely, can't just check attack map, since attack could be another convoy,
// that could also be disrupted
// 2.

bool MoveProcessor::checkPaths(const Move * move, std::unordered_set<const ConvoyMove *> convoys) {
// TODO
}

void MoveProcessor::processConvoys() {
// find at least a single pathway for the move to reach its destination thru use of convoys
	for(auto itA = attacksViaConvoy_.begin(); itA != attacksViaConvoy_.end(); it++) {
		for(auto it2A = itA->second.begin(); it2A != itA->second.end(); ) {
			if(!it2A->first->getViaConvoy()) {
				continue;
			}
			
			try {
				auto convoysToDestination = convoys_.at(itA->first);
				auto correctConvoys = convoysToDestination.at(it2A->first->getPiece()->getLocation());
				bool isAPath = checkPaths(itA2->first, correctConvoys);
				if(isAPath) {
					
					it2A++;
				} else {
					it2A = itA->second.erase(it2A);
				}
			} catch(std::out_of_range) {
				it2A = itA->second.erase(it2A);
			}
		}
	}
}

//		map<string, map<string, std::unordered_set<const SupportMove *> > > supports_;

void MoveProcessor::processSupports() {
	bool needToCheckForDislodgement = false;
	for(auto it = supports_.begin(); it != supports_.end(); it++) {
		string destination = it->first;
		for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			string source = it2->first;
			
			// first check if there even exists an attack that matches support
			bool isAttackThatMatches = false;
			try {
				auto attacks = attacks_.at(destination);
				for(auto it3 : attacks) {
					if(it3.first->getPiece()->getLocation() == source && it3.first->isValid()) {
						isAttackThatMatches = true;
						break;
					}
				}
			} catch(std::out_of_range) {}
	
			if(!isAttackThatMatches) {
				// erase all moves that had this source and destination
				it2 = it->second.erase(it2);
				continue;
			}
			// for each support that supports an attack from source to destination
			for(auto it3 = it2->second.begin(); it3 != it2->second.end(); ) {
				// next check if an attacker has attacked this unit && check if the attacker is from destination -> if it is must reprocess after
				string location = (*it3)->getPiece()->getLocation();
				try {
					auto attacks = attacks_.at(location);
					bool isAttackedFromNotDestination = false;
					for(auto it4 : attacks) {
						if(it4.first->getPiece()->getLocation() != location) { // if isn't a hold move
							if (it4.first->getPiece()->getLocation() == destination) {
								needToCheckForDislodgement = true;
							} else {
								isAttackedFromNotDestination = true;
								it3 = it2->second.erase(it3);
								break;
							}
						}
					}
					if(!isAttackedFromNotDestination) {
						it3++;
					}
				} catch(std::out_of_range) {}
			}
		}
	}
	if(!needToCheckForDislodgement) {
		return;
	}

	// if at this point, then there is at least one attacker attacking a support from that support's destination
	for(auto it = supports_.begin(); it != supports_.end(); it++) {
		string destination = it->first;
		for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			for(auto it3 = it2->second.begin(); it3 != it2->second.end(); it3++) {
				const SupportMove * move = *it3;
				string location = move->getPiece()->getLocation();
				float valueOfHold = 0;
				float valueOfAttack = 0;
				try {
					auto attacks = attacks_.at(location);
					for(auto it4 : attacks) {
						if(it4.first->getPiece()->getLocation() == destination && it4.first->isValid()) {
							valueOfAttack = it4.second;
						} else if(it4.first->getPiece()->getLocation() == location) {
							valueOfHold = it4.second;
						}
						if(valueOfHold > 0 && valueOfAttack > 0) {
							break;
						}
					}
					if(valueOfAttack > 0) {
						try {
							valueOfHold += calculateSupportStrength(location, location);
						} catch(std::out_of_range) {}
						try {
							valueOfAttack += calculateSupportStrength(destination, location);
						} catch(std::out_of_range) {}

						if(valueOfAttack > valueOfHold) {
							it2->second.erase(it3); // make sure second.first still gives correct reference and does not make a copy like error from other day
						}
						break;
					}
				} catch(std::out_of_range) {}
			}
		}
	}
}

// for all invalid convoy attacks, check to see if the attack strength on convoy is equal to
// or less than hold strength for every convoy in all chains. If true for any, invalid attack
// is now valid, and must return true at end of function.
bool processConvoysThatMaybeAreDislodgedByDestination() {
	for(auto itA = attacks_.begin(); itA != attacks_.end(); itA++) {
		for(auto it2A = itA->second.begin(); it2A != itA->second.end(); it2A++) {
			if(it2A->first->get
		}
	}

}

void MoveProcessor::processAttacks() {
// make this function return the dislodgelist

}
*/

void MoveProcessor::processMoves() {
	std::cout << "Processing" << std::endl;

	for(Move * move : moves_) {
		move->process(attacks_, supports_, convoys_);
	}
// might need a while loop enclosing processConvoys and processSupports where the while loop ends
// when there are no more convoys moves that are being attacked with at least one support?
	processConvoys(); // return boolean which is condition
	
	bool newValidAttack = false;
	do {
		processSupports();
		newValidAttack = processConvoysThatMaybeAreDislodgedByDestination();
	} while(newValidAttack);
	
	//processAttacks();


	bool mustReprocess;
	int j = 0;
	std::vector<Piece *> dislodgeList;
	do {
		std::cout << "evaluating" << std::endl;
		mustReprocess = false;
		std::vector<HoldMove *> holdMovesToBeAdded;

		// for each attacked location
		for(auto it = attacks_.begin(); it != attacks_.end(); it++) {

			for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
				bool isHoldMove = it2->first->getPiece()->getLocation() == it->first;
				if(!isHoldMove) {
					auto it3 = attacks_.find(it2->first->getPiece()->getLocation());
					if(it3 != attacks_.end()) {
						bool found = false;
						for(auto it4 : it3->second) {
							float attack = it2->second;
							float otherAttack = it4.second;
							try {
								attack += calculateSupportStrength(it2->first->getPiece()->getLocation(), it->first);
							} catch (std::out_of_range) {}
							try {
								otherAttack += calculateSupportStrength(it4.first->getPiece()->getLocation(), it3->first);
							} catch (std::out_of_range) {}
							if(it4.first->getPiece()->getLocation() == it->first && otherAttack >= attack) {
								// turn into hold move BEFORE calculating winningattack move
								std::cout << "\n\nadding " << it2->first->getPiece()->getLocation() << " to hold moves to be added" << std::endl;
								HoldMove * newHoldMove = new HoldMove((it2->first)->getPiece());
								holdMovesToBeAdded.push_back(newHoldMove);
								mustReprocess = true;
								it2 = (it->second).erase(it2);
								found = true;
								break;
							}
						}
						if(found) {
							break;
						}
					}
				}
			}

			const Move * winningAttackMove = NULL;
			float largestValue = 0;

			// for each attack on this location
			for(auto it2 : it->second) {
				float attackValue = it2.second;
				try {
					attackValue += calculateSupportStrength(it2.first->getPiece()->getLocation(), it->first);
				} catch (std::out_of_range) {}
				if(attackValue == largestValue) {
					winningAttackMove = NULL;
				} else if(attackValue > largestValue) {
					largestValue = attackValue;
					winningAttackMove = it2.first;
				}
			}
			std::cout << "largest value for " << it->first << " is " << largestValue << std::endl;
			std::cout << "size is " << it->second.size() << " for attack on " << it->first << std::endl;

			// means there is a standoff
			int i = 0;

			// for each attack on this location
			for(auto it2 = it->second.begin(); it2 != it->second.end(); i++) {
				std::cout << "i: " << i << std::endl;
				bool isHoldMove = it2->first->getPiece()->getLocation() == it->first;
				// need to remove all attacks that is not the winner and replace it with a hold move
				float attackValue = it2->second;
				try {
					attackValue += calculateSupportStrength(it2->first->getPiece()->getLocation(), it->first);
				} catch(std::out_of_range) {}
				
				if(attackValue != largestValue || winningAttackMove == NULL) {
				// have to correct for self-dislodgement
					if(!isHoldMove /*&& not same nation as unit stationed there*/) {
						std::cout << "removing: " << it2->first->getPiece()->getLocation() << " moving to " << it->first << std::endl;
						HoldMove * newHoldMove = new HoldMove((it2->first)->getPiece());
						holdMovesToBeAdded.push_back(newHoldMove);
						mustReprocess = true;
					} else {
						dislodgeList.push_back(it2->first->getPiece());
					}
					it2 = (it->second).erase(it2);
					std::cout << "size: " << it->second.size() << std::endl;
				} else {
					it2++;
				}
			}
			std::cout << "size after deletion is " << attacks_.find(it->first)->second.size() << " for attack on " << it->first << std::endl;
		
			std::cout << "Winning attack for " << it->first << " is ";
			if(winningAttackMove == NULL) {
				std::cout << "no one";
			} else {
				std::cout << winningAttackMove->getPiece()->getLocation();
			}
			std::cout << std::endl;
		}
		while(!holdMovesToBeAdded.empty()) {
			HoldMove * holdMove = holdMovesToBeAdded.back(); // need to be deleted
			holdMove->process(attacks_, supports_, convoys_);
			holdMovesToBeAdded.pop_back();
		}
		j++;
	} while(mustReprocess && j < 5);
	
	
	// output results
	for(auto it : attacks_) {
		std::cout << "Winning attacks on " << it.first << ":" << std::endl;
		for(auto it2 : it.second) {
			std::cout << it2.first->getPiece()->getLocation() << ", ";
		}
		std::cout << std::endl;
	}

	std::cout << "Dislodged pieces: " << std::endl;
	for(auto it : dislodgeList) {
		std::cout << it->getLocation() << ", ";	
	}
	std::cout << std::endl;
}

std::unordered_set<Move *> & MoveProcessor::getMoves() const {
	return moves_;
}

NonAttackMap & MoveProcessor::getNonAttacks() const {
	return nonAttacks_;
}
		
AttackMap & MoveProcessor::getAttacks() const {
	return attacks_;
}
		
ConvoyMap & MoveProcessor::getConvoys() const {
	return convoys_;
}


