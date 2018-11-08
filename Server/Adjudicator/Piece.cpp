#include "Piece.h"
#include <ostream>

using std::ostream;
using std::endl;

Piece::Piece(Nation nationality, string startingLocation) : nationality_{nationality}, location_{startingLocation} {}

Nation Piece::getNationality() const {
	return nationality_;
}

string Piece::getLocation() const {
	return location_;
}

void Piece::setLocation(string newLocation) {
	location_ = newLocation;
}

bool Piece::isHoldValid(const HoldMove * move, Graph * graph) const {
	return true;
}

ostream & operator<<(ostream & out, const Piece & piece) {
	piece.print(out);
	return out;
}
