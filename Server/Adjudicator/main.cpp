#include "Graph.h"
#include "Territory.h"
#include "Nation.h"
#include "Piece.h"
#include "ArmyPiece.h"
#include "FleetPiece.h"
#include "Move.h"
#include "MovementMove.h"
#include "HoldMove.h"
#include "SupportMove.h"
#include "ConvoyMove.h"
#include <iostream>
#include <sstream>
#include <cassert>
#include <fstream>
#include <map>
#include <vector>
#include "MoveProcessor.h"
#include "ChildProcess.h"
#include <cstdio>
#include <stdio.h>
#include <errno.h>

using namespace std;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::istringstream;
using std::ifstream;
using std::map;
using std::vector;
using std::fgets;

static map<Nation, vector<Piece *> > readInPieces() {
	ifstream file;
	try {
		file.open("pieces.txt");
	} catch (...) {
		assert(false);
	}
	string line;
	map<Nation, vector<Piece *> > pieces;
	while(getline(file, line)) {
		printf("line by line...");
		istringstream s(line);
		string first, second, third, fourth;
		s >> first;
		if(first == "//" || first == "") {
			continue;
		}
		s >> second;
		s >> third;
		if(s.eof()) {
			fourth = "";
		} else {
			s >> fourth;
		}
		Nation nation;
		try {
			nation = getNation(second);
		} catch(string incorrectString) {
			cout << "ERROR: " << incorrectString << endl;
			assert(false);
		}
		
		Piece * piece;
		if(first == "a") {
			piece = new ArmyPiece(nation, third);		
		} else if(first == "f") {
			piece = new FleetPiece(nation, third, fourth);
		} else {
			cout << "ERROR: " << first << endl;
			assert(false);
		}
		auto it = pieces.find(nation);
		if(it == pieces.end()) {
			vector<Piece *> pieceList;
			pieceList.push_back(piece);
			pieces.insert(std::make_pair(nation, pieceList));
		} else {
			(it->second).push_back(piece);
		}
	}
	file.close();
	return pieces;
}


static void readInNodes(Graph & graph, string fileName) {
	ifstream file;
	try {
		file.open(fileName);
	} catch (...) {
		assert(false);
	}
	if(!file) {
		cerr << "file not opening" << endl;
	}
	string line;
	map<Nation, vector<Piece *> > pieces;
	while(getline(file, line)) {
		istringstream s(line);
		string first, second, third;
		s >> first;
		if(first == "//" || first == "") {
			continue;
		}
		s >> second;
		s >> third;
		if(first == "a") {
			graph.addArmyEdge(second, third);
		} else if(first == "f") {
			graph.addFleetEdge(second, third);
		} else if(first == "l" || first == "w" || first == "c") {
			bool boolean;
			if(third == "true") {
				boolean = true;
			} else if(third == "false") {
				boolean = false;
			} else {
				cerr << "ERROR: " << third << endl;
				assert(false);
			}
			Territory::Type type;
			try {
				type = getTerritoryType(first);
			} catch(string incorrectString) {
				cerr << "ERROR: " << incorrectString << endl;
				assert(false);
			}
			Territory * territory = new Territory(second, boolean, type);
			graph.addNode(second, territory);
		} else {
			cerr << "ERROR: " << first << endl;
			assert(false);
		}
	}
	file.close();
}

static void tc1(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::GERMANY, "Berlin"), "Silesia"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Warsaw"), "Silesia"));
	p.processMoves();
	std::cout << "----done tc1" << std::endl;
}

static void tc2(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Warsaw"), "Prussia"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Moscow"), "Warsaw"));
	p.addMove(new HoldMove(new ArmyPiece(Nation::GERMANY, "Prussia")));
	p.processMoves();
	std::cout << "----" << std::endl;
}

