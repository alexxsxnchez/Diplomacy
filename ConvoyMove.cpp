#include "ConvoyMove.h"
#include <ostream>
#include "Piece.h"

using std::string;
using std::map;
using std::pair;

ConvoyMove::ConvoyMove(Piece * piece, string source, string destination) : Move{piece}, source_{source}, destination_{destination} {}

void ConvoyMove::print(ostream & out) const {


}

void ConvoyMove::putIntoSet(unordered_set<HoldMove *> & holdMoves, unordered_set<MovementMove *> & movementMoves, unordered_set<SupportMove *> & supportMoves, unordered_set<ConvoyMove *> & convoyMoves) {
	convoyMoves.insert(this);
}

bool ConvoyMove::isLegal(Graph * graph) const {
	return getPiece()->isConvoyValid(this, graph);
}

void ConvoyMove::process(map<string, map<const Move *, float> > & attacks) const {

}

string ConvoyMove::getSource() const {
	return source_;
}

string ConvoyMove::getDestination() const {
	return destination_;
}
