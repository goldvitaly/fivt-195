#ifndef GRAPH_HPP
#define GRAPH_HPP
#include "Incidents.hpp"
#include <vector>

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
	
	size_t size() const {
		return incidents.size();
	}
	Graph(){}
	Graph (Graph&& g){
		incidents = std::move(g.incidents);
	}
	Graph& operator=(Graph&& g){
		incidents = std::move(g.incidents);
		return *this;
	}
	Graph(const Graph&) = delete;
	Graph& operator=(const Graph&) = delete;
};
#endif /* GRAPH_HPP */

