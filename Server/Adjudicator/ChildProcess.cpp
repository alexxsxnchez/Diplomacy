#include "ChildProcess.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include "Nation.h"

using json = nlohmann::json;
using std::string;
using std::cout;
using std::endl;

ChildProcess::ChildProcess(Graph * g, string data) {
	MoveProcessor p(g);
	std::cerr << "child process constructor" << std::endl;
	//std::cout << data << std::endl << std::endl<<std::endl;
	
	json j = json::parse(data);
	for (json::iterator it = j.begin(); it != j.end(); ++it) {
		//std::cout << *it << std::endl << std::endl;
		string firstLoc = it.key();
		string unitType = it.value()["unit"]["type"];
		string nationStr = it.value()["unit"]["nation"];
		Nation nationality = getNation(nationStr);
		
		Piece * piece;
		if(unitType == "army") {
			piece = new ArmyPiece(nationality, firstLoc);
		} else if(unitType == "fleet") {
			piece = new FleetPiece(nationality, firstLoc); // coast specifier
		} else {
			assert(false);
		}	
		
		string moveType = it.value()["moveType"];
		if(moveType == "HOLD") {
			HoldMove * move = new HoldMove(piece);
			std::cerr << *move << std::endl;
			p.addMove(move);
		} else if(moveType == "MOVE") {
			string secondLoc = it.value()["secondLoc"];
			MovementMove * move = new MovementMove(piece, secondLoc); // need to add convoy specifier
			std::cerr << *move << std::endl;
			p.addMove(move);
		} else if(moveType == "SUPPORT") {
			string secondLoc = it.value()["secondLoc"];
			string thirdLoc = it.value()["thirdLoc"];
			SupportMove * move = new SupportMove(piece, secondLoc, thirdLoc);
			std::cerr << *move << std::endl;
			p.addMove(move);
		} else if(moveType == "CONVOY") {
			string secondLoc = it.value()["secondLoc"];
			string thirdLoc = it.value()["thirdLoc"];
			ConvoyMove * move = new ConvoyMove(piece, secondLoc, thirdLoc);
			std::cerr << *move << std::endl;
			p.addMove(move);
		}
	}
	
	MoveProcessor::Results results = p.processMoves();
	outputResults(results);
}

void ChildProcess::outputResults(MoveProcessor::Results results) {
	cout << "{";
	bool first = true;
	for(auto it : results) {
		if(!first) {
			cout << ", ";
		} else {
			first = false;
		}
		cout << "\"" << it.first << "\": {\"success\": ";
		if(it.second.first) {
			cout << "true";
		} else {
			cout << "false";
		}
		cout << ", \"description\": \"" << it.second.second << "\"}";
	}
	cout << "}" << endl;
}