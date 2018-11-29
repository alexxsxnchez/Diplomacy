#include "ChildProcess.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include "Nation.h"
#include <map>
#include <unordered_set>

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
		string unitCoastSpecifier = "";
		if(it.value()["unit"]["coast"].is_string()) {
			unitCoastSpecifier = it.value()["unit"]["coast"];
		}
		std::cerr << "COAST: " << unitCoastSpecifier << std::endl; 
		Nation nationality = getNation(nationStr);
		
		Piece * piece;
		if(unitType == "army") {
			piece = new ArmyPiece(nationality, firstLoc);
		} else if(unitType == "fleet") {
			piece = new FleetPiece(nationality, firstLoc, unitCoastSpecifier); // coast specifier
		} else {
			assert(false);
		}	
		
		string moveType = it.value()["moveType"];
		string moveCoastSpecifier = "";
		if(it.value()["coast"].is_string()) {
			moveCoastSpecifier = it.value()["coast"];
		}
		if(moveType == "HOLD") {
			HoldMove * move = new HoldMove(piece);
			std::cerr << *move << std::endl;
			p.addMove(move);
		} else if(moveType == "MOVE") {
			string secondLoc = it.value()["secondLoc"];
			bool viaConvoy = it.value()["viaConvoy"];
			MovementMove * move = new MovementMove(piece, secondLoc, moveCoastSpecifier, viaConvoy); // need to add convoy specifier
			std::cerr << *move << std::endl;
			p.addMove(move);
		} else if(moveType == "SUPPORT") {
			string secondLoc = it.value()["secondLoc"];
			string thirdLoc = it.value()["thirdLoc"];
			SupportMove * move = new SupportMove(piece, secondLoc, thirdLoc, moveCoastSpecifier);
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
	p.outputResults(results, cout);
}

ChildProcess::ChildProcess(Graph * g, string units, string destroyCounts) {
	json unitsJSON = json::parse(units);
	json destroyCountsJSON = json::parse(destroyCounts);	
	std::map<string, std::map<string, unsigned int> > distances;
	
	for(json::iterator it = unitsJSON.begin(); it != unitsJSON.end(); it++) {
		string nation = it.value()["nation"];
		unsigned int destroyCount = destroyCountsJSON[nation];
		if(destroyCount == 0) {
			continue;
		}
		
		Nation nationality = getNation(nation);
		string unitType = it.value()["type"];
		string location = it.key();
		if(it.value()["coast"].is_string()) {
			string unitCoastSpecifier = it.value()["coast"];
			location = location + "_" + unitCoastSpecifier;
		}
		std::cerr << "hello3" << std::endl;
		
		unsigned int distance = calculateDistanceToHome(g, location, nationality, unitType);
		std::cerr << "hello4 distance: " << distance << std::endl;
		auto distanceForNationMap = distances.find(nation);
		if(distanceForNationMap == distances.end()) {
			std::map<string, unsigned int> distanceMap;
			distances.insert(std::make_pair(nation, distanceMap));
			distanceForNationMap = distances.find(nation);
		}
		if(distanceForNationMap->second.size() == destroyCount) {
			unsigned int smallestDistance = 1000;
			string smallestDistanceLocation = "";
			for(auto it2 = distanceForNationMap->second.begin(); it2 != distanceForNationMap->second.end(); it2++) {
				if(it2->second < smallestDistance) {
					smallestDistance = it2->second;
					smallestDistanceLocation = it2->first;
				}
			}
			if(distance > smallestDistance) {
				distanceForNationMap->second.erase(smallestDistanceLocation);
				distanceForNationMap->second.insert(std::make_pair(location, distance));
			}
		} else {
			distanceForNationMap->second.insert(std::make_pair(location, distance));
		}
	}
	outputDestroyUnitResults(distances);
}

unsigned int ChildProcess::calculateDistanceToHome(Graph * g, string location, Nation nation, string unitType) const {

	std::unordered_set<string> centres;
	switch(nation) {
		case RUSSIA:
			centres.insert("StPetersburg");
			centres.insert("StPetersburg_SC");
			centres.insert("StPetersburg_NC");
			centres.insert("Warsaw");
			centres.insert("Moscow");
			centres.insert("Sevastopol");
			break;
	
		case TURKEY:
			centres.insert("Ankara");
			centres.insert("Smyrna");
			centres.insert("Constantinople");
			break;

		case ITALY:
			centres.insert("Venice");
			centres.insert("Rome");
			centres.insert("Naples");
			break;

		case AUSTRIA:
			centres.insert("Vienna");
			centres.insert("Budapest");
			centres.insert("Trieste");
			break;

		case GERMANY:
			centres.insert("Berlin");
			centres.insert("Kiel");
			centres.insert("Munich");
			break;

		case FRANCE:
			centres.insert("Paris");
			centres.insert("Marseilles");
			centres.insert("Brest");
			break;

		case ENGLAND:
			centres.insert("Liverpool");
			centres.insert("Edinburgh");
			centres.insert("London");
			break;
	}
	
	bool isArmy = unitType == "army";
	return g->searchPath(location, centres, isArmy).size();
}

void ChildProcess::outputDestroyUnitResults(std::map<string, std::map<string, unsigned int> > distances) const {
	cout << "[" << endl;
	bool first = true;
	for(auto it : distances) {
		bool first = true;
		for(auto it2 : it.second) {
			if(!first) {
				cout << "," << endl;
			} else {
				first = false;
			}
			cout << "\t\"" << it2.first << "\"";
		}
	}
	cout << endl <<"]";
}

