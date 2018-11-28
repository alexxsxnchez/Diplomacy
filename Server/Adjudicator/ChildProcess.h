#include "MoveProcessor.h"
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
#include <string>
#include <map>

using std::string;

class ChildProcess {
	private:
		unsigned int calculateDistanceToHome(Graph * g, string location, Nation nation, string unitType) const;
		void outputDestroyUnitResults(std::map<string, std::map<string, unsigned int> > distances) const;
	public:
		// main adjudication
		ChildProcess(Graph * g, string data);
		// program to determine which units to auto destroy 
		ChildProcess(Graph * g, string units, string destroyCounts);
};