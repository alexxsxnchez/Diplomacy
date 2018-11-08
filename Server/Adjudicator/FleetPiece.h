#ifndef FLEETPIECE_H
#define FLEETPIECE_H

#include "Piece.h"

using std::vector;
using std::string;

class FleetPiece : public Piece {
	private:
		string coast_;
	protected:
		void print(ostream & out) const;
	public:
		FleetPiece(Nation nationality, string startingLocation, string coastSpecifier = "");
//		vector<Move> findPossibleMoves(const Graph * territories) const;
		string getCoast() const {
			return coast_;
		}
		
		bool isMovementValid(const MovementMove * move, Graph * graph) const;

		bool isSupportValid(const SupportMove * move, Graph * graph) const;

		bool isConvoyValid(const ConvoyMove * move, Graph * graph) const;
		
		std::unordered_set<string> getNeighbours(Graph * graph) const;
};

#endif
