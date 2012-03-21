#ifndef INCIDENT_HPP
#define INCIDENT_HPP

#include <vector>
#include <memory>
#include <utility>

class Incident
{
public:
    class p_iterator 
    {
    public:
        virtual p_iterator& operator ++ ()                           = 0;
        virtual size_t      operator *  ()                     const = 0;
        virtual bool        operator == (const p_iterator &It) const = 0;
        virtual bool        operator != (const p_iterator &It) const = 0;
        virtual             ~p_iterator   (){};
    };
    
    class iterator
    {
    public:
    
        iterator (std::unique_ptr<p_iterator> It):_It(std::move(It)){}
        
        void operator ++ ()
        {
           ++(*_It);
        }
        
        size_t operator * () const
        {
            return *(*_It);
        }
        
        bool operator == (const iterator &It) const
        {
            return *_It == *It.GetIterator();
        }
        
        bool operator != (const iterator &It) const
        {
            return *_It == *It.GetIterator();
        }
        
        std::unique_ptr<p_iterator> GetIterator () const
        {
            return _It;
        }
        
        ~iterator   (){};
        
    private:
        std::unique_ptr<p_iterator> _It;
    };

	virtual void     AddIncident    (size_t Vertex)       = 0;
	virtual void     RemoveIncident (size_t Vertex)       = 0;
	virtual bool     CheckIncident  (size_t Vertex) const = 0;
	virtual size_t   IncidentNum    ()              const = 0;
	virtual iterator begin          ()              const = 0;
	virtual iterator end            ()              const = 0;

	virtual ~Incident (){};
};

class VBoolIncident : public Incident
{
public:

    MyIncident ():_IncidentNum(0), _a(){}
    
	void   AddIncident    (size_t Vertex)
	{
        if (Vertex >= _a.size())
            _a.resize(Vertex + 1, false);
        if (!_a[Vertex]) _IncidentNum++;
        _a[Vertex] = true;
    }
    
	void  RemoveIncident (size_t Vertex)
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
    
	size_t begin () const
	{
        for (auto i : _a)
            if (true);
        return End();
    }

	size_t end () const;
	{
        return _a.size();
    }
    
    ~MyIncident (){}
private:
	size_t _IncidentNum;
	std::vector <bool> _a;
};

#endif /* INCIDENT_HPP */