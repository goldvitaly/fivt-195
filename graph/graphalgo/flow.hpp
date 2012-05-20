#ifndef __FLOW_H__
#define __FLOW_H__

namespace graph
{
namespace algorithm
{
namespace impl
{
	template <class FlowWeight> 
	struct FlowInfo
	{
		std::vector <FlowWeight> current_flow;
		explicit FlowInfo(size_t len): current_flow(len, 0)
		{
		}
	};

	template <class FlowWeight>
	class FlowNetworkEdgeType
	{
		private:
			int edge_id;
			FlowInfo<FlowWeight>* flow_info;
		public:
			bool reversed;
			FlowNetworkEdgeType(FlowInfo<FlowWeight>& flow_info, const FlowWeight& capacity, int edge_id, bool reversed): 
				flow_info(&flow_info), capacity(capacity), edge_id(edge_id), reversed(reversed) {};
			FlowWeight capacity;
			void add_flow(const FlowWeight& flow)
			{
				if (reversed)
					flow_info->current_flow[edge_id] -= flow;
				else
					flow_info->current_flow[edge_id] += flow;
			}
			FlowWeight get_flow() const
			{
				if (reversed)
					return -flow_info->current_flow[edge_id];
				else
					return flow_info->current_flow[edge_id];
			}
			FlowWeight get_rest_capacity() const
			{
				return capacity - get_flow();
			}
	};

	template <class FlowWeight>
	class FlowRecountFunc
	{
		public:
			FlowWeight operator ()(const FlowWeight& path, const FlowNetworkEdgeType<FlowWeight>& edge)
			{
				return std::max(std::min(path, edge.get_rest_capacity()), FlowWeight(0));
			}
	};
	template <class FlowWeight>
	class FlowComp
	{
		bool operator()(const FlowWeight& lhs, const FlowWeight rhs)
		{
			return lhs < rhs;
		}
	};
}

	template <class FlowWeight>
	FlowWeight find_maximal_flow(const WeightedGraph<FlowWeight>& g, int from, int to)
	{
		typedef impl::FlowNetworkEdgeType<FlowWeight> flow_edge_type;
		WeightedGraph<flow_edge_type> flow_network(g.size());
		impl::FlowInfo<FlowWeight> flow_info(g.edges());
		int last_edge_id = 0;
		for (int i = 0; i < g.size(); i ++)
			for (auto it = g[i].begin(); it != g[i].end(); it ++)
			{
				flow_network.add_edge(i, it->to, flow_edge_type(flow_info, it->info, last_edge_id, 0));
				flow_network.add_edge(it->to, i, flow_edge_type(flow_info, 0, last_edge_id, 1));
				last_edge_id += 1;
			}
		FlowWeight result(0);
		Path<FlowWeight, flow_edge_type> path = find_shortest_path(flow_network, from, to, 
			impl::FlowRecountFunc<FlowWeight>(), std::greater<FlowWeight>(), std::numeric_limits<FlowWeight>::max());
		while (path && path.path_length.get() != FlowWeight(0))
		{
			FlowWeight cur_flow = path.path_length.get();
			result += cur_flow;
			for (int i = 0; i < path.edges.size(); i ++) 
				path.edges[i]->info.add_flow(cur_flow);
			path = find_shortest_path(flow_network, from, to, 
				impl::FlowRecountFunc<FlowWeight>(), std::greater<FlowWeight>(), std::numeric_limits<FlowWeight>::max());
		}
		return result;
	}
}
}


#endif // __FLOW_H__
