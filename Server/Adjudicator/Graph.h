#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <unordered_set>
#include <map>
#include <list>
#include <ostream>

using std::string;
using std::unordered_set;
using std::map;
using std::ostream;
using std::list;

class Territory;

class Graph {
	private:
		class Node {
			private:
				string id_;
				Territory * data_;
				// if holds Node* then we store the nodes several times: once in the map, and again in every node it neighbours
				unordered_set<string> armyNeighbours_;
				unordered_set<string> fleetNeighbours_;
				unordered_set<string> doubleCoastNeighbours_;
			public:
				Node(string id, Territory * data);
				~Node();
				void addArmyNeighbour(string key);
				void addFleetNeighbour(string key);
				void addDoubleCoastNeighbour(string key);
				void removeArmyNeighbour(string key);
				void removeFleetNeighbour(string key);
				string getId() const;
				Territory * getData() const;
				const unordered_set<string> & getArmyNeighbours() const;
				const unordered_set<string> & getFleetNeighbours() const;
				const unordered_set<string> & getDoubleCoastNeighbours() const;
				void outputArmyNeighbours(ostream & out) const;
				void outputFleetNeighbours(ostream & out) const;

				friend ostream & operator<<(ostream & out, const Node & node);
		};

		map<string, Node *> nodes_;

	public:
		Graph();
		~Graph();
		bool addNode(string id, Territory * data);
		bool addArmyEdge(string id1, string id2);
		bool addFleetEdge(string id1, string id2);
		bool addDoubleCoastEdge(string id1, string id2);
		bool removeNode(string data);
		bool removeArmyEdge(string id1, string id2);
		bool removeFleetEdge(string id1, string id2);
		Territory * getTerritory(string id) const;
		unordered_set<string> getArmyNeighbours(string id) const;
		unordered_set<string> getFleetNeighbours(string id) const;
		unordered_set<string> getDoubleCoastNeighbours(string id) const;
		list<string> searchPath(string id1, unordered_set<string> & destinations, bool isArmy) const;
		
		friend ostream & operator<<(ostream & out, const Graph::Node & node);
		friend ostream & operator<<(ostream & out, const Graph & graph);
};

#endif
