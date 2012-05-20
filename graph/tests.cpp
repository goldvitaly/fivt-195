#include "graph.hpp"
#include "useful.hpp"
#include "graphalgo.hpp"

#include <vector>
#include <set>
#include <map>
#include <iostream>
#include <cstdlib>
#include <ctime>

class Timer
{
	private:
		unsigned int start_time;
	public:
		Timer() { start_time = clock(); };
		void start() { start_time = clock(); };
		double get_time() { return (clock() - start_time) / double(CLOCKS_PER_SEC); };
};

template <class VertexRealization>
int test_graph_realization(unsigned int vertex_number, unsigned int requests)
{	
	Timer t;
	graph::Graph testing_realization(vertex_number);
	for (unsigned int i = 0; i < vertex_number; i ++)
		testing_realization.reset(i, new VertexRealization());
	std::vector < std::set <unsigned int> > model_realization(vertex_number);
	for (unsigned int i = 0; i < requests; i ++)
	{
		int v = rand() % vertex_number;
		int u = rand() % vertex_number;
		char has_in_model = model_realization[v].find(u) != model_realization[v].end();
		char has_in_testing = testing_realization.has_edge(v,u);
		if (has_in_model != has_in_testing)
		{
			std::cerr << "Test failed on step " << i;
			if (has_in_model) 
				std::cerr << ": existing edge (" << v << ", " << u << ") not found" << std::endl;
			else
				std::cerr << " non-existing edge (" << v << ", " << u << ") found" << std::endl;
			return -1;
		}
		if (has_in_model)
		{
			model_realization[v].erase(model_realization[v].find(u));
			testing_realization.del_edge(v,u);
		}			
		else
		{
			model_realization[v].insert(u);
			testing_realization.add_edge(v,u);
		}
		std::set <unsigned int> adjacency_list; 
		for (auto neighbor: testing_realization[v])
			adjacency_list.insert(neighbor.to);
		if (adjacency_list != model_realization[v])
		{

			std::cerr << "Test failed on step " << i << std::endl << "Expected adjacency list: " << model_realization[v] << ", got: " << adjacency_list << std::endl; 
			for (auto it = testing_realization[v].begin(); it != testing_realization[v].end(); it ++)
				std::cerr << v << " ";
			std::cerr << std::endl;
			return -1;
		}
	}
	std::cerr << "Test OK" << " within " << t.get_time() << " seconds" << std::endl;
	return 0;
};

int strongly_connection_test(int vertex_number, int requests)
{
	Timer t;
	class NaiveSolver
	{
		public:
			static void dfs(unsigned int current_vertex, std::vector<char>& used, const graph::Graph& g)
			{
				if (used[current_vertex]) return;
				used[current_vertex] = 1;
				for (auto next_edge: g[current_vertex])
					if (!used[next_edge.to])
						dfs(next_edge.to, used, g);
			}
			static std::vector<unsigned int> solve(const graph::Graph& g)
			{
				int vertex_number = g.size();
				std::vector<unsigned int> coloring(vertex_number, 0);
				std::vector<std::vector<char>> connectivity_table(vertex_number, std::vector<char>(vertex_number, false));
				for (int i = 0; i < vertex_number; i ++)
					dfs(i, connectivity_table[i], g);
				std::vector<char> used(vertex_number, 0);
				int colors = 0;
				for (int i = 0; i < vertex_number; i ++)
					if (!used[i])
					{
						for (int j = 0; j < vertex_number; j ++)
							if (connectivity_table[i][j] && connectivity_table[j][i])
							{
								used[j] = 1;
								coloring[j] = colors;
							}
						colors ++;
					}
				return coloring;
			};
	};
	class SmartVectorComparator
	{
		private:
			std::vector <unsigned int> renumerate(const std::vector<unsigned int>& coloring)
			{
				std::vector<unsigned int> result;
				result.reserve(coloring.size());
				int colors = 0;
				std::map<unsigned int, unsigned int> compressor;
				for (unsigned int c: coloring)
				{
					if (compressor.count(c) > 0)
					{
						result.push_back(colors);
						compressor[c] = colors ++;
					}
					else
						result.push_back(compressor[c]);
				}
				return result;
			};
		public:
			bool operator () (const std::vector<unsigned int>& a, const std::vector<unsigned int>& b)
			{
				return renumerate(a) == renumerate(b);
			};
	};
	graph::Graph g(vertex_number);
	for (int step = 0; step < requests; step ++)
	{
		int u = rand() % vertex_number;
		int v = rand() % vertex_number;
		if (!g.has_edge(v,u))
			g.add_edge(v, u);
		else
			g.del_edge(v, u);
		std::vector<unsigned int> model_coloring = NaiveSolver::solve(g);
		std::vector<unsigned int> testing_coloring = graph::algorithm::get_strongly_connected_components_coloring(g);
		if (!SmartVectorComparator()(model_coloring, testing_coloring))
		{
			for (int i = 0; i < vertex_number; i ++)
			{
				std::vector<unsigned int> vertices;
				for (auto it: g[i])
					vertices.push_back(it.to);
				std::cerr << vertices << std::endl;
			}
			std::cerr << "Test failed on step " << step << std::endl << "Expected: " << model_coloring << std::endl << "Got: " << testing_coloring << std::endl;
			return -1;
		}
	}
	std::cerr.precision(2);
	std::cerr << "Test OK" << " within " << t.get_time() << " seconds" << std::endl;
	return 0;
}