// ERROR: fixed with circular paradox
static void tc3(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Warsaw"), "Livonia"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Moscow"), "Warsaw"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::GERMANY, "Livonia"), "Moscow"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc4(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Warsaw"), "Prussia"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Prussia"), "Warsaw"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc5(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::GERMANY, "Burgundy"), "Gascony"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::FRANCE, "Marseilles"), "Burgundy"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::FRANCE, "Gascony"), "Marseilles", "Burgundy"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc6(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new FleetPiece(Nation::FRANCE, "GulfOfLyons"), "TyrrehenianSea"));
	p.addMove(new MovementMove(new FleetPiece(Nation::ITALY, "Naples"), "TyrrehenianSea"));
	p.addMove(new SupportMove(new FleetPiece(Nation::FRANCE, "WesternMed"), "GulfOfLyons", "TyrrehenianSea"));
	p.addMove(new SupportMove(new FleetPiece(Nation::ITALY, "Rome"), "Naples", "TyrrehenianSea"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc7(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::GERMANY, "Silesia"), "Prussia"));
	p.addMove(new SupportMove(new FleetPiece(Nation::GERMANY, "BalticSea"), "Silesia", "Prussia"));
	p.addMove(new HoldMove(new ArmyPiece(Nation::RUSSIA, "Prussia")));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc8(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new FleetPiece(Nation::FRANCE, "GulfOfLyons"), "TyrrehenianSea"));
	p.addMove(new HoldMove(new FleetPiece(Nation::ITALY, "TyrrehenianSea")));
	p.addMove(new SupportMove(new FleetPiece(Nation::FRANCE, "WesternMed"), "GulfOfLyons", "TyrrehenianSea"));
	p.addMove(new SupportMove(new FleetPiece(Nation::ITALY, "Rome"), "TyrrehenianSea", "TyrrehenianSea"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc9(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::AUSTRIA, "Bohemia"), "Munich"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::GERMANY, "Munich"), "Silesia"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Warsaw"), "Silesia"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::AUSTRIA, "Tyrolia"), "Bohemia", "Munich"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::GERMANY, "Berlin"), "Munich", "Silesia"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::RUSSIA, "Prussia"), "Warsaw", "Silesia"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc10(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::TURKEY, "Bulgaria"), "Rumania"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Rumania"), "Bulgaria"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Sevastopol"), "Rumania"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::RUSSIA, "Serbia"), "Rumania", "Bulgaria"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc11(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::TURKEY, "Bulgaria"), "Rumania"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Rumania"), "Bulgaria"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Sevastopol"), "Rumania"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::RUSSIA, "Serbia"), "Rumania", "Bulgaria"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::RUSSIA, "Greece"), "Rumania", "Bulgaria"));
	p.addMove(new SupportMove(new FleetPiece(Nation::TURKEY, "BlackSea"), "Bulgaria", "Rumania"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc12(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::GERMANY, "Prussia"), "Warsaw"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::GERMANY, "Silesia"), "Prussia", "Warsaw"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Bohemia"), "Silesia"));
	p.addMove(new HoldMove(new ArmyPiece(Nation::RUSSIA, "Warsaw")));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc13(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::GERMANY, "Prussia"), "Warsaw"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::GERMANY, "Silesia"), "Prussia", "Warsaw"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Warsaw"), "Silesia"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc14(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new FleetPiece(Nation::GERMANY, "Berlin"), "Prussia"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::GERMANY, "Silesia"), "Berlin", "Prussia"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Prussia"), "Silesia"));
	p.addMove(new MovementMove(new FleetPiece(Nation::RUSSIA, "BalticSea"), "Prussia"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::RUSSIA, "Warsaw"), "Prussia", "Silesia"));
	p.processMoves();
	std::cout << "----" << std::endl;	
}

