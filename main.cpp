#include "Graph.hpp"
#include "Incidents.hpp"
#include "SetIncidents.hpp"
#include "StronglyConnectedComponents.hpp"
#include "TrivialStronglyConnectedComponents.hpp"
#include "VectorIncidents.hpp"
#include "StronglyConnectedComponentsInfo.hpp"
#include <cstdlib>
#include <iostream>
#include <set>
#include <map>
using namespace std;

typedef std::unique_ptr<Incidents> PIncidents;
typedef std::vector<size_t> Coloring;

Graph genGraph(int mask){
	Graph graph;
	graph.addVertex(PIncidents(new VectorIncidents()));
	graph.addVertex(PIncidents(new SetIncidents()));
	graph.addVertex(PIncidents(new VectorIncidents()));
	graph.addVertex(PIncidents(new SetIncidents()));
	for(int i=0; i < 4; ++i){
		for(int j = 0; j < 4; ++j){
			int curEdge = 4 * i + j;
			if(mask & (1 << curEdge)){
				graph.addEdge(i, j);
			}
		}
	}
	return graph;
}
bool checkColoringsEqual(const Coloring& a, const Coloring& b){
	assert(a.size()==b.size());
	map<size_t, set<size_t> > m;
	for(size_t i = 0; i < a.size(); ++i){
		m[a[i]].insert(b[i]);
	}
	for(auto& s: m){
		if(s.second.size() != 1)
			return false;
	}
	return true;
}
bool testGraph(const Graph& graph){
	Coloring received = StronglyConnectedComponents(graph).getComponents().ids();
	Coloring expected = TrivialStronglyConnectedComponents(graph).getColoring();
	return checkColoringsEqual(received, expected);
}
int main() {
	for(int i = 0; i < (1 << 16); ++i){
		if(!testGraph(genGraph(i))){
			cerr<<"Test failed on 4-vertex graph";
			exit (1);
		}
	}
	return 0;
}