template <class Weight, class WeightGen>
graph::WeightedGraph<Weight> generate_graph(int vertex_number, int edge_number, WeightGen gen)
{
	graph::WeightedGraph<Weight> result(vertex_number);
	std::vector < std::pair<unsigned int, unsigned int> > edges;
	for (int i = 0; i < vertex_number; i ++)
		for (int j = 0; j < vertex_number; j ++)
			edges.push_back(std::make_pair(i, j));
	random_shuffle(edges.begin(), edges.end());
	edge_number = std::min(edge_number, (int)edges.size());
	for (int i = 0; i < edge_number; i ++)
	{
		Weight edge_weight = gen();
		result.add_edge(edges[i].first, edges[i].second, edge_weight);
	}
	return result;
}

template <class Weight, class PathInfo, class PathInfoComp, class PathRecountFunc>
std::pair<bool, PathInfo> naive_find_shortest_path(
		const graph::WeightedGraph<Weight>& g,
		unsigned int from,
		unsigned int to,
		PathRecountFunc recount_func = PathRecountFunc(), 
		PathInfoComp comp = PathInfoComp(), 
		PathInfo initial_dist = PathInfo() 
		)
{
	std::vector<bool> used(g.size(), 0);
	std::vector<PathInfo> cur_dist(g.size(), initial_dist);
	used[from] = 1;
	for (int i = 0; i < g.size(); i ++)
		for (unsigned int vertex_from = 0; vertex_from < g.size(); vertex_from ++)
			if (used[vertex_from])
				for (auto it = g[vertex_from].begin(); it != g[vertex_from].end(); it ++)
				{
					int vertex_to = it->to;
					if (used[vertex_to])
						cur_dist[vertex_to] = std::min(cur_dist[vertex_to], recount_func(cur_dist[vertex_from], it->info), comp);
					else
					{
						cur_dist[vertex_to] = recount_func(cur_dist[vertex_from], it->info);
						used[vertex_to] = 1;
					}
				}
	return make_pair(used[to], cur_dist[to]);
}


template <class Weight, class PathInfo, class WeightGen, class PathInfoComp, class PathRecountFunc>
int test_shortest_path_finding(
		int vertex_number,
		int edge_number,
		WeightGen gen,
		PathRecountFunc recount_func = PathRecountFunc(), 
		PathInfoComp comp = PathInfoComp(), 
		PathInfo initial_dist = PathInfo() 
		)
{
	graph::WeightedGraph<Weight> g = generate_graph<Weight>(vertex_number, edge_number, gen);
	unsigned int from = rand() % vertex_number;
	unsigned int to = rand() % vertex_number;
	graph::algorithm::Path<PathInfo, Weight> shortest_path = graph::algorithm::find_shortest_path(g, from, to, recount_func, comp, initial_dist);
	bool path_found = shortest_path.path.size() > 0;
	if (path_found)
	{
		bool valid_path = shortest_path.path[0] == from && shortest_path.path.back() == to;
		for (unsigned int i = 1; i < shortest_path.path.size(); i ++)
			valid_path = valid_path && g.has_edge(shortest_path.path[i-1], shortest_path.path[i]);
		if (!valid_path)
		{
			std::cerr << "Test failed. Path is incorrect" << std::endl;
			return 1;
		}
		PathInfo path_len = initial_dist;
		for (unsigned int i = 1; i < shortest_path.path.size(); i ++)
			path_len = recount_func(path_len, g.get_edge(shortest_path.path[i-1], shortest_path.path[i]));
		if (path_len != shortest_path.path_length.get())
		{
			std::cerr << "Test failed. Provided length doesn't correspond to real length" << std::endl;
			return 1;
		}
	}
	std::pair<bool, PathInfo> correct_shortest_path = naive_find_shortest_path(g, from, to, recount_func, comp, initial_dist);
	if (correct_shortest_path.first != path_found)
	{
		std::cerr << "Test failed: path wasn't found when it exists" << std::endl;
		return 1;
	}
	if (path_found && comp(correct_shortest_path.second, shortest_path.path_length.get()))
	{
		std::cerr << "Test failed: shorter path exists" << std::endl;
		return 1;
	}
	if (path_found && comp(shortest_path.path_length.get(), correct_shortest_path.second))
	{
		std::cerr << "Test failed: naive algorithm failed" << std::endl;
		return 1;
	}
	return 0;
}

