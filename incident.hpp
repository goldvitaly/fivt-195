#ifndef GRAPH_ALGORITHM_HPP
#define GRAPH_ALGORITHM_HPP

#include <vector>
#include <memory>
#include <utility>

class Incident
{
public:
    class _iterator 
    {
    public:
        virtual void   operator ++ ()                          = 0;
        virtual size_t operator *  ()                    const = 0;
        virtual bool   operator == (const _iterator &It) const = 0;
        virtual bool   operator != (const _iterator &It) const = 0;
        virtual void   operator =  (const _iterator &It)       = 0;
        
        virtual        ~_iterator   (){};
    };
    
    class iterator
    {
    public:
        void   operator ++ ();
        size_t operator *  ()                   const;
        bool   operator == (const iterator &It) const;
        bool   operator != (const iterator &It) const;
        void   operator =  (const iterator &It);
        
               ~iterator   (){};
    private:
        std::unique_ptr<_iterator> _It;
    };

	virtual void     AddIncident    (size_t Vertex)       = 0;
	virtual void     RemoveIncident (size_t Vertex)       = 0;
	virtual bool     CheckIncident  (size_t Vertex) const = 0;
	virtual size_t   IncidentNum    ()              const = 0;
	virtual iterator begin          ()              const = 0;
	virtual iterator end            ()              const = 0;

	virtual ~Incident (){};
};

class MyIncident : public Incident
{
public:
	MyIncident ()
	{
		_IncidentNum = 0;
	}
	void AddIncident (size_t Vertex)
	{
		if (Vertex >= _a.size())
			_a.resize(Vertex + 1, false);
		if (!_a[Vertex]) _IncidentNum++;
		_a[Vertex] = true;
	}
	void RemoveIncident (size_t Vertex)
	{
		if (Vertex < _a.size())
		{
			if (_a[Vertex]) _IncidentNum--;
			_a[Vertex] = false;
		}
	}
	bool CheckIncident (size_t Vertex) const
	{
		if (Vertex >= _a.size())
			return false;
		return _a[Vertex];
	}
	size_t IncidentNum () const
	{
		return _IncidentNum;
	}
	size_t Begin () const
	{
		for (size_t i = 0; i < _a.size(); ++i)
			if (_a[i])
				return i;
		return End();
	}
	size_t Next (size_t Vertex) const
	{
		for (size_t i = Vertex + 1; i < _a.size(); ++i)
			if (_a[i])
				return i;
		return End();
	}
	size_t End () const
	{
		return _a.size();
	}
	~MyIncident (){}
private:
	size_t _IncidentNum;
	std::vector <bool> _a;
};
