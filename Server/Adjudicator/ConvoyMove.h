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
		bool isLegal_ = true;

	protected:
		void print(ostream & out) const;

	public:
		ConvoyMove(Piece * piece, string source, string destination);
		bool isLegal(Graph * graph) const;
		bool process(MoveProcessor & processor);
		string getSource() const;
		string getDestination() const;
		bool getIsLegal() const;
		bool isCompletelyDecided() const;
		
		bool isPartOfParadoxCore(MoveProcessor * processor) const;
		Move * getParadoxDependency(MoveProcessor * processor) const;
		void settleParadox(bool isParadoxCore);
		void forceFail();
};

#endif
