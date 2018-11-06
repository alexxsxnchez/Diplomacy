{
	"targets": [
		{
			"target_name": "standalone",
			"type": "executable",
			"sources": [ "main.cpp", "MoveProcessor.cpp", "MovementMove.cpp", "Piece.cpp", "ArmyPiece.cpp", "FleetPiece.cpp", "ConvoyMove.cpp", "Graph.cpp", "HoldMove.cpp", "Move.cpp", "SupportMove.cpp", "Territory.cpp", "pieces.txt" ],
			"cflags": ["-Wall", "-std=c++14"],
			"conditions": [
				[ "OS=='mac'", {
						"xcode_settings": {
							"OTHER_CPLUSPLUSFLAGS" : ["-std=c++14","-stdlib=libc++"],
							"OTHER_LDFLAGS": ["-stdlib=libc++"],
							"MACOSX_DEPLOYMENT_TARGET": "10.7",
							"GCC_ENABLE_CPP_EXCEPTIONS": "YES"
						}
					}
				]
			]
		}
	]
}
