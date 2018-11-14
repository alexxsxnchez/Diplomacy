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

using std::string;

class ChildProcess {
	private:
	
	public:
		ChildProcess(Graph * g, string data);
};