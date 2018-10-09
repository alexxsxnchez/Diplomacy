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
		bool isLegal(Graph * graph) const;
		bool process(MoveProcessor & processor);
		string getSource() const;
		string getDestination() const;
		bool isCompletelyDecided() const;
};

#endif
