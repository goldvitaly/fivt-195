#include "Graph.hpp"
#include "Incidents.hpp"
#include "SetIncidents.hpp"
#include "StronglyConnectedComponents.hpp"
#include "VectorIncidents.hpp"
#include "StronglyConnectedComponentsInfo.hpp"
#include <cstdlib>
#include <iostream>

using namespace std;
int main() {
	Graph graph;
	for(int i = 0; i < 5; ++i){
		graph.addVertex(new SetIncidents());
	}
	graph.addEdge(3, 4);
	graph.addEdge(4, 3);
	graph.addEdge(1, 4);
	graph.addEdge(1, 2);
	graph.addEdge(2, 0);
	graph.addEdge(0, 1);
	StronglyConnectedComponentsInfo scc (StronglyConnectedComponents(graph).getComponents());
	
	std::cout<<scc.getComponentsCount()<<endl;
	
	auto components = scc.allComponents();
	for(auto component: components){
		for(auto vertex: component){
			cout<<vertex<<' ';
		}
		cout<<endl;
	}

	return 0;
}

