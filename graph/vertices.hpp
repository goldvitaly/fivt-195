#ifndef VERTICES_H
#define VERTICES_H

#include <iostream>
#include <typeinfo>
#include "graph.hpp"
#include <vector>
#include <algorithm>
#include <iterator>
#include <set>

namespace graph
{
	template <class Container>
	class VertexWithSTLContainer: public Graph::Vertex
	{
		protected:
			Container data;
		public:
			class iterator: public Graph::Iterator
			{
				private:
					typename Container::const_iterator iterator_to_element;
				public:
					explicit iterator(typename Container::const_iterator it): iterator_to_element(it) {};
					void next() { iterator_to_element ++; };
//					void prev() { iterator_to_element --; };
					unsigned int get() const { return *iterator_to_element; };
					bool operator == (const Graph::Iterator& it) const
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
					Graph::Iterator* copy() const
					{
						return new iterator(iterator_to_element);
					}
			};
			size_t size() const { return data.size(); };
			Graph::iterator begin() const
			{
				return Graph::iterator(new iterator(data.begin()));
			}
			Graph::iterator end() const
			{
				return Graph::iterator(new iterator(data.end()));
			};
	};

	class VertexWithAbstractVector: public graph::VertexWithSTLContainer< std::vector<unsigned int> >
	{
	};

	class VertexWithUnsortedVector: public VertexWithAbstractVector 
	{
		public:
			void add(unsigned int vertex) 
			{
				data.push_back(vertex); 
			}; 
			bool has(unsigned int vertex) const
			{
				return std::find(data.begin(), data.end(), vertex) != data.end(); 
			}
			void del(unsigned int vertex)
			{
				std::vector <unsigned int>::iterator it = std::find(data.begin(), data.end(), vertex);
				if (it != data.end())
					data.erase(it);				
			}
	};

	class VertexWithSortedVector: public VertexWithAbstractVector 
	{		
		public:
			void add(unsigned int vertex) 
			{
				data.insert(std::upper_bound(data.begin(), data.end(), vertex), vertex);
			}; 
			bool has(unsigned int vertex) const
			{ 
				std::vector<unsigned int>::const_iterator it = std::lower_bound(data.begin(), data.end(), vertex);
				return it != data.end() && *it == vertex;
			}
			void del(unsigned int vertex) 
			{
				std::vector<unsigned int>::iterator it = std::lower_bound(data.begin(), data.end(), vertex);
				if (it != data.end() && *it == vertex)
					data.erase(it);				
			}
	};

	template <class Container> 
	class VertexWithAbstractSet: public graph::VertexWithSTLContainer<Container>
	{
		typedef graph::VertexWithSTLContainer<Container> Base;
		public:
			void add(unsigned int vertex)
			{
				Base::data.insert(vertex);
			};	
			bool has(unsigned int vertex) const 
			{
				return Base::data.find(vertex) != Base::data.end();
			};
			void del(unsigned int vertex)
			{
				typename Container::iterator it = Base::data.find(vertex);
				if (it != Base::data.end())
					Base::data.erase(it);
			};
	};

	class VertexWithSet: 	  public graph::VertexWithAbstractSet<std::set<unsigned int> > {};

	class VertexWithMultiset: public graph::VertexWithAbstractSet<std::multiset<unsigned int> > {}; 

	class DefaultVertexChooser
	{
		public:
			Graph::Vertex* operator() (unsigned int vertex_number, unsigned int number_of_vertices)
			{
				return new VertexWithUnsortedVector();
			}
	};
};


Graph::Graph(unsigned int vertex_count): vertices(vertex_count)
{
	for (unsigned int i = 0; i < vertices.size(); i ++)
		vertices[i].reset(new graph::VertexWithUnsortedVector());
};

#endif
