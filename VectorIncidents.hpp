#ifndef VECTORINCIDENTS_HPP
#define VECTORINCIDENTS_HPP
#include <vector>
#include "Incidents.hpp"
#include <typeinfo>
class VectorIncidents : public Incidents {
public:
	class BaseIterator : public Incidents::BaseIterator {
	public:
		explicit BaseIterator(const std::vector<size_t>::const_iterator& iter): iter(iter){}
		virtual void operator ++ () {
			++iter;
		}
		virtual size_t operator * () const {
			return *iter;
		}
		virtual bool operator != (const Incidents::BaseIterator& base) const {
			try{
				const BaseIterator& second = dynamic_cast<const BaseIterator&>(base);
				return iter != second.iter;
			}
			catch(std::bad_cast){
				return false;
			}
		}
	private:
		std::vector<size_t>::const_iterator iter;
	};

	VectorIncidents(){}
	explicit VectorIncidents(const std::vector<size_t>& v): incidents(v) {}
	
	virtual Iterator begin() const{
		return Iterator(new BaseIterator(incidents.begin()));
	}
	virtual Iterator end() const{
		return Iterator(new BaseIterator(incidents.end()));
	}
	
	size_t size() const {
		return incidents.size();
	}
	
	virtual void addEdge(size_t to){
		incidents.push_back(to);
	}
	
	virtual void removeEdge(size_t to){
		std::vector<size_t>::const_iterator founded=std::find(incidents.begin(),incidents.end(), to);
		if(founded !=incidents.end())
			incidents.erase(founded);
	}
	
	virtual bool checkEdge(size_t to) const {
		return std::find(incidents.begin(),incidents.end(), to) != incidents.end();
	}
	
private:
	std::vector<size_t> incidents;
};
#endif /* VECTORINCIDENTS_HPP */

