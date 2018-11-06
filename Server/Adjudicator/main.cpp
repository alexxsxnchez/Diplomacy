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
#include <cstdio>

using std::cin;
using std::cout;
using std::endl;
using std::istringstream;
using std::ifstream;
using std::map;
using std::vector;

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
	return pieces;
}


static void readInNodes(Graph & graph) {
	ifstream file;
	try {
		file.open("input.txt");
	} catch(...) {
		assert(false);
	}
	
	string in;
	while(getline(file, in)) {
		istringstream s(in);
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
				cout << "ERROR: " << third << endl;
				assert(false);
			}
			Territory::Type type;
			try {
				type = getTerritoryType(first);
			} catch(string incorrectString) {
				cout << "ERROR: " << incorrectString << endl;
				assert(false);
			}
			Territory * territory = new Territory(second, boolean, type);
			graph.addNode(second, territory);
		} else {
			cout << "ERROR: " << first << endl;
			assert(false);
		}
	}
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
	p.addMove(new MovementMove(new FleetPiece(Nation::FRANCE, "Gulf_of_Lyons"), "Tyrrehenian_Sea"));
	p.addMove(new MovementMove(new FleetPiece(Nation::ITALY, "Naples"), "Tyrrehenian_Sea"));
	p.addMove(new SupportMove(new FleetPiece(Nation::FRANCE, "Western_Mediterranean"), "Gulf_of_Lyons", "Tyrrehenian_Sea"));
	p.addMove(new SupportMove(new FleetPiece(Nation::ITALY, "Rome"), "Naples", "Tyrrehenian_Sea"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc7(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::GERMANY, "Silesia"), "Prussia"));
	p.addMove(new SupportMove(new FleetPiece(Nation::GERMANY, "Baltic_Sea"), "Silesia", "Prussia"));
	p.addMove(new HoldMove(new ArmyPiece(Nation::RUSSIA, "Prussia")));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc8(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new FleetPiece(Nation::FRANCE, "Gulf_of_Lyons"), "Tyrrehenian_Sea"));
	p.addMove(new HoldMove(new FleetPiece(Nation::ITALY, "Tyrrehenian_Sea")));
	p.addMove(new SupportMove(new FleetPiece(Nation::FRANCE, "Western_Mediterranean"), "Gulf_of_Lyons", "Tyrrehenian_Sea"));
	p.addMove(new SupportMove(new FleetPiece(Nation::ITALY, "Rome"), "Tyrrehenian_Sea", "Tyrrehenian_Sea"));
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
	p.addMove(new SupportMove(new FleetPiece(Nation::TURKEY, "Black_Sea"), "Bulgaria", "Rumania"));
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
	p.addMove(new MovementMove(new FleetPiece(Nation::RUSSIA, "Baltic_Sea"), "Prussia"));
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
	p.addMove(new SupportMove(new FleetPiece(Nation::FRANCE, "Gulf_of_Lyons"), "Marseilles", "Marseilles"));
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
	p.addMove(new ConvoyMove(new FleetPiece(Nation::ENGLAND, "North_Sea"), "London", "Norway"));
	p.processMoves();
	std::cout << "----" << std::endl;	
}

static void tc17(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::ENGLAND, "London"), "Tunis", true));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::ENGLAND, "English_Channel"), "London", "Tunis"));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::ENGLAND, "Mid-Atlantic_Ocean"), "London", "Tunis"));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::ENGLAND, "Western_Mediterranean"), "London", "Tunis"));
	p.processMoves();
	std::cout << "----" << std::endl;	
}

static void tc18(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::FRANCE, "Spain"), "Naples", true));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::FRANCE, "Gulf_of_Lyons"), "Spain", "Naples"));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::FRANCE, "Tyrrhenian_Sea"), "Spain", "Naples"));
	p.addMove(new SupportMove(new FleetPiece(Nation::ITALY, "Tunis"), "Ionian_Sea", "Tyrrhenian_Sea"));
	p.addMove(new MovementMove(new FleetPiece(Nation::ITALY, "Ionian_Sea"), "Tyrrhenian_Sea"));
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
	p.addMove(new SupportMove(new FleetPiece(Nation::RUSSIA, "Baltic_Sea"), "Skagerrak", "Denmark"));
	p.addMove(new MovementMove(new FleetPiece(Nation::GERMANY, "North_Sea"), "Denmark"));
	p.addMove(new SupportMove(new FleetPiece(Nation::GERMANY, "Heligoland_Bight"), "North_Sea", "Denmark"));
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
	p.addMove(new ConvoyMove(new FleetPiece(Nation::ENGLAND, "North_Sea"), "London", "Belgium"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::FRANCE, "Belgium"), "London", true));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::FRANCE, "English_Channel"), "Belgium", "London"));
	p.processMoves();
	std::cout << "----" << std::endl;	
}

