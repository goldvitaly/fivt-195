#include <algorithm>

#include "Graph.hpp"
#include "iIncidence.hpp"
#include "Iterator.hpp"
#include "IteratorWrapper.hpp"

class VectorIncidence: public Graph::iIncidence
{
public: // built-in iterator definition
	class VectorIterator: public Graph::iIncidence::Iterator
	{
	public:
		VectorIterator() {}
		VectorIterator(std::vector<size_t>::const_iterator it_): it(it_) {}
		~VectorIterator() {}
		
		virtual std::unique_ptr<Iterator> clone() {
			return std::unique_ptr<Iterator>(new VectorIterator(it));
		}
		
		virtual Iterator& operator++ () {
			++it;
			return *this;
		}
		virtual size_t operator* () const {
			return *it;
		}
		virtual bool operator!= (const Iterator& iterator) const {
			return it != dynamic_cast<const VectorIterator&>(iterator).it; // todo: catch bad_cast
		}
		
	private:
		std::vector<size_t>::const_iterator it;
	};
	
public: // definition of methods
	VectorIncidence() {}
	~VectorIncidence() {} // TODO: add copy constructors

	virtual std::unique_ptr<iIncidence> clone() const {
		return std::unique_ptr<iIncidence>(new VectorIncidence(*this));
	}

	virtual void addEdge(size_t to) {
		adjacent.push_back(to);
	}
	virtual void delEdge(size_t to) {
		adjacent.erase(std::find(adjacent.begin(), adjacent.end(), to));
	}
	
	virtual size_t size() const {
		return adjacent.size();
	}

	virtual IteratorWrapper begin() const {
		return IteratorWrapper(new VectorIterator(adjacent.begin()));
	}
	virtual IteratorWrapper end() const {
		return IteratorWrapper(new VectorIterator(adjacent.end()));
	}

private:
	std::vector<size_t> adjacent;
	
};
