#include "Move.h"
#include "Piece.h"
#include "MovementMove.h"
#include <iostream>

Move::Move(Piece * piece) : piece_{piece} {}

Move::~Move() {}

Piece * Move::getPiece() const {
	return piece_;
}

void Move::calculateHoldStrength(MoveProcessor & processor) {
	holdStrength_.min = 1 + processor.calculateSupportStrength(piece_->getLocation(), piece_->getLocation(), true);
	holdStrength_.max = 1 + processor.calculateSupportStrength(piece_->getLocation(), piece_->getLocation(), false);
}

bool Move::determineDislodgeDecision(MoveProcessor & processor) {
	if(dislodged_ != UNDECIDED) {
		return false;
	}
	auto it = processor.getAttacks().find(this->getPiece()->getLocation());
	bool canBecomeSustained = true;
	if(it != processor.getAttacks().end()) {
		for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++) {
			MovementMove * move = *it2;
			if(move->getMoveDecision() != NO) {
				canBecomeSustained = false;
				if(move->getMoveDecision() == YES) {
					dislodgedFrom_ = move->getPiece()->getLocation();
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
	std::cerr << "Returning false (undecided) for nonAttack dislodgeDecision" << std::endl;
	return false;
}

std::unordered_set<string> Move::calculateRetreatOptions(std::unordered_set<string> contestedAreas, Graph * graph) const {
	std::unordered_set<string> retreatOptions = piece_->getNeighbours(graph);
	for(string contestedArea : contestedAreas) {
		retreatOptions.erase(contestedArea);
	}
	retreatOptions.erase(dislodgedFrom_);
	return retreatOptions;
}

DecisionResult Move::getDislodgeDecision() const {
	return dislodged_;
}

Strength Move::getHoldStrength() const {
	return holdStrength_;
}

string Move::getDescription() const {
	return description_;
}

ostream & operator<<(ostream & out, const Move & move) {
	move.print(out);
	return out;
}
