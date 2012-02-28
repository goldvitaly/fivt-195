#ifndef INCIDENTS_HPP
#define INCIDENTS_HPP
#include <typeinfo>
#include <memory>

class Incidents {
public:
	class BaseIterator {
	public:
		virtual void operator ++ () = 0;
		virtual size_t operator * () const = 0;
		virtual bool operator != (const BaseIterator&) const = 0;
	};
	class Iterator {
	public:
		BaseIterator* it;
		//reaches ownership
		explicit Iterator(BaseIterator* iter):it(iter){
			//std::cerr<<"Create Iter"<<this<<' '<<it.<<std::endl;
		}
		void operator ++ (){
			++(*it);
			//return *this;
		}
		size_t operator * () const {
			return **it;
		}
		~Iterator() {
			//std::cerr<<"destroy iter"<<this<<' '<<it<<std::endl;
			delete it;
		}
		bool operator != (Iterator& second) const {
			/*std::cerr<<"Start Iterator::!="<<std::endl;
			std::cerr<<"Current obj: "<<this<<' '<<it<<std::endl;
			std::cerr<<"Second obj: "<<&second<<' '<<second.it<<std::endl;
			std::cerr<<"Current obj type"<<typeid(*it).hash_code()<<std::endl;
			std::cerr<<"Secont obj type"<<typeid(*(second.it)).hash_code()<<std::endl;
			std::cerr<<"Call child !="<<std::endl;*/
			return (*it) != *(second.it);
		}
	};
	virtual Iterator begin() const = 0;
	virtual Iterator end() const = 0;
	virtual size_t count() const = 0;
	virtual void addEdge(size_t to) = 0;
	virtual void removeEdge(size_t to) = 0;
	virtual bool checkEdge(size_t to) const = 0;
	virtual ~Incidents(){}
};

#endif /* INCIDENTS_HPP */