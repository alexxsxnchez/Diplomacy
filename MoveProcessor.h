#ifndef MOVE_PROCESSOR_H
#define MOVE_PROCESSOR_H

#include <unordered_set>
#include <map>

using std::string;
using std::map;

class Move;
class HoldMove;
class MovementMove;
class SupportMove;
class ConvoyMove;

class MoveProcessor {
	private:
//		std::unordered_set<HoldMove *> holdMoves_;
//		std::unordered_set<MovementMove *> movementMoves_;
//		std::unordered_set<SupportMove *> supportMoves_;
//		std::unordered_set<ConvoyMove *> convoyMoves_;
		std::unordered_set<Move *> moves_;
//		map<string, Move *> moves_;
		map<string, map<const Move *, float> > attacks_;
		map<string, map<string, std::unordered_set<const SupportMove *> > > supports_;
		map<string, map<string, string> > convoys_;
		//		map<string, std::pair<std::unordered_set<const SupportMove *>, float> > supports_;
		float calculateSupportStrength(string source, string destination);
		void processConvoys();
		void processSupports();
		void processAttacks();
	public:
		MoveProcessor();
		virtual ~MoveProcessor();
		
		void addMove(Move * move);
		void processMoves();
};

#endif
