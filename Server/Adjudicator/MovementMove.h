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
		DecisionResult moved_ = UNDECIDED;
		DecisionResult hasPath_ = UNDECIDED;
		string destination_;
		bool viaConvoy_;
		
		void calculateAttackStrength(MoveProcessor & processor);
		void calculatePreventStrength(MoveProcessor & processor);
		void calculateDefendStrength(MoveProcessor & processor);
		void calculateHoldStrength(MoveProcessor & processor) override;
		bool determineMoveDecision(MoveProcessor & processor);
		bool determinePathDecision(MoveProcessor & processor);
		bool determineDislodgeDecision(MoveProcessor & processor) override;
		DecisionResult reachesPath(MoveProcessor & processor/*, string currentSource, std::unordered_set<string> & alreadySearched, bool firstSearch = false*/);
	protected:
		void print(ostream & out) const;
		
	public:
		MovementMove(Piece * piece, string destination, bool viaConvoy = false);
		bool isLegal(Graph * graph) const;
		bool process(MoveProcessor & processor);
		string getDestination() const;
		bool getViaConvoy() const;
		DecisionResult getMoveDecision() const;
		DecisionResult getPathDecision() const;
		Strength getAttackStrength() const;
		Strength getPreventStrength() const;
		Strength getDefendStrength() const;
		bool isCompletelyDecided() const;
		
		bool isPartOfParadoxCore(MoveProcessor * processor) const;
		Move * getParadoxDependency(MoveProcessor * processor) const;
		void settleParadox(bool isParadoxCore);
};

#endif