static void tc14_5(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::ITALY, "Piedmont"), "Marseilles"));	
	p.addMove(new MovementMove(new ArmyPiece(Nation::ITALY, "Venice"), "Piedmont"));	
	p.addMove(new MovementMove(new ArmyPiece(Nation::FRANCE, "Marseilles"), "Piedmont"));	
	p.addMove(new MovementMove(new ArmyPiece(Nation::FRANCE, "Burgundy"), "Marseilles"));	
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc14_6(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::ITALY, "Piedmont"), "Marseilles"));	
	p.addMove(new MovementMove(new ArmyPiece(Nation::ITALY, "Venice"), "Piedmont"));	
	p.addMove(new MovementMove(new ArmyPiece(Nation::FRANCE, "Marseilles"), "Piedmont"));	
	p.addMove(new SupportMove(new ArmyPiece(Nation::GERMANY, "Burgundy"), "Piedmont", "Marseilles"));
	p.addMove(new SupportMove(new FleetPiece(Nation::FRANCE, "GulfOfLyons"), "Marseilles", "Marseilles"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc15(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::GERMANY, "Munich"), "Silesia"));
	p.addMove(new HoldMove(new ArmyPiece(Nation::GERMANY, "Berlin")));
	p.addMove(new MovementMove(new ArmyPiece(Nation::AUSTRIA, "Bohemia"), "Munich"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Prussia"), "Berlin"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::AUSTRIA, "Tyrolia"), "Bohemia", "Munich"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::RUSSIA, "Silesia"), "Prussia", "Berlin"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc16(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::ENGLAND, "London"), "Norway", true));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::ENGLAND, "NorthSea"), "London", "Norway"));
	p.processMoves();
	std::cout << "----" << std::endl;	
}

static void tc17(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::ENGLAND, "London"), "Tunis", true));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::ENGLAND, "EnglishChannel"), "London", "Tunis"));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::ENGLAND, "MidAtlantic"), "London", "Tunis"));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::ENGLAND, "WesternMed"), "London", "Tunis"));
	p.processMoves();
	std::cout << "----" << std::endl;	
}

static void tc18(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::FRANCE, "Spain"), "Naples", true));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::FRANCE, "GulfOfLyons"), "Spain", "Naples"));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::FRANCE, "TyrrhenianSea"), "Spain", "Naples"));
	p.addMove(new SupportMove(new FleetPiece(Nation::ITALY, "Tunis"), "IonianSea", "TyrrhenianSea"));
	p.addMove(new MovementMove(new FleetPiece(Nation::ITALY, "IonianSea"), "TyrrhenianSea"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc19(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::FRANCE, "Paris"), "Burgundy"));
	p.addMove(new HoldMove(new ArmyPiece(Nation::FRANCE, "Burgundy")));
	p.addMove(new SupportMove(new ArmyPiece(Nation::FRANCE, "Marseilles"), "Paris", "Burgundy"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc20(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::FRANCE, "Paris"), "Burgundy"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::FRANCE, "Burgundy"), "Marseilles"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::GERMANY, "Ruhr"), "Paris", "Burgundy"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::ITALY, "Marseilles"), "Burgundy"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc21(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::GERMANY, "Ruhr"), "Burgundy"));
	p.addMove(new HoldMove(new ArmyPiece(Nation::FRANCE, "Burgundy")));
	p.addMove(new SupportMove(new ArmyPiece(Nation::FRANCE, "Paris"), "Ruhr", "Burgundy"));
	p.addMove(new HoldMove(new ArmyPiece(Nation::GERMANY, "Munich")));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc22(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::AUSTRIA, "Tyrolia"), "Munich"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::GERMANY, "Munich"), "Tyrolia"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::AUSTRIA, "Bohemia"), "Silesia", "Munich"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::GERMANY, "Silesia"), "Munich"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::GERMANY, "Ruhr"), "Munich"));
	p.processMoves();
	std::cout << "----" << std::endl;
}	

static void tc23(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Berlin"), "Kiel"));
	p.addMove(new MovementMove(new FleetPiece(Nation::RUSSIA, "Skagerrak"), "Denmark"));
	p.addMove(new SupportMove(new FleetPiece(Nation::RUSSIA, "BalticSea"), "Skagerrak", "Denmark"));
	p.addMove(new MovementMove(new FleetPiece(Nation::GERMANY, "North_Sea"), "Denmark"));
	p.addMove(new SupportMove(new FleetPiece(Nation::GERMANY, "HelgolandBight"), "NorthSea", "Denmark"));
	p.addMove(new MovementMove(new FleetPiece(Nation::GERMANY, "Denmark"), "Kiel"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc24(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::AUSTRIA, "Vienna"), "Budapest"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::AUSTRIA, "Serbia"), "Budapest"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::RUSSIA, "Galicia"), "Serbia", "Budapest"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

// ERROR: should get solved when circular attacks paradox is solved
static void tc25(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::ENGLAND, "London"), "Belgium", true));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::ENGLAND, "NorthSea"), "London", "Belgium"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::FRANCE, "Belgium"), "London", true));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::FRANCE, "EnglishChannel"), "Belgium", "London"));
	p.processMoves();
	std::cout << "----" << std::endl;	
}

