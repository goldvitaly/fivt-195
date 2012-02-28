#ifndef VECTORINCIDENTS_HPP
#define VECTORINCIDENTS_HPP
#include <vector>
#include "Incidents.hpp"
#include <typeinfo>
class VectorIncidents : public Incidents {
public:
	class BaseIterator : public Incidents::BaseIterator {
	public:
		BaseIterator(const std::vector<size_t>::const_iterator& iter):iter(iter){}
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
	VectorIncidents(std::vector<size_t> v): incidents(v) {}
	
	virtual Iterator begin() const{
		return Iterator(new BaseIterator(incidents.begin()));
	}
	virtual Iterator end() const{
		return Iterator(new BaseIterator(incidents.end()));
	}
	
	size_t count() const {
		return incidents.size();
	}
	
	virtual void addEdge(size_t to){
		incidents.push_back(to);
	}
	
	virtual void removeEdge(size_t to){
		for(std::vector<size_t>::iterator it=incidents.begin(); it!=incidents.end(); ++it){
			if(*it == to){
				incidents.erase(it);
				break;
			}
		}
	}
	
	virtual bool checkEdge(size_t to) const {
		for(std::vector<size_t>::const_iterator it=incidents.begin(); it!=incidents.end(); ++it){
			if(*it == to){
				return true;
			}
		}
		return false;
	}
	
private:
	std::vector<size_t> incidents;
};
#endif /* VECTORINCIDENTS_HPP */

