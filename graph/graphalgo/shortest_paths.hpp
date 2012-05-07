#ifndef __SHORTEST_PATHS_H__
#define __SHORTEST_PATHS_H__

#include <vector>

namespace graph
{
namespace algorithm
{

	template <class PathInfo> 
	class Path
	{
		public:
			std::vector<unsigned int> path; // vertices in path
			PathInfo path_length;
			Path(std::vector<unsigned int>path, PathInfo path_length): path(path), path_length(path_length)
			{
			}
			operator bool() { 
				return path.size() > 0; 
			} 
	};

namespace impl
{	
	template <class PathInfo> 
	class ShortestPathsInfo
	{
		public:
			unsigned int from;
			std::vector <PathInfo> dist;
			std::vector<char> used;
			std::vector <unsigned int> parents; // parent[from] == from
	};

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
				bool gt = comparator(values[rhs], values[lhs]);
				if (!ls && !gt) return lhs < rhs;
				return ls;
			}
	};

	//Weight Func: path + weight -> path 
	template <class PathInfo, class PathRecountFunc, class PathInfoComparator = std::less<PathInfo>, class WeightType>
	ShortestPathsInfo<PathInfo> find_shortest_paths(const WeightedGraph<WeightType>& g, unsigned int from, 
			PathRecountFunc recount_func, PathInfoComparator comp = PathInfoComparator(), PathInfo initial_dist = PathInfo())
	{
		ShortestPathsInfo<PathInfo> result; 
		result.used.assign(g.size(), 0);
		std::vector<char> proceeded(g.size(), 0);
		result.parents.assign(g.size(), 0);
		result.dist.assign(g.size(), initial_dist);
		result.from = from;
		std::set<unsigned int, IntsByVectorComparator<PathInfo,PathInfoComparator>> active 
			(IntsByVectorComparator<PathInfo, PathInfoComparator>(result.dist,comp));
		active.insert(from);
		result.parents[from] = from;
		result.used[from] = 1;
		while (!active.empty())
		{
			unsigned int current_vertex = *active.begin();
			active.erase(active.begin());
			proceeded[current_vertex] = 1;
			for (auto it = g[current_vertex].begin(); it != g[current_vertex].end(); it ++)
			{
				unsigned int updating_vertex = it->to;
				PathInfo new_path = recount_func(result.dist[current_vertex], it->info);
				if (!result.used[updating_vertex] || (!proceeded[updating_vertex] && comp(new_path, result.dist[updating_vertex])))
				{
					result.used[updating_vertex] = 1;
					active.erase(updating_vertex);
					result.dist[updating_vertex] = new_path;
					result.parents[updating_vertex] = current_vertex;
					active.insert(updating_vertex);
				}
			}
		}
		return result;
	}
}

	template <class PathInfo, class PathRecountFunc, class PathInfoComparator = std::less<PathInfo>, class WeightType>
	Path<PathInfo> find_shortest_path(const WeightedGraph<WeightType>& g, unsigned int from, unsigned int to, 
			PathRecountFunc recount_func, PathInfoComparator comp = PathInfoComparator(), PathInfo initial_dist = PathInfo())
	{
		impl::ShortestPathsInfo<PathInfo> shortest_paths = impl::find_shortest_paths(g, from, recount_func, comp, initial_dist);
		if (!shortest_paths.used[to])
			return Path<PathInfo>(std::vector<unsigned int>(), shortest_paths.dist[to]);
		std::vector<unsigned int> resulted_path;
		int v = to;
		while (shortest_paths.parents[v] != v)
		{
			resulted_path.push_back(v);
			v = shortest_paths.parents[v];
		}
		resulted_path.push_back(v);
		std::reverse(resulted_path.begin(), resulted_path.end());
		return Path<PathInfo>(resulted_path, shortest_paths.dist[to]);
	}

}
}

#endif //__SHORTEST_PATHS_H__
