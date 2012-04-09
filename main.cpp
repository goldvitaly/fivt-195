#include "Graph.hpp"
#include "Incidents.hpp"
#include "MatrixIncidents.hpp"
#include "SetIncidents.hpp"
#include "StronglyConnectedComponents.hpp"
#include "TrivialStronglyConnectedComponents.hpp"
#include "VectorIncidents.hpp"
#include "StronglyConnectedComponentsInfo.hpp"
#include "ShortestPath.hpp"
#include <cstdlib>
#include <iostream>
#include <set>
#include <map>
#include <memory>
using namespace std;

typedef std::unique_ptr<Incidents<NoWeight> > PIncidents;
typedef std::vector<size_t> Coloring;

Graph<NoWeight> genGraph(int mask){
	Graph<NoWeight> graph;
	graph.addVertex(PIncidents(new VectorIncidents<NoWeight>()));
	graph.addVertex(PIncidents(new MatrixIncidents(4)));
	graph.addVertex(PIncidents(new VectorIncidents<NoWeight>()));
	graph.addVertex(PIncidents(new SetIncidents<NoWeight>()));
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
	for(size_t i = 0; i < a.size(); ++i){
		for(size_t j=0; j < b.size(); ++j){
			if((a[i]==a[j]) != (b[i]==b[j]))
				return false;
		}
	}
	return true;
}
bool testGraph(const Graph<NoWeight>& graph){
	Coloring received = StronglyConnectedComponents(graph).getComponents().ids();
	Coloring expected = TrivialStronglyConnectedComponents(graph).getColoring();
	return checkColoringsEqual(received, expected);
}

void testTarjan(){
	for(int i = 0; i < (1 << 16); ++i){
		if(!testGraph(genGraph(i))){
			cerr<<"Test failed on 4-vertex graph";
			exit (1);
		}
	}
}

void testDijkstra(){
	Graph<int> g;
	g.addVertex(std::unique_ptr<Incidents<int> >(new VectorIncidents<int>()));
	g.addVertex(std::unique_ptr<Incidents<int> >(new VectorIncidents<int>()));
	g.addEdge(0, 1, 42);
	ShortestPath<int> sp(g);
	cout<<*sp.calculate(0).length(1);
}
int main() {
	testTarjan();
	testDijkstra();
	return 0;
}