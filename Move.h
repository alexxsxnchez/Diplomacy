#ifndef MOVE_H
#define MOVE_H

#include <ostream>
#include <map>
#include <string>
#include <unordered_set>

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

class Move {
	private:
		Piece * piece_;
	protected:
		virtual void print(ostream & out) const = 0;
	public:
		Move(Piece * piece);
		virtual ~Move();
		Piece * getPiece() const;
		virtual void putIntoSet(unordered_set<HoldMove *> &, unordered_set<MovementMove *> &, unordered_set<SupportMove *> &, unordered_set<ConvoyMove *> &) = 0;
		virtual bool isLegal(Graph * graph) const = 0;
		virtual void process(map<string, map<const Move *, float> > & attacks, 
			map<string, map<string, std::unordered_set<const SupportMove *> > > & supports, 
			map<string, map<string, string> > & convoys) const = 0;

		friend ostream & operator<<(ostream & out, const Move & move);
};

#endif