static void tc26(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::ENGLAND, "London"), "Belgium", true));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::ENGLAND, "NorthSea"), "London", "Belgium"));
	p.addMove(new MovementMove(new FleetPiece(Nation::FRANCE, "Brest"), "EnglishChannel"));
	p.addMove(new SupportMove(new FleetPiece(Nation::FRANCE, "IrishSea"), "Brest", "EnglishChannel"));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::ENGLAND, "EnglishChannel"), "London", "Belgium"));
	p.processMoves();
	std::cout << "----" << std::endl;	
}

// ERROR: prob fixed with convoy paradox
static void tc27(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::FRANCE, "Tunis"), "Naples", true));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::FRANCE, "TyrrhenianSea"), "Tunis", "Naples"));
	p.addMove(new MovementMove(new FleetPiece(Nation::ITALY, "IonianSea"), "TyrrhenianSea"));
	p.addMove(new SupportMove(new FleetPiece(Nation::ITALY, "Naples"), "IonianSea", "TyrrhenianSea"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc28(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::FRANCE, "Tunis"), "Naples", true));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::FRANCE, "TyrrhenianSea"), "Tunis", "Naples"));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::FRANCE, "IonianSea"), "Tunis", "Naples"));
	p.addMove(new MovementMove(new FleetPiece(Nation::ITALY, "Rome"), "TyrrhenianSea"));
	p.addMove(new SupportMove(new FleetPiece(Nation::ITALY, "Naples"), "Rome", "TyrrhenianSea"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc29(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::FRANCE, "Tunis"), "Naples", true));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::FRANCE, "TyrrhenianSea"), "Tunis", "Naples"));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::FRANCE, "IonianSea"), "Tunis", "Naples"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::FRANCE, "Apulia"), "Tunis", "Naples"));
	p.addMove(new MovementMove(new FleetPiece(Nation::ITALY, "Rome"), "TyrrhenianSea"));
	p.addMove(new SupportMove(new FleetPiece(Nation::ITALY, "Naples"), "Rome", "Tyrrhenian_Sea"));
	p.processMoves();
	std::cout << "----" << std::endl;
}
	
void runTests(Graph * g) {
	cout << endl << *g << endl;
	cout << endl << endl;
	map<Nation, vector<Piece *> > pieces = readInPieces();
	for(auto pair : pieces) {
		for(Piece * piece : pair.second) {
			cout << *piece << endl;
		}
	}
	auto it = pieces.find(Nation::GERMANY);
	int i = 0;
	Piece * piece = (it->second).at(i);
	while(piece->getLocation() != "Kiel") {
		i++;
		piece = (it->second).at(i);
	}
	piece = new FleetPiece(Nation::ENGLAND, "Bulgaria", "Bulgaria_SC");
	MovementMove * move = new MovementMove(piece, "Constantinople");
	if(move->isLegal(g)) {
		cout << "move is valid" << endl;
	} else {
		cout << "move is not valid" << endl;
	}
	
	cout << "about to start real tests" << endl;
	tc1(g);
	tc2(g);
	tc3(g);
	tc4(g);
	tc5(g);
	tc6(g);
	tc7(g);
	tc8(g);
	tc9(g);
	tc10(g);
	tc11(g);
	tc12(g);
	tc13(g);
	tc14(g);
	tc14_5(g);
	tc14_6(g);
	tc15(g);
	tc16(g);
	tc17(g);
	tc18(g);
	tc19(g);
	tc20(g);
	tc21(g);
	tc22(g);
	tc23(g);
	tc24(g);
	tc25(g);
	tc26(g);
	tc27(g);
	tc28(g);
	tc29(g);
	

	list<string> path = g->searchPath("Clyde", "London");
	for(string s : path) {
		cout << s << endl;
	}
}

int main(int argc, char ** argv) {
	if(argc == 1 || argc > 3) {
		assert(false);
	}
	string fileName(argv[1]);
	Graph * g = new Graph;
	//readInNodes(*g, fileName);
	if(argc == 2) {
		// normal testing
		runTests(g);
	} else if(argc == 3) {
		string data(argv[2]);
		// called as child process
		ChildProcess(g, data);
	}
	delete g;
}	
