#ifndef INCIDENTS_H
#define INCIDENTS_H
#include <algorithm>
#include <memory>
#include <utility>
#include <set>

template <class Weight>
struct Edge
{
	int to;
	Weight weight;
	bool operator < (const Edge &a) const
	{
		return (to < a.to); 				
	}
	bool operator == (const Edge &a) const
	{
		return (to == a.to);	
	}			
};

template <class Weight>
class BaseIterator
{
public:
	virtual bool operator != (const BaseIterator&) const = 0;
	virtual BaseIterator& operator ++() = 0;
	virtual Edge<Weight> operator *() const = 0;
	virtual ~BaseIterator()
	{}
};

template <class Weight>
class Incidents 
{
	public:
		class Iterator 
		{
			public:
				explicit Iterator(std::unique_ptr<BaseIterator<Weight>> p):base(std::move(p))
				{} 
				bool operator != (const Iterator& second)
				{
					return *base != *second.base;
				}
				Iterator& operator ++()
				{
					++(*base);
					return *this;
				}
				
				Edge<Weight> operator *() const 
				{
					return **base;
				}
			private:
				std::unique_ptr<BaseIterator<Weight>> base;
		};
		
		virtual void add(const Edge<Weight> &to) = 0;
		virtual void remove(const Edge<Weight> &to) = 0;
		virtual bool check(const Edge<Weight> &to) const = 0;
		virtual Iterator begin() const = 0;
		virtual Iterator end() const = 0;
		virtual ~Incidents()
		{}
};

template <class Itr, class Weight>
class STLGraphIterator : public BaseIterator<Weight>
{
	public:
		STLGraphIterator(Itr iter, Itr end) : cur(iter),end(end){}
		virtual ~STLGraphIterator()
		{}
		bool operator != (const BaseIterator<Weight> &second) const
		{
			return cur != (dynamic_cast<const STLGraphIterator&>(second)).cur;
		}
		BaseIterator<Weight>& operator ++()
		{
			++cur;
			return *this;
		}
		Edge<Weight> operator *() const
		{
			return *cur;
		}
		Itr cur, end;
};

template <class Weight>
class VecIncidents : public Incidents <Weight>
{
	typedef typename Incidents<Weight>::Iterator Iterator;
	public:
		void add(const Edge<Weight> &to)
		{
			incidents.push_back(to);
		}
		void remove(const Edge<Weight> &to)
		{
			auto iter = std::find(incidents.begin(), incidents.end(), to);
			if(iter != incidents.end())
				incidents.erase(iter);
		}
		bool check(const Edge<Weight> &to) const
		{
			auto iter = std::find(incidents.begin(), incidents.end(), to);
			return iter != incidents.end();
		}
		typedef STLGraphIterator<typename std::vector<Edge<Weight>>::const_iterator, Weight> InnerIterator;
		Iterator begin() const 
		{ 
			return Iterator(std::unique_ptr<BaseIterator<Weight>>(new InnerIterator(incidents.begin(), incidents.end())));
		}
		Iterator end() const 
		{
			return Iterator(std::unique_ptr<BaseIterator<Weight>>(new InnerIterator(incidents.end  (), incidents.end())));
		}
	private:
		
		std::vector<Edge<Weight>> incidents;
};

template<class Weight>
class SetIncidents : public Incidents <Weight>
{
	typedef typename Incidents<Weight>::Iterator Iterator;
	public:
		void add(const Edge<Weight> &to)
		{
			incidents.insert(to);
		}
		void remove(const Edge<Weight> &to)
		{
			incidents.erase(to);
		}
		bool check(const Edge<Weight> &to) const
		{
			return incidents.find(to) != incidents.end();
		}
		typedef STLGraphIterator<typename std::set<Edge<Weight>>::const_iterator, Weight> InnerIterator;
		Iterator begin() const
		{ 
			return Iterator(std::unique_ptr<BaseIterator<Weight>>(new InnerIterator(incidents.begin(), incidents.end())));
		}
		Iterator end() const
		{
			return Iterator(std::unique_ptr<BaseIterator<Weight>>(new InnerIterator(incidents.end  (), incidents.end())));
		}
	private:
		std::set<Edge<Weight>> incidents;
};

#endif /* INCIDENTS_H */
 
