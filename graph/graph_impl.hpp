#ifndef GRAPH_IMPLEMENTATION_H
#define GRAPH_IMPLEMENTATION_H

#include <vector>
#include <stddef.h>
#include <utility>
#include <memory>
#include <iterator>

/*
	Class for graph storing. 
	Vertexes are numbered using unsigned ints. 
*/

namespace graph
{
namespace impl
{

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
		template <class VertexChooser> Graph(unsigned int size, VertexChooser vertex_chooser = VertexChooser()): vertices(size)
		{
			for (size_t i = 0; i < vertices.size(); i ++)
				vertices[i].reset(vertex_chooser(i, size));
		}
		explicit Graph(const Graph& G) = delete;
		Graph& operator = (const Graph&) = delete;
		virtual ~Graph() {}; 
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
				virtual Iterator* clone() const = 0;
				virtual ~Iterator() {};
				virtual bool operator == (const Iterator& it) const = 0;
		};
	protected:
		Vertex& get(unsigned int vertex) { return *vertices[vertex]; };
		std::vector < std::unique_ptr <Vertex> > vertices;
	private:
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
					pointer_to_iterator.reset(it.pointer_to_iterator->clone());
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
				~IteratorWrapper() {};
		};
};


}; // namespace impl
}; // namespace graph


#endif //GRAPH_H
