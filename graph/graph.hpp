#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "graphimpl.hpp"
#include "vertices.hpp"

namespace graph
{
	class Graph: public impl::Graph<tag::no_info>
	{
		typedef impl::Graph<tag::no_info> Base;
		public:
			explicit Graph(unsigned int vertex_number): impl::Graph<tag::no_info>(vertex_number)
			{
				for (unsigned int i = 0; i < vertex_number; i ++)
					vertices[i].reset(new VertexWithUnsortedVector<>());
			}
			void add_edge(unsigned int, unsigned int, tag::no_info) = delete;
			void del_edge(unsigned int, unsigned int, tag::no_info) = delete;
//			bool has_edge(unsigned int, unsigned int, tag::no_info) const = delete;
			tag::no_info get_edge(unsigned int, unsigned int) = delete;
			void add_edge(unsigned int from, unsigned int to) { Base::add_edge(from, to, tag::no_info()); };
			void del_edge(unsigned int from, unsigned int to) { Base::del_edge(from, to, tag::no_info()); };
	};

	template <class WeightType>
	class WeightedGraph: public impl::Graph<WeightType> 
	{
		typedef impl::Graph<WeightType> Base;
		public:
			explicit WeightedGraph(unsigned int vertex_number): impl::Graph<WeightType>(vertex_number)
			{
				for (unsigned int i = 0; i < vertex_number; i ++)
					Base::vertices[i].reset(new VertexWithUnsortedVector<WeightType>());
			}
	};
}

#endif //GRAPH_HPP
