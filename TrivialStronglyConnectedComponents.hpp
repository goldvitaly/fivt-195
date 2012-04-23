#ifndef TRIVIALSTRONGLYCONNECTEDCOMPONENTS_HPP
#define TRIVIALSTRONGLYCONNECTEDCOMPONENTS_HPP
#include "Graph.hpp"
#include "StronglyConnectedComponentsInfo.hpp"
#include <algorithm>
#include <limits>
#include <stack>
#include <vector>
class TrivialStronglyConnectedComponents{
public:
	explicit TrivialStronglyConnectedComponents(const Graph<NoWeight>& graph): graph(graph) {}
	std::vector<size_t> getColoring() {
		int n = graph.size();
		possible.assign(n,std::vector<char>(n, false));
		for(int i = 0; i < n;++i){
			dfs(i, i);
		}
		std::vector<size_t> v;
		for(int i = 0; i < n; ++i){
			v.push_back(i);
		}
		for(int i = 0; i < n; ++i){
			for(int j = 0; j < n; ++j){
				if(possible[i][j] && possible[j][i]){
					v[j] = v[i];
				}
			}
		}
		return v;
	}
private:
	void dfs(size_t start, size_t v){
		possible[start][v] = true;
		for(const auto& next: graph.getIncidents(v)){
			if(!possible[start][next.id])
				dfs(start, next.id);
		}
	}
	std::vector<std::vector<char> > possible;
	const Graph<NoWeight>& graph;
};

#endif /* TRIVIALSTRONGLYCONNECTEDCOMPONENTS_HPP */

