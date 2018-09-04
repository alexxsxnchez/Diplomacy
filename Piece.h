#ifndef PIECE_H
#define PIECE_H

#include "Nation.h"
#include <string>
#include <vector>

class Graph;
class Move;
class HoldMove;
class MovementMove;
class SupportMove;
class ConvoyMove;

using std::string;
using std::vector;

class Piece {
	private:
		Nation nationality_;
		string location_;
	protected:
		virtual void print(ostream & out) const = 0;
	public:
		Piece(Nation nationality, string startingLocation);
		Nation getNationality() const;
		string getLocation() const;
		void setLocation(string newLocation);
//		virtual vector<Move> findPossibleMoves(const Graph * territories) const = 0;
		bool isHoldValid(const HoldMove * move, Graph * graph) const;
		virtual bool isMovementValid(const MovementMove * move, Graph * graph) const = 0;
		virtual bool isSupportValid(const SupportMove * move, Graph * graph) const = 0;
		virtual bool isConvoyValid(const ConvoyMove * move, Graph * graph) const = 0;
	
	friend ostream & operator<<(ostream & out, const Piece & piece);
};

#endif
