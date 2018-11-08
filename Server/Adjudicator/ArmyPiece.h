#ifndef ARMYPIECE_H
#define ARMYPIECE_H

#include "Piece.h"

using std::vector;
using std::string;

class ArmyPiece : public Piece {
	protected:
		void print(ostream & out) const;
	public:
		ArmyPiece(Nation nationality, string startingLocation);
//		vector<Move> findPossibleMoves(const Graph * territories) const;
		bool isMovementValid(const MovementMove * move, Graph * graph) const;
		bool isSupportValid(const SupportMove * move, Graph * graph) const;
		bool isConvoyValid(const ConvoyMove * move, Graph * graph) const;
		std::unordered_set<string> getNeighbours(Graph * graph) const;
};

#endif
