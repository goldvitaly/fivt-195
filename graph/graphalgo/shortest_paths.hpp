#ifndef __SHORTEST_PATHS_H__
#define __SHORTEST_PATHS_H__

#include <vector>

namespace graph
{
namespace algorithm
{

	template <class PathInfo, class GraphEdgeType> 
	class Path
	{
		public:
			std::vector<unsigned int> path; // vertices in path
			std::vector<typename graph::impl::Graph<GraphEdgeType>::edge_type*> edges;
			boost::optional<PathInfo> path_length;
			Path() { };
			Path(const std::vector<unsigned int>& path, 
					std::vector<typename graph::impl::Graph<GraphEdgeType>::edge_type*> edges,
					PathInfo path_length): path(path), edges(edges), path_length(path_length)
			{
			}
			Path(const std::vector<unsigned int>& path, 
					std::vector<typename graph::impl::Graph<GraphEdgeType>::edge_type*> edges): path(path), edges(edges)
			{
			}
			operator bool() const { 
				return path.size() > 0; 
			} 
	};

	template <class PathInfo, class GraphEdgeType> 
	class ShortestPathsInfo
	{
		template <class _PathInfo, class PathRecountFunc, class PathInfoComparator, class WeightType>
		friend ShortestPathsInfo<_PathInfo,WeightType> find_shortest_paths(WeightedGraph<WeightType>&, unsigned int, 
				PathRecountFunc, PathInfoComparator, _PathInfo);
		private:
			unsigned int from;
			std::vector <PathInfo> dist;
			std::vector<char> used;
			std::vector< typename graph::impl::Graph<GraphEdgeType>::edge_type* > edges_to_vertex;
			std::vector <unsigned int> parents; // parent[from] == from
			ShortestPathsInfo() {};
			ShortestPathsInfo(size_t len, PathInfo initial_dist, unsigned int from): 
				used(len, 0), parents(len, 0), from(from), dist(len, initial_dist), edges_to_vertex(len, NULL)
			{
			}
		public:
			Path<PathInfo, GraphEdgeType> get_path(int to) const
			{
				if (!used[to])
					return Path<PathInfo,GraphEdgeType>(std::vector<unsigned int>(), 
							std::vector<typename graph::impl::Graph<GraphEdgeType>::edge_type*>());
				std::vector<unsigned int> resulted_path;
				std::vector<typename graph::impl::Graph<GraphEdgeType>::edge_type*> edges;
				int v = to;
				while (parents[v] != v)
				{
					resulted_path.push_back(v);
					edges.push_back(edges_to_vertex[v]);
					v = parents[v];
				}
				resulted_path.push_back(v);
				std::reverse(edges.begin(), edges.end());
				std::reverse(resulted_path.begin(), resulted_path.end());
				return Path<PathInfo, GraphEdgeType>(resulted_path, edges, dist[to]);
			}
	};

namespace impl
{
	template <class T, class Comp = std::less<T> >
	class IntsByVectorComparator
	{
		private:
			std::vector<T>& values;
			Comp comparator;
		public:
			IntsByVectorComparator(std::vector<T>& values, Comp comparator = Comp()): values(values), comparator(comparator) {}
		    bool operator() (unsigned int lhs, unsigned int rhs) {
				bool ls = comparator(values[lhs], values[rhs]);
				if (ls)
					return true;
				bool gt = comparator(values[rhs], values[lhs]);
				if (!ls && !gt) return lhs < rhs;
				return false;
			}
	};
}

	//Weight Func: path + weight -> path 
	template <class PathInfo, class PathRecountFunc, class PathInfoComparator = std::less<PathInfo>, class WeightType>
	ShortestPathsInfo<PathInfo, WeightType> find_shortest_paths(WeightedGraph<WeightType>& g, unsigned int from, 
			PathRecountFunc recount_func, PathInfoComparator comp = PathInfoComparator(), PathInfo initial_dist = PathInfo())
	{
		ShortestPathsInfo<PathInfo, WeightType> result(g.size(), initial_dist, from); 
		std::vector<char> proceeded(g.size(), 0);
		std::set<unsigned int, impl::IntsByVectorComparator<PathInfo,PathInfoComparator>> active 
			(impl::IntsByVectorComparator<PathInfo, PathInfoComparator>(result.dist,comp));
		active.insert(from);
		result.parents[from] = from;
		result.used[from] = 1;
		while (!active.empty())
		{
			unsigned int current_vertex = *active.begin();
			active.erase(active.begin());
			proceeded[current_vertex] = 1;
			for (typename WeightedGraph<WeightType>::iterator it = g[current_vertex].begin(); it != g[current_vertex].end(); it ++)
			{
				unsigned int updating_vertex = it->to;
				PathInfo new_path = recount_func(result.dist[current_vertex], it->info);
				if (!result.used[updating_vertex] || (!proceeded[updating_vertex] && comp(new_path, result.dist[updating_vertex])))
				{
					result.used[updating_vertex] = 1;
					active.erase(updating_vertex);
					result.dist[updating_vertex] = new_path;
					result.parents[updating_vertex] = current_vertex;
					result.edges_to_vertex[updating_vertex] = it.get_pointer();
					active.insert(updating_vertex);
				}
			}
		}
		return result;
	}

	template <class PathInfo, class PathRecountFunc, class PathInfoComparator = std::less<PathInfo>, class WeightType>
	Path<PathInfo, WeightType> find_shortest_path(WeightedGraph<WeightType>& g, unsigned int from, unsigned int to, 
			PathRecountFunc recount_func, PathInfoComparator comp = PathInfoComparator(), PathInfo initial_dist = PathInfo())
	{
		return find_shortest_paths(g, from, recount_func, comp, initial_dist).get_path(to);
	}

}
}

#endif //__SHORTEST_PATHS_H__
