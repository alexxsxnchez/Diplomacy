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
		string coast_;
		DecisionResult supportGiven_ = UNDECIDED;

		bool determineSupportDecision(MoveProcessor & processor);
	protected:
		void print(ostream & out) const;

	public:
		SupportMove(Piece * piece, string source, string destination, string coast_ = "");
		void calculateIsLegal(Graph * graph);
		bool process(MoveProcessor & processor);
		string getSource() const;
		string getDestination() const;
		string getCoast() const;
		DecisionResult getSupportDecision() const;
		bool isCompletelyDecided() const;
		
		bool isPartOfParadoxCore(MoveProcessor * processor) const;
		Move * getParadoxDependency(MoveProcessor * processor) const;
		void settleParadox(bool isParadoxCore);
		void forceFail();
};

#endif
