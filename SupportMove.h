#ifndef SUPPORT_MOVE_H
#define SUPPORT_MOVE_H

#include "Move.h"
#include <ostream>
#include <string>
#include <map>
#include <unordered_set>

using std::string;
using std::pair;
using std::map;

class SupportMove : public Move {
	private:
		string source_;
		string destination_;
		DecisionResult supportGiven_ = UNDECIDED;

		bool determineSupportDecision(MoveProcessor & processor);
	protected:
		void print(ostream & out) const;

	public:
		SupportMove(Piece * piece, string source, string destination);
		bool isLegal(Graph * graph) const;
		bool process(MoveProcessor & processor);
		string getSource() const;
		string getDestination() const;
		DecisionResult getSupportDecision() const;
		bool isCompletelyDecided() const;
};

#endif
