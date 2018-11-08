#include "Territory.h"

using std::endl;

Territory::Territory(string name, bool hasStar, Type type) : name_{name}, hasStar_{hasStar}, type_{type} {}

string Territory::getName() const {
	return name_;
}

bool Territory::hasStar() const {
	return hasStar_;
}

Territory::Type Territory::getType() const {
	return type_;
}

bool Territory::operator==(const Territory & other) const {
	return name_ == other.name_;
}

Territory::Type getTerritoryType(string s) {
	Territory::Type type;
	if(s == "l") {
		type = Territory::Type::INLAND;
	} else if(s == "c") {
		type = Territory::Type::COAST;
	} else if(s == "w") {
		type = Territory::Type::WATER;
	} else {
		throw s;
	}
	return type;
}

ostream & operator<<(ostream & out, const Territory::Type & type) {
	switch(type) {
		case Territory::Type::INLAND:
			out << "INLAND";
			break;
		case Territory::Type::COAST:
			out << "COAST";
			break;
		case Territory::Type::WATER:
			out << "WATER";
			break;
	}
	return out;
}

ostream & operator<<(ostream & out, const Territory & territory) {
	string hasStarString = territory.hasStar() ? "true" : "false";
	out << "\tHas star: " << hasStarString << endl;
	out << "\tType: " << territory.getType() << endl;
	return out;
}
