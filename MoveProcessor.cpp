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
	for(HoldMove * move : holdMoves_) {
		delete move;
	}
	for(MovementMove * move : movementMoves_) {
		delete move;
	}
	for(SupportMove * move : supportMoves_) {
		delete move;
	}
	for(ConvoyMove * move : convoyMoves_) {
		delete move;
	}
}

void MoveProcessor::addMove(Move * move) {
	move->putIntoSet(holdMoves_, movementMoves_, supportMoves_, convoyMoves_);
}

// ALGO

// 1. first do division of move types into different collections
// 2. then process movement first
// 3. then process convoys and remove movements that require convoys that don't work (also add convoy to attack as if it was hold)
// 4. then process supports and remove any that were target of successful movements. add successful supports to attacks and add support location as hold value in attacks
// then process holds in attacks

// 5. evaluate -> any standoffs requires reprocessing by first replacing failed movements with holds. There will also be something to look at if any won standoffs were in a location of a fleet doing a convoy. would have to remove the convoy and reprocess movement moves that require convoy.

// 6. repeat step 5 until no more standoffs. Return successful movements



void MoveProcessor::processMoves() {
	std::cout << "Processing" << std::endl;
	for(HoldMove * move : holdMoves_) {
		move->process(attacks_);
	}
	for(MovementMove * move : movementMoves_) {
		move->process(attacks_);
	}
	for(ConvoyMove * move : convoyMoves_) {
		move->process(attacks_);
	}
	for(SupportMove * move : supportMoves_) {
		move->process(attacks_);
	}

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
							if(it4.first->getPiece()->getLocation() == it->first && it4.second >= it2->second) {
								// turn into hold move BEFORE calculating winningattack move
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
				if(it2.second == largestValue) {
					winningAttackMove = NULL;
				} else if(it2.second > largestValue) {
					largestValue = it2.second;
					winningAttackMove = it2.first;
				}
			}
			std::cout << "largest value for " << it->first << " is " << largestValue << std::endl;
			std::cout << "size is " << it->second.size() << " for attack on " << it->first << std::endl;

//			if(it->second.size() > 1) {
				// means there is a standoff
				int i = 0;

				// for each attack on this location
				for(auto it2 = it->second.begin(); it2 != it->second.end(); i++) {
					std::cout << "i: " << i << std::endl;
					bool isHoldMove = it2->first->getPiece()->getLocation() == it->first;
/*					bool isStrongerOrEqualAttackFromDestination = false;
					if(!isHoldMove) {
						auto it3 = attacks_.find(it2->first->getPiece()->getLocation());
						if(it3 != attacks_.end()) {
							for(auto it4 : it3->second) {
								if(it4.first->getPiece()->getLocation() == it->first && it4.second >= it2->second) {
									isStrongerOrEqualAttackFromDestination = true;
									// turn into hold move BEFORE calculating winningattack move


									break;
								}
							}
						}
					}
*/					// need to remove all attacks that is not the winner and replace it with a hold move
					if(it2->second != largestValue || winningAttackMove == NULL/* || isStrongerOrEqualAttackFromDestination*/) {
						if(!isHoldMove) {
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

//			}
		
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
			holdMove->process(attacks_);
			holdMovesToBeAdded.pop_back();
		}
		j++;
	} while(mustReprocess && j < 5);
	
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
