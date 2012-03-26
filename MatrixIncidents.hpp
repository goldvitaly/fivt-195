#ifndef MATRIXINCIDENTS_HPP
#define MATRIXINCIDENTS_HPP
#include "Incidents.hpp"
#include "Vertex.hpp"
#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>
class MatrixIncidents : public Incidents<NoWeight>{
	typedef Incidents<NoWeight> CurIncidents;
	typedef CurIncidents::BaseIterator ParentBaseIterator;
	class BaseIterator : public ParentBaseIterator {
	public:
		explicit BaseIterator(const std::vector<bool>& v):v(v) {
			index = 0;
			while(index < v.size() && v[index]==0)
				++index;
		}
		BaseIterator(const std::vector<bool>& v, size_t index):v(v), index(index){}
		virtual void operator ++(){
			do{
				++index;
			}
			while(index < v.size() && v[index]==0);
		}
		virtual Vertex<NoWeight> operator * () const {
			return Vertex<NoWeight>(index);
		}
		virtual bool operator != (const ParentBaseIterator& other) const {
			try{
				const BaseIterator& second = dynamic_cast<const BaseIterator&>(other);
				return index != second.index;
			}
			catch(std::bad_cast){
				assert(false);
			}
		}
	private:
		const std::vector<bool> v;
		size_t index;
	};
public:
	explicit MatrixIncidents(size_t number):incidents(number, false){}
	MatrixIncidents(const std::vector<bool>& incidents):incidents(std::move(incidents)){}
	virtual Iterator begin() const {
		return Iterator(std::unique_ptr<ParentBaseIterator>(new BaseIterator(incidents)));
	}
	virtual Iterator end() const {
		return Iterator(std::unique_ptr<ParentBaseIterator>(new BaseIterator(incidents, incidents.size())));
	}
	virtual size_t size() const {
		return std::count(incidents.begin(), incidents.end(), true);
	};
	virtual void addEdge(size_t to, const NoWeight& = NoWeight()) {
		incidents[to] = true;
	}
	virtual void removeEdge(size_t to) {
		incidents[to] = false;
	}
	virtual bool checkEdge(size_t to) const {
		return incidents[to];
	}
private:
	std::vector<bool> incidents;
};

#endif /* MATRIXINCIDENTS_HPP */
