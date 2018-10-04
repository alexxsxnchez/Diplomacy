#include "Move.h"
#include "Piece.h"

Move::Move(Piece * piece) : piece_{piece} {}
Move::~Move() {}


Piece * Move::getPiece() const {
	return piece_;
}

void Move::calculateHoldStrength(MoveProcessor & processor) {
	holdStrength_.min = 1 + processor.calculateSupportStrength(piece_->getLocation(), piece_->getLocation(), true);
	holdStrength_.max = 1 + processor.calculateSupportStrength(piece_->getLocation(), piece_->getLocation(), false);
}

ostream & operator<<(ostream & out, const Move & move) {
	move.print(out);
	return out;
}
