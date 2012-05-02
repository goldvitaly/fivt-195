#ifndef VECTOR_INCIDENCE
#define VECTOR_INCIDENCE

#include <algorithm>

#include "../Graph/IIncidence.hpp"
#include "../Graph/Iterator.hpp"
#include "../Graph/IteratorWrapper.hpp"

class VectorIncidence: public graph::IIncidence
{
public: // built-in iterator definition
	class VectorIterator: public graph::Iterator
	{
	public:
		VectorIterator() {}
		explicit VectorIterator(std::vector<size_t>::const_iterator it_): it(it_) {}
		virtual ~VectorIterator() {}
		
		virtual std::unique_ptr<Iterator> clone() const {
			return std::unique_ptr<Iterator>(new VectorIterator(it));
		}
		
		virtual Iterator& operator++ () {
			++it;
			return *this;
		}
		virtual size_t operator* () const {
			return *it;
		}
		virtual bool operator!= (const graph::Iterator& iterator) const {
			return it != dynamic_cast<const VectorIterator&>(iterator).it; // todo: catch bad_cast
		}
		
	private:
		std::vector<size_t>::const_iterator it;
	};
	
public: // definition of methods
	VectorIncidence() {}
	virtual ~VectorIncidence() {} // TODO: add copy constructors

	virtual std::unique_ptr<graph::IIncidence> clone() const {
		return std::unique_ptr<graph::IIncidence>(new VectorIncidence(*this));
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

	virtual graph::IteratorWrapper begin() const {
		return graph::IteratorWrapper(new VectorIterator(adjacent.begin()));
	}
	virtual graph::IteratorWrapper end() const {
		return graph::IteratorWrapper(new VectorIterator(adjacent.end()));
	}

private:
	std::vector<size_t> adjacent;
	
};

#endif
