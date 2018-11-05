#ifndef TERRITORY_H
#define TERRITORY_H

#include <string>
#include <ostream>

using std::string;
using std::ostream;

class Territory {
	public:
		enum Type { INLAND, COAST, WATER };

		Territory(string name, bool hasStar, Type type);
		string getName() const;
		bool hasStar() const;
		Type getType() const;
		bool operator==(const Territory & other) const;
	
	private:
		string name_;
		bool hasStar_;
		Type type_;
};

Territory::Type getTerritoryType(string s);
ostream & operator<<(ostream & out, const Territory::Type & type);
ostream & operator<<(ostream & out, const Territory & territory);

#endif
