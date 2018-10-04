#ifndef MOVE_H
#define MOVE_H

#include <ostream>
#include <map>
#include <string>
#include <unordered_set>
#include "MoveProcessor.h"

using std::unordered_set;
using std::ostream;
using std::map;
using std::pair;
using std::string;

class Piece;
class Graph;
class HoldMove;
class MovementMove;
class SupportMove;
class ConvoyMove;

struct Strength {
	unsigned int min = 0;
	unsigned int max = 100; // large number
}

enum DecisionResult {
	YES,
	NO,
	UNDECIDED
};

class Move {
	private:
		Piece * piece_;
		
	protected:
		DecisionResult dislodged_ = UNDECIDED;	
		Strength holdStrength_;
		
		virtual void calculateHoldStrength(MoveProcessor & processor);
		virtual bool determineDislodgeDecision(MoveProcessor & processor);
		virtual void print(ostream & out) const = 0;
		
	public:
		Move(Piece * piece);
		virtual ~Move();
		Piece * getPiece() const;
		virtual bool isLegal(Graph * graph) const = 0;
		virtual bool process(MoveProcessor & processor) = 0;
		DecisionResult getDislodgeDecision() const;
		Strength getHoldStrength() const;

		friend ostream & operator<<(ostream & out, const Move & move);
};

#endif
