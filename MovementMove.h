#ifndef MOVEMENT_MOVE_H
#define MOVEMENT_MOVE_H

#include "Move.h"
#include <ostream>
#include <string>

using std::string;
using std::map;
using std::pair;

class MovementMove : public Move {
	private:
		string destination_;

	protected:
		void print(ostream & out) const;
	public:
		MovementMove(Piece * piece, string destination);
		void putIntoSet(unordered_set<HoldMove *> & holdMoves, unordered_set<MovementMove *> & movementMoves, unordered_set<SupportMove *> & supportMoves, unordered_set<ConvoyMove *> & convoyMoves);
		bool isLegal(Graph * graph) const;
		void process(map<string, map<const Move *, float> > & attacks, 
			map<string, map<string, std::unordered_set<const SupportMove *> > > & supports, 
			map<string, map<string, string> > & convoys) const;
		string getDestination() const;
};

#endif
