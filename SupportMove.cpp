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

void SupportMove::process(map<string, map<const Move *, float> > & attacks) const {
	// if not attacked, then add support functionality
	auto it = attacks.find(getPiece()->getLocation());

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
	// act as hold move
	it = attacks.find(getPiece()->getLocation());
	std::pair<const Move *, float> pair = std::make_pair(this, 1.5);
	if(it == attacks.end()) {
		std::map<const Move *, float> map;
		map.insert(pair);
		attacks.insert(std::make_pair(getPiece()->getLocation(), map));
	} else {
		bool found = false;
		for(auto it2 = (it->second).begin(); it2 != (it->second).end(); it2++) {
			if(it2->first->getPiece()->getLocation() == getPiece()->getLocation()) {
				float value = it2->second;
				delete it2->first;
				(it->second).insert(std::make_pair(this, 1.5 + (value * -1)));
				found = true;
				break;
			}
		}
		if(!found) {
			(it->second).insert(pair);
		}
	}
}

string SupportMove::getSource() const {
	return source_;
}

string SupportMove::getDestination() const {
	return destination_;
}
