#ifndef SUPPORT_MOVE_H
#define SUPPORT_MOVE_H

#include "Move.h"
#include <ostream>
#include <string>

using std::string;
using std::pair;
using std::map;

class SupportMove : public Move {
	private:
		string source_;
		string destination_;

	protected:
		void print(ostream & out) const;

	public:
		SupportMove(Piece * piece, string source, string destination);
		void putIntoSet(unordered_set<HoldMove *> & holdMoves, unordered_set<MovementMove *> & movementMoves, unordered_set<SupportMove *> & supportMoves, unordered_set<ConvoyMove *> & convoyMoves);
		bool isLegal(Graph * graph) const;
		void process(map<string, map<const Move *, float> > & attacks) const;
		string getSource() const;
		string getDestination() const;
};

#endif
