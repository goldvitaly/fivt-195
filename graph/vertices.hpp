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
					typename Container::iterator iterator_to_element;
				public:
					explicit iterator(typename Container::iterator it): iterator_to_element(it) {}
					void next() { iterator_to_element ++; };
					typename impl::Graph<EdgeInfo>::edge_type& get() 
					{
						return *iterator_to_element; 
					};
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

			class const_iterator: public graph::impl::Graph<EdgeInfo>::ConstIterator
			{
				private:
					typename Container::const_iterator iterator_to_element;
				public:
					explicit const_iterator(typename Container::const_iterator it): iterator_to_element(it){}
					void next() { iterator_to_element ++; };
					const typename impl::Graph<EdgeInfo>::edge_type& get() const 
					{
						return *iterator_to_element; 
					};
					bool operator == (const typename graph::impl::Graph<EdgeInfo>::ConstIterator& it) const
					{
						try
						{
							const const_iterator& real_it = dynamic_cast<const const_iterator&>(it);
							return iterator_to_element == real_it.iterator_to_element;
						}
						catch (const std::bad_cast& e)
						{
							return false;
						}
					}
					typename graph::impl::Graph<EdgeInfo>::ConstIterator* clone() const
					{
						return new const_iterator(iterator_to_element);
					}
			};


			size_t size() const { return data.size(); };
			typename graph::impl::Graph<EdgeInfo>::iterator begin()
			{
				return typename graph::impl::Graph<EdgeInfo>::iterator(new iterator(data.begin()));
			}
			typename graph::impl::Graph<EdgeInfo>::iterator end()
			{
				return typename graph::impl::Graph<EdgeInfo>::iterator(new iterator(data.end()));
			}
			typename graph::impl::Graph<EdgeInfo>::const_iterator begin() const 
			{
				return typename graph::impl::Graph<EdgeInfo>::const_iterator(new const_iterator(data.begin()));
			}
			typename graph::impl::Graph<EdgeInfo>::const_iterator end() const
			{
				return typename graph::impl::Graph<EdgeInfo>::const_iterator(new const_iterator(data.end()));
			}
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
			bool has(unsigned int vertex) const
			{
				return std::find_if(Base::data.begin(), Base::data.end(), graph::impl::EdgeIsEqualPred<edge_type>(vertex)) != Base::data.end(); 
			}
			const edge_type& get(unsigned int vertex) const
			{
				typename std::vector<edge_type>::const_iterator it = std::find_if(Base::data.begin(), Base::data.end(), 
						graph::impl::EdgeIsEqualPred<edge_type>(vertex));
				if (it == Base::data.end())
					throw std::out_of_range("No such edge");
				return *it;
			}
			edge_type& get(unsigned int vertex)
			{
				typename std::vector<edge_type>::iterator it = std::find_if(Base::data.begin(), Base::data.end(), 
						graph::impl::EdgeIsEqualPred<edge_type>(vertex));
				if (it == Base::data.end())
					throw std::out_of_range("No such edge");
				return *it;
			}
			bool del(unsigned int vertex)
			{
				int old_size = Base::data.size();
				typename std::vector<edge_type>::iterator it = std::find_if(Base::data.begin(), Base::data.end(), 
						graph::impl::EdgeIsEqualPred<edge_type>(vertex));
				if (it != Base::data.end() && it->to == vertex)
					Base::data.erase(it);
				return Base::data.size() != old_size;
			}
	};

	template <class EdgeInfo = graph::tag::no_info>
	class VertexWithSortedVector: public VertexWithAbstractVector<EdgeInfo>
	{		
		typedef graph::VertexWithAbstractVector<EdgeInfo> Base;
		typedef typename graph::impl::Graph<EdgeInfo>::edge_type edge_type;
		typedef graph::impl::EdgeComparator<edge_type> edge_comp;
		public:
			typename graph::impl::Graph<EdgeInfo>::Vertex* clone() const { 
				return new graph::VertexWithSortedVector<EdgeInfo>(*this); 
			};
			bool add(edge_type edge) 
			{
				int old_size = Base::data.size(); 
				Base::data.insert(std::upper_bound(Base::data.begin(), Base::data.end(), edge, edge_comp()), edge);
				return old_size != Base::data.size();
			}; 
			bool has(unsigned int vertex) const
			{
				typename std::vector<edge_type>::const_iterator it = std::lower_bound(Base::data.begin(), Base::data.end(), vertex, edge_comp());
				return it != Base::data.end() && it->to == vertex;
			}
			const edge_type& get(unsigned int vertex) const
			{
				typename std::vector<edge_type>::const_iterator it = std::lower_bound(Base::data.begin(), Base::data.end(), vertex, edge_comp());
				if (it == Base::data.end() || it->to != vertex)
					throw std::out_of_range("No such edge");
				return *it;
			}
			edge_type& get(unsigned int vertex) 
			{
				typename std::vector<edge_type>::iterator it = std::lower_bound(Base::data.begin(), Base::data.end(), vertex, edge_comp());
				if (it == Base::data.end() || it->to != vertex)
					throw std::out_of_range("No such edge");
				return *it;
			}
			bool del(unsigned int vertex) 
			{
				int old_size = Base::data.size();
				typename std::vector<edge_type>::iterator it = std::lower_bound(Base::data.begin(), Base::data.end(), vertex, edge_comp());
				if (it != Base::data.end() && it->to == vertex)
					Base::data.erase(it);		
				return Base::data.size() != old_size;
			}
	};

