#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <vector>
#include "Incidents.hpp"

class Graph{
	typedef std::unique_ptr<Incidents> Pointer;
	std::vector<Pointer> incidents;
public:
	//reachs ownership
	void addVertex(Incidents* vertexIncidents){
		incidents.push_back(Pointer(vertexIncidents));
	}
	
	void addEdge(size_t from, size_t to){
		incidents[from]->addEdge(to);
	}
	
	void removeEdge(size_t from, size_t to){
		incidents[from]->removeEdge(to);
	}
	
	bool checkEdge(size_t from, size_t to) const {
		return incidents[from]->checkEdge(to);
	}
	
	Incidents& getIncidents(size_t from) const {
		return *incidents[from];
	}
	
	size_t vertexCount() const {
		return incidents.size();
	}
};
#endif /* GRAPH_HPP */

