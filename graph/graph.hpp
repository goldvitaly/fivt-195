#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <stddef.h>
#include <utility>
#include <memory>

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
		size_t size_;
	public:
		typedef IteratorWrapper iterator;		
		void add_edge(unsigned int from, unsigned int to)
		{
			(*this)[from].add(to);
		};
		void del_edge(unsigned int from, unsigned int to) 
		{
			(*this)[from].del(to);
		};
		size_t size() const { return size_; };
		explicit Graph(unsigned int);
		template <class VertexChooser, class Iterator>
		explicit Graph(VertexChooser vc, Iterator begin, Iterator end) // Expected degree of vertices
		{
			size_ = 0;
			for (Iterator it = begin; it != end; it ++) size_ ++;
			vertices.resize(size_);
			int i = 0;
			for (Iterator it = begin; it != end; it ++, i ++)
				vertices[i] = vc(*it, size_);
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
		Vertex& operator [](unsigned int vertex) const
		{
			return *vertices[vertex];			
		};
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
			for (int i = 0; i < vertices.size(); i ++)
				delete vertices[i];
		};
	private:
		std::vector < Vertex* > vertices;
		class IteratorWrapper
		{
			private:
				std::unique_ptr<Iterator> pointer_to_iterator;
			public:
				unsigned int operator * ()
				{
					return pointer_to_iterator->get();
				}
				IteratorWrapper& operator ++ ()
				{
					pointer_to_iterator->next();
				}
				IteratorWrapper& operator ++ (int)
				{
					pointer_to_iterator->next();
				};
//				IteratorWrapper& operator -- ()
//				{
//					pointer_to_iterator->prev();
//				}			
				IteratorWrapper& operator = (const IteratorWrapper& it)
				{
					pointer_to_iterator.reset(it.pointer_to_iterator->copy());
				}
				bool operator == (const IteratorWrapper& it)
				{
					if (!pointer_to_iterator && !it.pointer_to_iterator)
						return true;
					if (!(pointer_to_iterator && it.pointer_to_iterator))
						return false;
					return *pointer_to_iterator == *it.pointer_to_iterator;
				}
				bool operator != (const IteratorWrapper& it)
				{
					return !(*this == it);
				}
				IteratorWrapper(const IteratorWrapper& it)
				{
					*this = it;
				}
				IteratorWrapper(Iterator* pointer): pointer_to_iterator(pointer)
				{

				};
				virtual ~IteratorWrapper()
				{
				};
		};
};

#include "vertices.hpp"

#endif //GRAPH_H
