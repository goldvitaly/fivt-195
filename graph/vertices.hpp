#ifndef VERTICES_H
#define VERTICES_H

#include <iostream>
#include <typeinfo>
#include "graphimpl.hpp"
#include <vector>
#include <algorithm>
#include <iterator>
#include <set>
#include <typeinfo>
#include <stdexcept>

namespace graph
{
	template <class Container, class EdgeInfo>
	class VertexWithSTLContainer: public graph::impl::Graph<EdgeInfo>::Vertex
	{
		protected:
			Container data;
		public:
			class iterator: public graph::impl::Graph<EdgeInfo>::Iterator
			{
				private:
					typename Container::const_iterator iterator_to_element;
				public:
					explicit iterator(typename Container::const_iterator it): iterator_to_element(it) {};
					void next() { iterator_to_element ++; };
//					void prev() { iterator_to_element --; };
					const typename impl::Graph<EdgeInfo>::edge_type& get() const { return *iterator_to_element; };
					bool operator == (const typename graph::impl::Graph<EdgeInfo>::Iterator& it) const
					{
						try
						{
							const iterator& real_it = dynamic_cast<const iterator&>(it);
							return iterator_to_element == real_it.iterator_to_element;
						}
						catch (const std::bad_cast& e)
						{
							return false;
						}
					}
					typename graph::impl::Graph<EdgeInfo>::Iterator* clone() const
					{
						return new iterator(iterator_to_element);
					}
			};
			size_t size() const { return data.size(); };
			typename graph::impl::Graph<EdgeInfo>::iterator begin() const 
			{
				return typename graph::impl::Graph<EdgeInfo>::iterator(new iterator(data.begin()));
			}
			typename graph::impl::Graph<EdgeInfo>::iterator end() const
			{
				return typename graph::impl::Graph<EdgeInfo>::iterator(new iterator(data.end()));
			};
	};

	template <class EdgeInfo>
	class VertexWithAbstractVector: public graph::VertexWithSTLContainer< 
										std::vector<typename graph::impl::Graph<EdgeInfo>::edge_type>, 
										EdgeInfo>
	{
	};

	template <class EdgeInfo = graph::tag::no_info>
	class VertexWithUnsortedVector: public VertexWithAbstractVector<EdgeInfo>
	{
		typedef graph::VertexWithAbstractVector<EdgeInfo> Base;
		typedef typename graph::impl::Graph<EdgeInfo>::edge_type edge_type;
		public:
			typename graph::impl::Graph<EdgeInfo>::Vertex* clone() const { 
				return new graph::VertexWithUnsortedVector<EdgeInfo>(*this); 
			}
			bool add(edge_type edge) 
			{
				int old_size = Base::data.size();
				Base::data.push_back(edge); 
				return Base::data.size() != old_size;
			}
			bool has(edge_type edge) const
			{
				return std::find(Base::data.begin(), Base::data.end(), edge) != Base::data.end(); 
			}
			const edge_type& get(unsigned int vertex) const
			{
				typename std::vector<edge_type>::const_iterator it = std::find(Base::data.begin(), Base::data.end(), edge_type(vertex));
				if (it == Base::data.end())
					throw std::out_of_range("No such edge");
				return *it;
			}
			bool del(edge_type edge)
			{
				int old_size = Base::data.size();
				typename std::vector<edge_type>::iterator it = std::find(Base::data.begin(), Base::data.end(), edge);
				return Base::data.size() != old_size;
			}
	};

	template <class EdgeInfo = graph::tag::no_info>
	class VertexWithSortedVector: public VertexWithAbstractVector<EdgeInfo>
	{		
		typedef graph::VertexWithAbstractVector<EdgeInfo> Base;
		typedef typename graph::impl::Graph<EdgeInfo>::edge_type edge_type;
		public:
			typename graph::impl::Graph<EdgeInfo>::Vertex* clone() const { 
				return new graph::VertexWithSortedVector<EdgeInfo>(*this); 
			};
			bool add(edge_type edge) 
			{
				int old_size = Base::data.size(); 
				Base::data.insert(std::upper_bound(Base::data.begin(), Base::data.end(), edge), edge);
				return old_size != Base::data.size();
			}; 
			bool has(edge_type edge) const
			{ 
				typename std::vector<edge_type>::const_iterator it = std::lower_bound(Base::data.begin(), Base::data.end(), edge);
				return it != Base::data.end() && *it == edge;
			}
			const edge_type& get(unsigned int vertex) const
			{
				typename std::vector<edge_type>::const_iterator it =
					std::lower_bound(Base::data.begin(), Base::data.end(), edge_type(vertex));
				if (it == Base::data.end() || it->to != vertex)
					throw std::out_of_range("No such edge");
				return *it;
			}
			bool del(edge_type edge) 
			{
				int old_size = Base::data.size();
				typename std::vector<edge_type>::iterator it = std::lower_bound(Base::data.begin(), Base::data.end(), edge);
				if (it != Base::data.end() && *it == edge)
					Base::data.erase(it);		
				return Base::data.size() != old_size;
			}
	};

	template <class Container, class EdgeInfo>
	class VertexWithAbstractSet: public graph::VertexWithSTLContainer<Container, EdgeInfo>
	{
		typedef graph::VertexWithSTLContainer<Container, EdgeInfo> Base;
		typedef typename graph::impl::Graph<EdgeInfo>::edge_type edge_type;
		public:
			bool add(edge_type edge)
			{
				int old_size = Base::data.size();
				Base::data.insert(edge);
				return old_size != Base::data.size();
			};
			bool has(edge_type edge) const 
			{
				return Base::data.find(edge) != Base::data.end();
			}
			bool del(edge_type edge)
			{
				int old_size = Base::data.size();
				typename Container::iterator it = Base::data.find(edge);
				if (it != Base::data.end())
					Base::data.erase(it);
				return old_size != Base::data.size();
			}
			const edge_type& get(unsigned int vertex) const 
			{
				if (!has(edge_type(vertex)))
					throw std::logic_error("no such edge");
				return *Base::data.find(edge_type(vertex));
			}
	};

	template <class EdgeInfo = graph::tag::no_info>
	class VertexWithSet: 	  public graph::VertexWithAbstractSet<std::set<typename graph::impl::Graph<EdgeInfo>::edge_type>, EdgeInfo> 
	{
		typedef typename graph::impl::Graph<EdgeInfo>::edge_type edge_type;
		public:
			typename graph::impl::Graph<EdgeInfo>::Vertex* clone() const { return new graph::VertexWithSet<EdgeInfo>(*this); };
	};

	template <class EdgeInfo = graph::tag::no_info>
	class VertexWithMultiset: public graph::VertexWithAbstractSet<std::multiset<typename graph::impl::Graph<EdgeInfo>::edge_type>, EdgeInfo> 
	{
		typedef typename graph::impl::Graph<EdgeInfo>::edge_type edge_type;
		public:
			typename graph::impl::Graph<EdgeInfo>::Vertex* clone() const { return new graph::VertexWithMultiset<EdgeInfo>(*this); };
	}; 

};



#endif
