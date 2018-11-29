#ifndef NATION_H
#define NATION_H

#include <ostream>
#include <string>

using std::string;
using std::ostream;

enum Nation {
	FRANCE,
	ENGLAND,
	GERMANY,
	RUSSIA,
	ITALY,
	AUSTRIA,
	TURKEY,
	INVALID
};

static Nation getNation(string s) {
	Nation nation;
	if(s == "f" || s == "FRANCE") {
		nation = Nation::FRANCE;
	} else if(s == "e" || s == "ENGLAND") {
		nation = Nation::ENGLAND;
	} else if(s == "g" || s == "GERMANY") {
		nation = Nation::GERMANY;
	} else if(s == "r" || s == "RUSSIA") {
		nation = Nation::RUSSIA;
	} else if(s == "i" || s == "ITALY") {
		nation = Nation::ITALY;
	} else if(s == "a" || s == "AUSTRIA") {
		nation = Nation::AUSTRIA;
	} else if(s == "t" || s == "TURKEY") {
		nation = Nation::TURKEY;
	} else {
		throw s;
	}
	return nation;
}

static ostream & operator<<(ostream & out, const Nation & nation) {
	switch(nation) {
		case FRANCE:
			out << "FRANCE";
			break;
		case ENGLAND:
			out << "ENGLAND";
			break;
		case GERMANY:
			out << "GERMANY";
			break;
		case RUSSIA:
			out << "RUSSIA";
			break;
		case ITALY:
			out << "ITALY";
			break;
		case AUSTRIA:
			out << "AUSTRIA";
			break;
		case TURKEY:
			out << "TURKEY";
			break;
	}
	return out;
}


#endif
