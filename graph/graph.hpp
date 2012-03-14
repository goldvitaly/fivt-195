#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <stddef.h>
#include <utility>
#include <memory>
#include <iterator>

/*
	Class for graph storaging. 
	Vertexes are numbered with unsigned ints. 
*/

namespace graph
{
	class DefaultVertexChooser;
};

class Graph
{
	private:
		class IteratorWrapper;
	public:
		typedef IteratorWrapper iterator;		
		void add_edge(unsigned int from, unsigned int to) { this->get(from).add(to); };
		void del_edge(unsigned int from, unsigned int to) { this->get(from).del(to); };
		bool has_edge(unsigned int from, unsigned int to) { return this->get(from).has(to); };
		size_t size() const { return vertices.size(); };
		explicit Graph(unsigned int);
		template <class VertexChooser> Graph(unsigned int size, VertexChooser vertex_chooser): vertices(size, NULL)
		{
			for (size_t i = 0; i < vertices.size(); i ++)
				vertices[i] = vertex_chooser(-1, size);
		}
		template <class VertexChooser, class Iterator> 
			Graph(VertexChooser vertex_chooser, Iterator begin, Iterator end) // Expected degree of vertices
		{
			int size = 0;
			for (Iterator it = begin; it != end; it ++) size ++;
			vertices.resize(size);
			int i = 0;
			for (Iterator it = begin; it != end; it ++, i ++)
				vertices[i] = vertex_chooser(*it, size);
		};
		class Vertex
		{
			public:
				virtual iterator begin() const = 0;
				virtual iterator end() const = 0;
				virtual size_t size() const = 0;
				virtual void add(unsigned int) = 0;
				virtual void del(unsigned int) = 0;
				virtual bool has(unsigned int) const = 0;
				virtual ~Vertex() {};
		};
		const Vertex& operator [](unsigned int vertex) const { return *vertices[vertex]; };
		class Iterator
		{
			public:
				virtual void next() = 0;
//				virtual void prev() = 0;
				virtual unsigned int get() const = 0;
				virtual Iterator* copy() const = 0;
				virtual ~Iterator() {};
				virtual bool operator == (const Iterator& it) const = 0;
		};
		virtual ~Graph()
		{
			for (size_t i = 0; i < vertices.size(); i ++)
				delete vertices[i];
		};
	private:
		Vertex& get(unsigned int vertex) { return *vertices[vertex]; };
		std::vector < Vertex* > vertices;
		class IteratorWrapper
		{
			private:
				std::unique_ptr<Iterator> pointer_to_iterator;
			public:
				// typedefs for enabling iterator_traits for this iterator
				typedef std::forward_iterator_tag iterator_category;
				typedef unsigned int value_type;
				typedef ptrdiff_t difference_type;
				typedef unsigned int& reference;
				typedef unsigned int* pointer;
				
				unsigned int operator * () { return pointer_to_iterator->get(); }
				IteratorWrapper& operator ++ ()	{ pointer_to_iterator->next(); return *this; }
				IteratorWrapper& operator ++ (int) { pointer_to_iterator->next(); return *this; }
//				IteratorWrapper& operator -- ()
//				{
//					pointer_to_iterator->prev();
//				}			
				IteratorWrapper& operator = (const IteratorWrapper& it)
				{
					pointer_to_iterator.reset(it.pointer_to_iterator->copy());
					return *this;
				}
				bool operator == (const IteratorWrapper& it)
				{
					if (!pointer_to_iterator && !it.pointer_to_iterator)
						return true;
					if (!(pointer_to_iterator && it.pointer_to_iterator))
						return false;
					return *pointer_to_iterator == *it.pointer_to_iterator;
				}
				bool operator != (const IteratorWrapper& it) { return !(*this == it); }
				IteratorWrapper(const IteratorWrapper& it) { *this = it; }
				IteratorWrapper(Iterator* pointer): pointer_to_iterator(pointer) {};
				virtual ~IteratorWrapper() {};
		};
};



#include "vertices.hpp"

#endif //GRAPH_H
