#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include <vector>
#include <queue>
#include <utility>

class Dijkstra
{
	public:
		explicit Dijkstra(const Graph& graph):graph(graph)
		{			
		}
		std::vector<int> findPath(int s)
		{				
			const int INF = 1<<30;
			std::vector<int> dist(graph.size(), INF),  par(graph.size());
			dist[s] = 0;
			std::priority_queue <std::pair<int, int>> queue;
			queue.push (std::make_pair (0, s));
			while (!queue.empty()) 
			{
				int ver = queue.top().second,  cur_d = -queue.top().first;
				queue.pop();
				if (cur_d > dist[ver])  continue;
		 
				for (Incidents::Iterator j = graph.getIncidents(ver).begin(); j != graph.getIncidents(ver).end(); ++j) 
				{
					int to = (*j).to, weight = (*j).weight;
					if (dist[ver] + weight < dist[to])
					{
						dist[to] = dist[ver] + weight;
						par[to] = ver;
						queue.push(std::make_pair (-dist[to], to));
					}
					
				}
			}
			return dist;
		}
	private:
		const Graph &graph;					
};

#endif /* DIJKSTRA_H */
