#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include <vector>
#include <queue>
#include <utility>
#include <functional>


template<class Weight>
class DijkstraInfo
{	
	public:
		DijkstraInfo(const std::vector<Weight> &dist,const std::vector<int> &par,const std::vector<char> &used) :
			dist(dist), par(par), used(used)
		{
			
		}
		std::vector<int> findPath(int f)
		{
			std::vector<int> path;
			while(par[f] != -1)
			{
				path.push_back(f);
				f = par[f];				
			}
			path.push_back(f);
			std::reverse(path.begin(), path.end());
			return path;
		}
		Weight distTo(int f)
		{
			return dist[f];
		}
		bool isPath(int f)
		{
			return (used[f]);
		}
		
	private:
		std::vector<Weight> dist;	
		std::vector<int> par;
		std::vector<char> used;					
};

template<class Weight> 
class Dijkstra
{
	public:
		explicit Dijkstra(const Graph<Weight>& graph):graph(graph)
		{			
		}
		DijkstraInfo<Weight> doDijkstra(int s)
		{
			used.resize(graph.size(), false);
			dist.resize(graph.size());
			par.resize(graph.size());
			std::priority_queue <std::pair<Weight, int>, std::vector<std::pair<Weight, int>>, std::greater<std::pair<Weight, int>>> queue;
			dist[s] = 0;
			used[s] = true;
			par[s] = -1;
			queue.push (std::make_pair (0, s));
			while (!queue.empty()) 
			{
				int ver = queue.top().second;
				Weight cur_d = queue.top().first;
				queue.pop();
				if (cur_d > dist[ver])
					continue;
				for (typename Incidents<Weight>::Iterator j = graph.getIncidents(ver).begin(); j != graph.getIncidents(ver).end(); ++j) 
				{
					int to = (*j).to;
					Weight weight = (*j).weight;
					if (!used[to] || dist[ver] + weight < dist[to])
					{
						used[to] = true;
						dist[to] = dist[ver] + weight;
						par[to] = ver;
						queue.push(std::make_pair (dist[to], to));
					}					
				}
			}
			return DijkstraInfo<Weight>(dist, par, used); 
		}
		
	private:
		const Graph<Weight> &graph;
		std::vector<Weight> dist;	
		std::vector<int> par;
		std::vector<char> used;
							
};

#endif /* DIJKSTRA_H */
