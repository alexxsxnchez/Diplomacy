#ifndef MOVEMENT_MOVE_H
#define MOVEMENT_MOVE_H

#include "Move.h"
#include <ostream>
#include <string>

using std::string;
using std::map;
using std::pair;

class MovementMove : public Move {
	private:
		Strength attackStrength_;
		Strength preventStrength_;
		Strength defendStrength_;
		DecisionResult moved_ = DecisionResult.UNDECIDED;
		DecisionResult hasPath_ = DecisionResult.UNDECIDED;
		string destination_;
		bool viaConvoy_;
		
		void calculateAttackStrength();
		void calculatePreventStrength();
		void calculateDefendStrength();
		void calculateHoldStrength() override;
		bool determineMoveDecision();
		bool determinePathDecision();
		bool determineDislodgeDecision() override;

	protected:
		void print(ostream & out) const;
		
	public:
		MovementMove(Piece * piece, string destination, bool viaConvoy);
		bool isLegal(Graph * graph) const;
		bool process(MoveProcessor & processor);
		string getDestination() const;
		bool getViaConvoy() const;
		DecisionResult getMoveDecision() const;
		DecisionResult getPathDecision() const;
		Strength getAttackStrength() const;
		Strength getPreventStrength() const;
		Strength getDefendStrength() const;
		
};

#endif
