#pragma once

#include "graph.hpp"

#include <vector>
#include <memory>

namespace graph
{
	template<typename WeightType>
	class WeightedIterator
	{
		public:
			typedef typename std::vector<WeightType>::const_iterator w_iter;

			WeightedIterator(std::unique_ptr<NodeIterator>& nodeIter, w_iter weightIter) : weightIterator(weightIter)
			{
				nodeIterator = std::move(nodeIter);
			}

			WeightedIterator& operator++()
			{
				++*nodeIterator;
				++weightIterator;
				return *this;
			}

			WeightedIterator* operator--()
			{
				--*nodeIterator;
				--weightIterator;
				return *this;
			}

			std::pair<unsigned, WeightType> operator*() const
			{
				return make_pair(**nodeIterator, *weightIterator);
			}

			bool operator==(const WeightedIterator& wi) const
			{
				return *nodeIterator == *wi.nodeIterator;
			}

			bool operator!=(const WeightedIterator& wi) const
			{
				return *nodeIterator != *wi.nodeIterator;
			}			
		private:
			std::unique_ptr<NodeIterator> nodeIterator;
			w_iter weightIterator;

	};

	template<typename WeightType>
	class WeightedNode
	{
		public:
			WeightedNode(const Node& nd, const std::vector<WeightType>& wg) : node(nd), weights(wg) {}

			WeightedIterator<WeightType> begin()
			{
				return WeightedIterator<WeightType>(node.begin().getBaseIterator(), weights.begin());
			}

			WeightedIterator<WeightType> end()
			{
				return WeightedIterator<WeightType>(node.end().getBaseIterator(), weights.end());
			}

		private:
			const Node& node;
			const std::vector<WeightType>& weights;
	};

	template<typename WeightType>
	class WeightedGraph
	{
		public:
			WeightedGraph() : graph(), weights() {}

			unsigned add(std::unique_ptr<Node> node)
			{
				weights.push_back(std::vector<WeightType>());
				return graph.add(std::move(node));
			}
			
			unsigned add(std::unique_ptr<Node> node, const std::vector<unsigned>& friends, const std::vector<WeightType>& edgesWeights)
			{
				weights.push_back(edgesWeights);
				return graph.add(std::move(node), friends);
			}
			
			void connect(unsigned v1, unsigned v2, WeightType weight)
			{
				weights[v1].push_back(weight);
				return graph.connect(v1, v2);
			}
			
			bool areConnected(unsigned v1, unsigned v2) const
			{
				return graph.areConnected(v1, v2);
			}		
			
			WeightedNode<WeightType> getNode(unsigned v) const
			{
				return WeightedNode<WeightType>(graph.getNode(v), weights[v]);
			}
			
			size_t size() const
			{
				return graph.size();
			}

			const Graph& getBaseGraph()
			{
				return graph;
			}

		private:
			Graph graph;
			std::vector<std::vector<WeightType>> weights;
	};
}
