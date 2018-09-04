#ifndef CONVOY_MOVE_H
#define CONVOY_MOVE_H

#include "Move.h"
#include <ostream>
#include <string>

using std::string;
using std::map;
using std::pair;

class ConvoyMove : public Move {
	private:
		string source_;
		string destination_;

	protected:
		void print(ostream & out) const;

	public:
		ConvoyMove(Piece * piece, string source, string destination);
		void putIntoSet(unordered_set<HoldMove *> & holdMoves, unordered_set<MovementMove *> & movementMoves, unordered_set<SupportMove *> & supportMoves, unordered_set<ConvoyMove *> & convoyMoves);
		bool isLegal(Graph * graph) const;
		void process(map<string, map<const Move *, float> > & attacks) const;
		string getSource() const;
		string getDestination() const;
};

#endif
