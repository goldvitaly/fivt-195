#ifndef VECTOR_INCIDENCE
#define VECTOR_INCIDENCE

#include <algorithm>

#include "../Graph/IIncidence.hpp"
#include "../Graph/Iterator.hpp"
#include "../Graph/IteratorWrapper.hpp"

template<typename TEdge>
class VectorIncidence: public graph::IIncidence<TEdge>
{
public: // built-in iterator definition
	class VectorIterator: public graph::Iterator<TEdge>
	{
	public:
		VectorIterator() {}
		explicit VectorIterator(std::vector<size_t>::const_iterator it_): it(it_) {}
		virtual ~VectorIterator() {}
		
		virtual std::unique_ptr<graph::Iterator<TEdge>> clone() const {
			return std::unique_ptr<graph::Iterator<TEdge>>(new VectorIterator(it));
		}
		
		virtual graph::Iterator<TEdge>& operator++ () {
			++it;
			return *this;
		}
		virtual const TEdge& operator* () const {
			return *it;
		}
		virtual bool operator!= (const graph::Iterator<TEdge>& iterator) const {
			return it != dynamic_cast<const VectorIterator&>(iterator).it; // todo: catch bad_cast
		}
		
	private:
		std::vector<size_t>::const_iterator it;
	};
	
public: // definition of methods
	VectorIncidence() {}
	virtual ~VectorIncidence() {} // TODO: add copy constructors

	virtual std::unique_ptr<graph::IIncidence<TEdge>> clone() const {
		return std::unique_ptr<graph::IIncidence<TEdge>>(new VectorIncidence(*this));
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

	virtual graph::IteratorWrapper<TEdge> begin() const {
		return graph::IteratorWrapper<TEdge>(new VectorIterator(adjacent.begin()));
	}
	virtual graph::IteratorWrapper<TEdge> end() const {
		return graph::IteratorWrapper<TEdge>(new VectorIterator(adjacent.end()));
	}

private:
	std::vector<size_t> adjacent;
	
};

#endif
