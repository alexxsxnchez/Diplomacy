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
	string in;
	while(getline(cin, in)) {
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

static void tc1() {
	MoveProcessor p;
	p.addMove(new MovementMove(new ArmyPiece(Nation::GERMANY, "Berlin"), "Silesia"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Warsaw"), "Silesia"));
	p.processMoves();
	std::cout << "----done tc1" << std::endl;
}

static void tc2() {
	MoveProcessor p;
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Warsaw"), "Prussia"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Moscow"), "Warsaw"));
	p.addMove(new HoldMove(new ArmyPiece(Nation::GERMANY, "Prussia")));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc3() {
	MoveProcessor p;
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Warsaw"), "Livonia"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Moscow"), "Warsaw"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::GERMANY, "Livonia"), "Moscow"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc4() {
	MoveProcessor p;
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Warsaw"), "Prussia"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Prussia"), "Warsaw"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc5() {
	MoveProcessor p;
	p.addMove(new MovementMove(new ArmyPiece(Nation::GERMANY, "Burgundy"), "Gascony"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::FRANCE, "Marseilles"), "Burgundy"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::FRANCE, "Gascony"), "Marseilles", "Burgundy"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc6() {
	MoveProcessor p;
	p.addMove(new MovementMove(new FleetPiece(Nation::FRANCE, "Gulf_of_Lyons"), "Tyrrehenian_Sea"));
	p.addMove(new MovementMove(new FleetPiece(Nation::ITALY, "Naples"), "Tyrrehenian_Sea"));
	p.addMove(new SupportMove(new FleetPiece(Nation::FRANCE, "Western_Mediterranean"), "Gulf_of_Lyons", "Tyrrehenian_Sea"));
	p.addMove(new SupportMove(new FleetPiece(Nation::ITALY, "Rome"), "Naples", "Tyrrehenian_Sea"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc7() {
	MoveProcessor p;
	p.addMove(new MovementMove(new ArmyPiece(Nation::GERMANY, "Silesia"), "Prussia"));
	p.addMove(new SupportMove(new FleetPiece(Nation::GERMANY, "Baltic_Sea"), "Silesia", "Prussia"));
	p.addMove(new HoldMove(new ArmyPiece(Nation::RUSSIA, "Prussia")));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc8() {
	MoveProcessor p;
	p.addMove(new MovementMove(new FleetPiece(Nation::FRANCE, "Gulf_of_Lyons"), "Tyrrehenian_Sea"));
	p.addMove(new HoldMove(new FleetPiece(Nation::ITALY, "Tyrrehenian_Sea")));
	p.addMove(new SupportMove(new FleetPiece(Nation::FRANCE, "Western_Mediterranean"), "Gulf_of_Lyons", "Tyrrehenian_Sea"));
	p.addMove(new SupportMove(new FleetPiece(Nation::ITALY, "Rome"), "Tyrrehenian_Sea", "Tyrrehenian_Sea"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc9() {
	MoveProcessor p;
	p.addMove(new MovementMove(new ArmyPiece(Nation::AUSTRIA, "Bohemia"), "Munich"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::GERMANY, "Munich"), "Silesia"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Warsaw"), "Silesia"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::AUSTRIA, "Tyrolia"), "Bohemia", "Munich"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::GERMANY, "Berlin"), "Munich", "Silesia"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::RUSSIA, "Prussia"), "Warsaw", "Silesia"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc10() {
	MoveProcessor p;
	p.addMove(new MovementMove(new ArmyPiece(Nation::TURKEY, "Bulgaria"), "Rumania"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Rumania"), "Bulgaria"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Sevastopol"), "Rumania"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::RUSSIA, "Serbia"), "Rumania", "Bulgaria"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc11() {
	MoveProcessor p;
	p.addMove(new MovementMove(new ArmyPiece(Nation::TURKEY, "Bulgaria"), "Rumania"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Rumania"), "Bulgaria"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Sevastopol"), "Rumania"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::RUSSIA, "Serbia"), "Rumania", "Bulgaria"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::RUSSIA, "Greece"), "Rumania", "Bulgaria"));
	p.addMove(new SupportMove(new FleetPiece(Nation::TURKEY, "Black_Sea"), "Bulgaria", "Rumania"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc12() {
	MoveProcessor p;
	p.addMove(new MovementMove(new ArmyPiece(Nation::GERMANY, "Prussia"), "Warsaw"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::GERMANY, "Silesia"), "Prussia", "Warsaw"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Bohemia"), "Silesia"));
	p.addMove(new HoldMove(new ArmyPiece(Nation::RUSSIA, "Warsaw")));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc13() {
	MoveProcessor p;
	p.addMove(new MovementMove(new ArmyPiece(Nation::GERMANY, "Prussia"), "Warsaw"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::GERMANY, "Silesia"), "Prussia", "Warsaw"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Warsaw"), "Silesia"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc14() {
	MoveProcessor p;
	p.addMove(new MovementMove(new FleetPiece(Nation::GERMANY, "Berlin"), "Prussia"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::GERMANY, "Silesia"), "Berlin", "Prussia"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Prussia"), "Silesia"));
	p.addMove(new MovementMove(new FleetPiece(Nation::RUSSIA, "Baltic_Sea"), "Prussia"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::RUSSIA, "Warsaw"), "Prussia", "Silesia"));
	p.processMoves();
	std::cout << "----" << std::endl;	
}

static void tc14_5() {
	MoveProcessor p;
	p.addMove(new MovementMove(new ArmyPiece(Nation::ITALY, "Piedmont"), "Marseilles"));	
	p.addMove(new MovementMove(new ArmyPiece(Nation::ITALY, "Venice"), "Piedmont"));	
	p.addMove(new MovementMove(new ArmyPiece(Nation::FRANCE, "Marseilles"), "Piedmont"));	
	p.addMove(new MovementMove(new ArmyPiece(Nation::FRANCE, "Burgundy"), "Marseilles"));	
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc14_6() {
	MoveProcessor p;
	p.addMove(new MovementMove(new ArmyPiece(Nation::ITALY, "Piedmont"), "Marseilles"));	
	p.addMove(new MovementMove(new ArmyPiece(Nation::ITALY, "Venice"), "Piedmont"));	
	p.addMove(new MovementMove(new ArmyPiece(Nation::FRANCE, "Marseilles"), "Piedmont"));	
	p.addMove(new SupportMove(new ArmyPiece(Nation::GERMANY, "Burgundy"), "Piedmont", "Marseilles"));
	p.addMove(new SupportMove(new FleetPiece(Nation::FRANCE, "Gulf_of_Lyons"), "Marseilles", "Marseilles"));
	p.processMoves();
	std::cout << "----" << std::endl;
}

static void tc15() {
	MoveProcessor p;
	p.addMove(new MovementMove(new ArmyPiece(Nation::GERMANY, "Munich"), "Silesia"));
	p.addMove(new HoldMove(new ArmyPiece(Nation::GERMANY, "Berlin")));
	p.addMove(new MovementMove(new ArmyPiece(Nation::AUSTRIA, "Bohemia"), "Munich"));
	p.addMove(new MovementMove(new ArmyPiece(Nation::RUSSIA, "Prussia"), "Berlin"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::AUSTRIA, "Tyrolia"), "Bohemia", "Munich"));
	p.addMove(new SupportMove(new ArmyPiece(Nation::RUSSIA, "Silesia"), "Prussia", "Berlin"));
	p.processMoves();
	std::cout << "----" << std::endl;
}


int main() {
	Graph * g = new Graph;
	readInNodes(*g);
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

	tc1();
	tc2();
	tc3();
	tc4();
	tc5();
	tc6();
	tc7();
	tc8();
	tc9();
	tc10();
	tc11();
	tc12();
	tc13();
	tc14();
	tc14_5();
	tc14_6();
	tc15();

	list<string> path = g->searchPath("Clyde", "London");
	for(string s : path) {
		cout << s << endl;
	}
	// still need to delete pieces
//	delete g;
	return 0;
}