class Int
{
	public:
		int value;
		explicit Int(int value): value(value)
		{
		}
};

bool operator < (const Int& lhs, const Int& rhs)
{
	return lhs.value < rhs.value;
}

Int operator + (const Int& lhs, const Int& rhs)
{
	return Int(lhs.value + rhs.value);
}

bool operator != (const Int& lhs, const Int& rhs)
{
	return lhs.value != rhs.value;
}

std::vector<unsigned int> generate_random_path(int n, int k, int from, int to)
{
	assert(k >= 2);
	std::set<unsigned int> cur;
	cur.insert(from);
	cur.insert(to);
	std::vector<unsigned int> ans;
	ans.reserve(k);
	ans.push_back(from);
	for (int i = 0; i < k - 2; i ++)
	{
		int u = from;
		while (cur.count(u) != 0)
			u = rand() % n;
		ans.push_back(u);
		cur.insert(u);
	}
	ans.push_back(to);
	return ans;
}

int test_int_flow(int n, int paths)
{
	assert(n >= 2);
	int from = rand() % n;
	int to = from;
	while (from == to)
		to = rand() % n;
	graph::WeightedGraph<int> g(n);
	std::vector < std::vector <int> > t(n, std::vector<int>(n,0));
	int real_flow = 0;
	for (int j = 0; j < paths; j ++)
	{
		int vertexes_in_path = 2 + rand() % (n - 1);
		int flow = rand() % 100;
		real_flow += flow;
		std::vector<unsigned int> path = generate_random_path(n, vertexes_in_path, from, to);
		for (int i = 1; i < path.size(); i++)
			t[path[i-1]][path[i]] += flow;
	}
	for (int i = 0; i < n; i ++)
		for (int j = 0; j < n; j++)
			if (t[i][j] > 0) 
				g.add_edge(i, j, t[i][j]);
	int found_flow = graph::algorithm::find_maximal_flow(g, from, to);
	if (found_flow != real_flow)
	{
		std::cerr << "Testing flow finding failed" << std::endl << "Expected: " << real_flow << ", got: " << found_flow << std::endl;
		return 1;
	}
	return 0;
}

int main()
{
	std::cerr << "Testing graph realization" << std::endl;
	if (test_graph_realization<graph::VertexWithUnsortedVector<>>(3,1e5) != 0) return 1;
	if (test_graph_realization<graph::VertexWithUnsortedVector<>>(100,1e5) != 0) return 1;
	if (test_graph_realization<graph::VertexWithSortedVector<>>(100,1e5) != 0) return 1;
	std::cerr << "Graph realization seemes to be OK" << std::endl << "Testing Tarjan algorithm realization" << std::endl;
	if (strongly_connection_test(5,   1e3) != 0) return 1;
	if (strongly_connection_test(10,  1e3) != 0) return 1;
	if (strongly_connection_test(50,  1e3) != 0) return 1;
	if (strongly_connection_test(100, 1e3) != 0) return 1;
	std::cerr << "Tarjan algorithm seemes to be OK" << std::endl; 
	for (int i = 0; i < 100; i ++)
		if (test_shortest_path_finding<int, int>(5, 15, [](){ return rand() % 100; }, std::plus<int>(), std::less<int>(), 0) != 0) 
			return 1;
	for (int i = 0; i < 100; i ++)
		if (test_shortest_path_finding<Int, Int>(5, 15, [](){ return Int(rand() % 100); }, std::plus<Int>(), std::less<Int>(), Int(0)) != 0) 
			return 1;
	std::cerr << "Dijkstra with ints seemes to be OK" << std::endl;
	for (int i = 0; i < 100; i ++)
		if (test_shortest_path_finding<double, double>(5, 15, drand48, 
					std::plus<double>(), 
					[](double a, double b){ return a < b - 1e-8; }, 
					double(0)) != 0) 
			return 1;
	std::cerr << "Dijkstra with doubles seemes to be OK" << std::endl; 
	for (int i = 1; i <= 10; i ++) if (test_int_flow(3, i) != 0)	return 1;
	for (int i = 0; i < 10; i ++) if (test_int_flow(3, 100) != 0)	return 1;
	for (int i = 0; i < 10; i ++) if (test_int_flow(5, 100) != 0)	return 1;
	for (int i = 0; i < 10; i ++) if (test_int_flow(100, 1000) != 0)	return 1;
	std::cerr << "Flow finding seemes to be OK" << std::endl;
	return 0;
}
