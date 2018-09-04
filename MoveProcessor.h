#ifndef MOVE_PROCESSOR_H
#define MOVE_PROCESSOR_H

#include <unordered_set>
#include <map>

class Move;
class HoldMove;
class MovementMove;
class SupportMove;
class ConvoyMove;

class MoveProcessor {
	private:
		std::unordered_set<HoldMove *> holdMoves_;
		std::unordered_set<MovementMove *> movementMoves_;
		std::unordered_set<SupportMove *> supportMoves_;
		std::unordered_set<ConvoyMove *> convoyMoves_;
//		std::unordered_set<Move *> moves_;
		std::map<std::string, std::map<const Move *, float> > attacks_;
	public:
		MoveProcessor();
		virtual ~MoveProcessor();
		
		void addMove(Move * move);
		void processMoves();
};

#endif
