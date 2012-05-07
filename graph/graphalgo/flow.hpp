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
	};

	struct FlowNetworkEdgeType
	{
		FlowWeight flow_capacity;
		FlowWeight residual_capacity;
		int edge_id;
	};

	template <class FlowWeight, class FindingPathAlgo>
	FlowWeight find_maximal_flow(const WeightedGraph<FlowWeight>& g, const FindingPathAlgo& algo)
	{
	}
}
}
}


#endif // __FLOW_H__
