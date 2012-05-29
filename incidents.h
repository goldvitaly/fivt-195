#ifndef INCIDENTS_H
#define INCIDENTS_H
#include <algorithm>
#include <memory>
#include <utility>
#include <set>

struct Edge
{
	int to, weight;
	bool operator < (const Edge &a) const
	{
		return (to < a.to); 				
	}
	bool operator == (const Edge &a) const
	{
		return (to == a.to);	
	}			
};

class BaseIterator
{
public:
	virtual bool operator != (const BaseIterator&) const = 0;
	virtual BaseIterator& operator ++() = 0;
	virtual Edge operator *() const = 0;
	virtual ~BaseIterator()
	{}
};

class Incidents 
{
	public:
		class Iterator 
		{
			public:
				explicit Iterator(std::unique_ptr<BaseIterator> p):base(std::move(p))
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
				
				Edge operator *() const 
				{
					return **base;
				}
			private:
				std::unique_ptr<BaseIterator> base;
		};
		
		virtual void add(Edge to) = 0;
		virtual void remove(Edge to) = 0;
		virtual bool check(Edge to) const = 0;
		virtual Iterator begin() const = 0;
		virtual Iterator end() const = 0;
		virtual ~Incidents()
		{}
};

template <typename Itr>
class STLGraphIterator : public BaseIterator
{
	public:
		STLGraphIterator(Itr iter, Itr end) : cur(iter),end(end){}
		virtual ~STLGraphIterator()
		{}
		bool operator != (const BaseIterator& second) const
		{
			return cur != (dynamic_cast<const STLGraphIterator&>(second)).cur;
		}
		BaseIterator& operator ++()
		{
			++cur;
			return *this;
		}
		Edge operator *() const
		{
			return *cur;
		}
		Itr cur, end;
};

class VecIncidents : public Incidents
{
	public:
		void add(Edge to)
		{
			incidents.push_back(to);
		}
		void remove(Edge to)
		{
			auto iter = std::find(incidents.begin(), incidents.end(), to);
			if(iter != incidents.end())
				incidents.erase(iter);
		}
		bool check(Edge to) const
		{
			auto iter = std::find(incidents.begin(), incidents.end(), to);
			return iter != incidents.end();
		}
		typedef STLGraphIterator<std::vector<Edge>::const_iterator > InnerIterator;
		Iterator begin() const 
		{ 
			return Iterator(std::unique_ptr<BaseIterator>(new InnerIterator(incidents.begin(), incidents.end())));
		}
		Iterator end() const 
		{
			return Iterator(std::unique_ptr<BaseIterator>(new InnerIterator(incidents.end  (), incidents.end())));
		}
	private:
		
		std::vector<Edge> incidents;
};

class SetIncidents : public Incidents 
{
	public:
		void add(Edge to)
		{
			incidents.insert(to);
		}
		void remove(Edge to)
		{
			incidents.erase(to);
		}
		bool check(Edge to) const
		{
			return incidents.find(to) != incidents.end();
		}
		typedef STLGraphIterator<std::set<Edge>::const_iterator> InnerIterator;
		Iterator begin() const
		{ 
			return Iterator(std::unique_ptr<BaseIterator>(new InnerIterator(incidents.begin(), incidents.end())));
		}
		Iterator end() const
		{
			return Iterator(std::unique_ptr<BaseIterator>(new InnerIterator(incidents.end  (), incidents.end())));
		}
	private:
		std::set<Edge> incidents;
};

#endif /* INCIDENTS_H */
 
