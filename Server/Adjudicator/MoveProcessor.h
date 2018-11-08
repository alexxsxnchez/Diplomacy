#ifndef MOVE_PROCESSOR_H
#define MOVE_PROCESSOR_H

#include <unordered_set>
#include <map>
#include "Nation.h"

using std::string;

class Move;
class HoldMove;
class MovementMove;
class SupportMove;
class ConvoyMove;
class Graph;

class MoveProcessor {
	public:
		typedef std::map<string, Move *> NonAttackMap;
		typedef std::map<string, std::unordered_set<MovementMove *> > AttackMap;
		typedef std::map<string, std::map<string, std::unordered_set<SupportMove *> > > SupportMap;
		typedef std::map<string, std::map<string, std::unordered_set<ConvoyMove *> > > ConvoyMap;
		typedef std::map<string, std::pair<bool, string> > Results;
		
	private:
//		std::unordered_set<HoldMove *> holdMoves_;
//		std::unordered_set<MovementMove *> movementMoves_;
//		std::unordered_set<SupportMove *> supportMoves_;
//		std::unordered_set<ConvoyMove *> convoyMoves_;
		std::unordered_set<Move *> moves_;
//		map<string, Move *> moves_;
		NonAttackMap nonAttacks_;
		AttackMap attacks_;
		//map<string, map<const MovementMove *, float> > attacksViaConvoy_;
		SupportMap supports_;
		ConvoyMap convoys_;
		Graph * map_;
		//		map<string, std::pair<std::unordered_set<const SupportMove *>, float> > supports_;
		/*void processMovesByConvoys();
		void processConvoys();
		void processSupports();
		void processAttacks();*/
	public:
		MoveProcessor(Graph * map);
		virtual ~MoveProcessor();
		
		void addMove(MovementMove * move);
		void addMove(HoldMove * move);
		void addMove(SupportMove * move);
		void addMove(ConvoyMove * move);
		Results processMoves();
		
		std::unordered_set<Move *> & getMoves();
		NonAttackMap & getNonAttacks();
		AttackMap & getAttacks();
		//SupportMap & getSupports() const;
		ConvoyMap & getConvoys();
		Graph * getMap() const;
		unsigned int calculateSupportStrength(string source, string destination, bool onlyGiven, Nation nationality = INVALID) const;
};

#endif
