#ifndef HOLD_MOVE_H
#define HOLD_MOVE_H

#include "Move.h"
#include <ostream>

using std::unordered_set;
using std::map;
using std::string;
using std::pair;

class HoldMove : public Move {

	protected:
		void print(ostream & out) const;

	public:
		HoldMove(Piece * piece);
		void putIntoSet(unordered_set<HoldMove *> & holdMoves, unordered_set<MovementMove *> & movementMoves, unordered_set<SupportMove *> & supportMoves, unordered_set<ConvoyMove *> & convoyMoves);
		bool isLegal(Graph * graph) const;
		void process(MoveProcessor & processor);
};

#endif
