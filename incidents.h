#ifndef INCIDENTS_H
#define INCIDENTS_H
#include <algorithm>
#include <memory>
#include <utility>
#include <set>
class BaseIterator{
public:
	virtual bool operator != (const BaseIterator&) const = 0;
	virtual BaseIterator& operator ++() = 0;
	virtual int operator *() const = 0;
	virtual ~BaseIterator()
	{}
};

class Incidents {
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
				
				int operator *() const 
				{
					return **base;
				}
			private:
				std::unique_ptr<BaseIterator> base;
		};
		
		virtual void add(int to) = 0;
		virtual void remove(int to) = 0;
		virtual bool check(int to) const = 0;
		virtual Iterator begin() const = 0;
		virtual Iterator end() const = 0;
		virtual ~Incidents()
		{}
};

template <typename Itr>
class STLGraphIterator : public BaseIterator
{
	public:
		STLGraphIterator(Itr iter, Itr end):cur(iter),end(end){}
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
		int operator *() const
		{
			return *cur;
		}
		Itr cur, end;
};

class VecIncidents : public Incidents
{
	public:
		void add(int to)
		{
			incidents.push_back(to);
		}
		void remove(int to)
		{
			auto iter = std::find(incidents.begin(), incidents.end(), to);
			if(iter != incidents.end())
				incidents.erase(iter);
		}
		bool check(int to) const
		{
			auto iter = std::find(incidents.begin(), incidents.end(), to);
			return iter != incidents.end();
		}
		typedef STLGraphIterator<std::vector<int>::const_iterator > InnerIterator;
		Iterator begin() const 
		{ 
			return Iterator(std::unique_ptr<BaseIterator>(new InnerIterator(incidents.begin(), incidents.end())));
		}
		Iterator end() const 
		{
			return Iterator(std::unique_ptr<BaseIterator>(new InnerIterator(incidents.end  (), incidents.end())));
		}
	private:
		std::vector<int> incidents;
};

class SetIncidents : public Incidents 
{
	public:
		void add(int to)
		{
			incidents.insert(to);
		}
		void remove(int to)
		{
			incidents.erase(to);
		}
		bool check(int to) const
		{
			return incidents.find(to) != incidents.end();
		}
		typedef STLGraphIterator<std::set<int>::const_iterator> InnerIterator;
		Iterator begin() const
		{ 
			return Iterator(std::unique_ptr<BaseIterator>(new InnerIterator(incidents.begin(), incidents.end())));
		}
		Iterator end() const
		{
			return Iterator(std::unique_ptr<BaseIterator>(new InnerIterator(incidents.end  (), incidents.end())));
		}
	private:
		std::set<int> incidents;
};

#endif /* INCIDENTS_H */