static void tc26(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::ENGLAND, "London"), "Belgium", true));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::ENGLAND, "North_Sea"), "London", "Belgium"));
	p.addMove(new MovementMove(new FleetPiece(Nation::FRANCE, "Brest"), "English_Channel"));
	p.addMove(new SupportMove(new FleetPiece(Nation::FRANCE, "Irish_Sea"), "Brest", "English_Channel"));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::ENGLAND, "English_Channel"), "London", "Belgium"));
	p.processMoves();
	std::cout << "----" << std::endl;	
}

// ERROR: prob fixed with convoy paradox
static void tc27(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::FRANCE, "Tunis"), "Naples", true));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::FRANCE, "Tyrrhenian_Sea"), "Tunis", "Naples"));
	p.addMove(new MovementMove(new FleetPiece(Nation::ITALY, "Ionian_Sea"), "Tyrrhenian_Sea"));
	p.addMove(new SupportMove(new FleetPiece(Nation::ITALY, "Naples"), "Ionian_Sea", "Tyrrhenian_Sea"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc28(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::FRANCE, "Tunis"), "Naples", true));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::FRANCE, "Tyrrhenian_Sea"), "Tunis", "Naples"));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::FRANCE, "Ionian_Sea"), "Tunis", "Naples"));
	p.addMove(new MovementMove(new FleetPiece(Nation::ITALY, "Rome"), "Tyrrhenian_Sea"));
	p.addMove(new SupportMove(new FleetPiece(Nation::ITALY, "Naples"), "Rome", "Tyrrhenian_Sea"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc29(Graph * g) {
	MoveProcessor p(g);
	p.addMove(new MovementMove(new ArmyPiece(Nation::FRANCE, "Tunis"), "Naples", true));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::FRANCE, "Tyrrhenian_Sea"), "Tunis", "Naples"));
	p.addMove(new ConvoyMove(new FleetPiece(Nation::FRANCE, "Ionian_Sea"), "Tunis", "Naples"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::FRANCE, "Apulia"), "Tunis", "Naples"));
	p.addMove(new MovementMove(new FleetPiece(Nation::ITALY, "Rome"), "Tyrrhenian_Sea"));
	p.addMove(new SupportMove(new FleetPiece(Nation::ITALY, "Naples"), "Rome", "Tyrrhenian_Sea"));
	p.processMoves();
	std::cout << "----" << std::endl;
}
		
int main() {
	Graph * g = new Graph;
	printf("ok here we go");
	readInNodes(*g);
	printf("read in nodes");
	cout << endl << *g << endl;
	cout << endl << endl;
	map<Nation, vector<Piece *> > pieces = readInPieces();
	printf("read in pieces");
	for(auto pair : pieces) {
		printf("hahah");
		for(Piece * piece : pair.second) {
			printf("%s\n", piece->getLocation().c_str());
			cout << *piece << endl;
		}
	}

	printf("does it make it next");
	auto it = pieces.find(Nation::GERMANY);
	printf("prob here");
	int i = 0;
	Piece * piece = (it->second).at(i);
	printf("okkk...");
	while(piece->getLocation() != "Kiel") {
		i++;
		piece = (it->second).at(i);
	}
	printf("here?");
	piece = new FleetPiece(Nation::ENGLAND, "Bulgaria", "Bulgaria_SC");
	MovementMove * move = new MovementMove(piece, "Constantinople");
	if(move->isLegal(g)) {
		cout << "move is valid" << endl;
	} else {
		cout << "move is not valid" << endl;
	}
	printf("about to start real tests");
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
	// still need to delete pieces
//	delete g;
	return 0;
}


