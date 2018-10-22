#include "Graph.h"
#include "Territory.h"
#include <iostream>
#include <unordered_set>
#include <queue>

using std::queue;
using std::cout;
using std::endl;
using std::unordered_set;

// Node class
Graph::Node::Node(string id, Territory * data) : id_{id}, data_{data} {}

Graph::Node::~Node() {
	delete data_;
}

void Graph::Node::addArmyNeighbour(string id) {
	armyNeighbours_.insert(id);
}

void Graph::Node::addFleetNeighbour(string id) {
	fleetNeighbours_.insert(id);
}

void Graph::Node::removeArmyNeighbour(string id) {
	armyNeighbours_.erase(id);
}

void Graph::Node::removeFleetNeighbour(string id) {
	fleetNeighbours_.erase(id);
}

string Graph::Node::getId() const {
	return id_;
}

Territory * Graph::Node::getData() const {
	return data_;
}

const unordered_set<string> & Graph::Node::getArmyNeighbours() const {
	return armyNeighbours_;
}

const unordered_set<string> & Graph::Node::getFleetNeighbours() const {
	return fleetNeighbours_;
}

void Graph::Node::outputArmyNeighbours(ostream & out) const {
}

void Graph::Node::outputFleetNeighbours(ostream & out) const {
}

ostream & operator<<(ostream & out, const Graph::Node & node) {
	out << "--------" << endl;
	out << "ID: " << node.id_ << endl;
	out << "DATA: " << endl << *node.getData();
	out << "ARMY NEIGHBOURS:" << endl;
	for(auto it : node.armyNeighbours_) {
		out << "\t" << it << endl; 
	}
	out << endl;
	return out;
}

// Graph
Graph::Graph() {}

Graph::~Graph() {
	for(auto pair : nodes_) {
		delete pair.second;
	}
}

bool Graph::addNode(string id, Territory * data) {
	if(nodes_.find(id) != nodes_.end()) {
		return false;
	}
	Node * node = new Node(id, data);
	nodes_.insert(std::make_pair(id, node));
	return true;
}

bool Graph::removeNode(string id) {
	auto it = nodes_.find(id);
	if(it == nodes_.end()) {
		return false;
	}
	nodes_.erase(it);
	return true;
}

bool Graph::addArmyEdge(string id1, string id2) {
	auto it1 = nodes_.find(id1);
	auto it2 = nodes_.find(id2);
	if(it1 == nodes_.end() || it2 == nodes_.end()) {
		return false;
	}
	it1->second->addArmyNeighbour(id2);
	it2->second->addArmyNeighbour(id1);
	return true;
}

bool Graph::addFleetEdge(string id1, string id2) {
	auto it1 = nodes_.find(id1);
	auto it2 = nodes_.find(id2);
	if(it1 == nodes_.end() || it2 == nodes_.end()) {
		return false;
	}
	it1->second->addFleetNeighbour(id2);
	it2->second->addFleetNeighbour(id1);
	return true;
}

bool Graph::removeArmyEdge(string id1, string id2) {
	auto it1 = nodes_.find(id1);
	auto it2 = nodes_.find(id2);
	if(it1 == nodes_.end() || it2 == nodes_.end()) {
		return false;
	}
	it1->second->removeArmyNeighbour(id2);
	it2->second->removeArmyNeighbour(id1);
	return true;
}

bool Graph::removeFleetEdge(string id1, string id2) {
	auto it1 = nodes_.find(id1);
	auto it2 = nodes_.find(id2);
	if(it1 == nodes_.end() || it2 == nodes_.end()) {
		return false;
	}
	it1->second->removeFleetNeighbour(id2);
	it2->second->removeFleetNeighbour(id1);
	return true;
}

Territory * Graph::getTerritory(string id) const {
	auto it = nodes_.find(id);
	if(it == nodes_.end()) {
		return nullptr;
	}
	return it->second->getData();
};

unordered_set<string> Graph::getArmyNeighbours(string id) const {
	auto it = nodes_.find(id);
	if(it == nodes_.end()) {
		return unordered_set<string>();
	}
	return it->second->getArmyNeighbours();
}

unordered_set<string> Graph::getFleetNeighbours(string id) const {
	auto it = nodes_.find(id);
	if(it == nodes_.end()) {
		return unordered_set<string>();
	}
	return it->second->getFleetNeighbours();
}

list<string> Graph::searchPath(string src, string dest) const {
	list<string> list;
	if(src == dest) {
		list.push_back(src);
		return list;
	}
	auto it1 = nodes_.find(src);
	auto it2 = nodes_.find(dest);
	if(it1 == nodes_.end() || it2 == nodes_.end()) {
		return list;
	}

	Node * srcNode = it1->second;

	map<string, string> visited;
	queue<Node *> queue;
	queue.push(srcNode);
	visited.insert(std::make_pair(srcNode->getId(), ""));

	while(!queue.empty()) {
		Node * node = queue.front();
		queue.pop();
		if(node->getId() == dest) {
			list.push_front(node->getId());
			string parent;
			while((parent = visited.find(node->getId())->second) != "") {
				node = nodes_.find(parent) -> second;
				list.push_front(parent);
			}
			return list;
		}
		for(string id : node->getArmyNeighbours()) {
			if(visited.find(id) == visited.end()) {
				Node * neighbour = nodes_.find(id) -> second;
				visited.insert(std::make_pair(id, node->getId()));
				queue.push(neighbour);
			}
		}
	}
	return list;
}

ostream & operator<<(ostream & out, const Graph & graph) {
	for(auto pair : graph.nodes_) {
		out << *(pair.second);
	}
	return out;
}