/*

	template <class Container, class EdgeInfo>
	class VertexWithAbstractSet: public graph::VertexWithSTLContainer<Container, EdgeInfo>
	{
		typedef graph::VertexWithSTLContainer<Container, EdgeInfo> Base;
		typedef typename graph::impl::Graph<EdgeInfo>::edge_type edge_type;
		typedef graph::impl::EdgeComparator<edge_type> edge_comp;
		public:
			class iterator: public graph::impl::Graph<EdgeInfo>::Iterator
			{	
				private: 
					typename Container::const_iterator iterator_to_element;
				public:
					explicit iterator
			}
			bool add(edge_type edge)
			{
				int old_size = Base::data.size();
				Base::data.insert(edge);
				return old_size != Base::data.size();
			};
			bool has(unsigned int vertex) const 
			{
				return Base::data.find(vertex) != Base::data.end();
			}
			bool del(unsigned int vertex)
			{
				int old_size = Base::data.size();
				typename Container::iterator it = Base::data.find(vertex);
				if (it != Base::data.end())
					Base::data.erase(it);
				return old_size != Base::data.size();
			}
			const edge_type& get(unsigned int vertex) const 
			{
				if (!has(vertex))
					throw std::logic_error("no such edge");
				return *Base::data.find(vertex);
			}
	};

	template <class EdgeInfo = graph::tag::no_info>
	class VertexWithSet: 	  public graph::VertexWithAbstractSet<
							  std::set<
									  	typename graph::impl::Graph<EdgeInfo>::edge_type,
										graph::impl::EdgeComparator<typename graph::impl::Graph<EdgeInfo>::edge_type>
									  >, EdgeInfo> 
	{
		typedef typename graph::impl::Graph<EdgeInfo>::edge_type edge_type;
		public:
			typename graph::impl::Graph<EdgeInfo>::Vertex* clone() const { return new graph::VertexWithSet<EdgeInfo>(*this); };
	};

	template <class EdgeInfo = graph::tag::no_info>
	class VertexWithMultiset: public graph::VertexWithAbstractSet<
							  std::multiset<
											typename graph::impl::Graph<EdgeInfo>::edge_type, 
											graph::impl::EdgeComparator<typename graph::impl::Graph<EdgeInfo>::edge_type>
							  >, EdgeInfo> 
	{
		typedef typename graph::impl::Graph<EdgeInfo>::edge_type edge_type;
		public:
			typename graph::impl::Graph<EdgeInfo>::Vertex* clone() const { return new graph::VertexWithMultiset<EdgeInfo>(*this); };
	}; 
*/

};



#endif
