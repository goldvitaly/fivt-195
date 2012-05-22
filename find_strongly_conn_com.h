#ifndef FIND_STRONGLY_CONNCOM_H
#define FIND_STRONGLY_CONNCOM_H

#include "incidents.h"
#include "graph.h"
#include <stack>
#include <vector>

class FindStronglyConnCom 
{
	public:

		explicit FindStronglyConnCom (const Graph& graph) : graph(graph) 
		{
			noComponent = -1;
		}

		std::vector<int> components() 
		{
			curComponent.assign(graph.size(), noComponent);
			used.assign(graph.size(), false);
			minTime.assign(graph.size(), 0);
			numComponents = 0;
			time = 0;
			for (int i = 0; i < (int) graph.size(); ++i) 
			{
				if (!used[i])
					dfs(i);
			}
			return curComponent;
		}

	private:

		int noComponent;
		int time;
		int numComponents;
		std::vector<char> used;
		std::vector<int> curComponent;
		std::vector<int> minTime;
		std::stack<int> started;
		const Graph& graph;
		void dfs(int vertex) 
		{
			minTime[vertex] = ++time;
			int startTime = minTime[vertex];
			used[vertex] = true;
			started.push(vertex);
			for (int to : graph.getIncidents(vertex)) 
			{
				if (!used[to])
					dfs(to);
				if (curComponent[to] == noComponent)
					minTime[vertex] = std::min(minTime[vertex], minTime[to]);
			}
			if (minTime[vertex] == startTime) 
			{
				while (curComponent[vertex] == noComponent)
				{
					curComponent[started.top()] = numComponents;
					started.pop();
				}
				++numComponents;
			}
		}
};

#endif /* FIND_STRONGLY_CONNCOM_H */
