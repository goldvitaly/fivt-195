#ifndef STRONGLYCONNECTEDCOMPONENTS_HPP
#define STRONGLYCONNECTEDCOMPONENTS_HPP

#include "Graph.hpp"
#include "StronglyConnectedComponentsInfo.hpp"
#include <algorithm>
#include <limits>
#include <stack>
#include <vector>
class StronglyConnectedComponents{
public:
	explicit StronglyConnectedComponents(const Graph& graph): graph(graph) {}
	StronglyConnectedComponentsInfo getComponents() {
		size = graph.size();
		used.assign(size,false);
		components.assign(size, -1);
		timer = 0;
		componentsCount = 0;
		mintime.assign(size, 0);
		for(size_t i=0;i < size; ++i){
			if(!used[i])
				dfs(i);
		}
		return StronglyConnectedComponentsInfo(componentsCount, components);
	}
private:
	void dfs(size_t v){
		mintime[v] = ++timer;
		size_t time = mintime[v];
		used[v] = true;
		byTimeIn.push(v);
		for(auto vert: graph.getIncidents(v)){
			if(!used[vert])
				dfs(vert);
			if(components[vert] == dummyComponent)
				mintime[v] = std::min(mintime[v], mintime[vert]);
		}
		if(mintime[v] == time){
			do{
				components[byTimeIn.top()] = componentsCount;
				byTimeIn.pop();
			} while (components[v] == dummyComponent);
			++componentsCount;
		}
	}
	size_t size;
	const static size_t dummyComponent = std::numeric_limits<size_t>::max();
	std::vector<char> used;
	std::vector<size_t> components;
	size_t timer;
	std::vector<size_t> mintime;
	size_t componentsCount;
	std::stack<size_t> byTimeIn;
	const Graph& graph;
};

#endif /* STRONGLYCONNECTEDCOMPONENTS_HPP */