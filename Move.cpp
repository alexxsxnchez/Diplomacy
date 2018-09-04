#include "Move.h"
#include "Piece.h"

Move::Move(Piece * piece) : piece_{piece} {}
Move::~Move() {}


Piece * Move::getPiece() const {
	return piece_;
}

ostream & operator<<(ostream & out, const Move & move) {
	move.print(out);
	return out;
}
