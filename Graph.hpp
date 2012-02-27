#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <vector>
#include "Incidents.hpp"

class Graph{
	std::vector<Incidents*> incidents;
public:
	//reachs ownership
	void addVertex(Incidents* vertixIncidents){
		incidents.push_back(vertixIncidents);
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
	
	Incidents* getIncidents(size_t from) const {
		return incidents[from];
	}
	
	size_t vertexCount() const {
		return incidents.size();
	}
	
	~Graph(){
		for(int i=0;i<incidents.size();++i){
			delete incidents[i];
		}
	}
};
#endif /* GRAPH_HPP */

