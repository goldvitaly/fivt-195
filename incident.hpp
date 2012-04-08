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
        virtual void operator ++ () = 0;
        virtual size_t operator * () const = 0;
        virtual bool operator == (const p_iterator &It) const = 0;
        virtual bool operator != (const p_iterator &It) const = 0;
        virtual ~p_iterator (){};
    };
    
    class iterator
    {
    public:
        
        std::unique_ptr<p_iterator> _It;
        
        explicit iterator (std::unique_ptr<p_iterator> It):_It(std::move(It)){}
    
        iterator& operator ++ ()
        {
           ++(*_It);
           return *this;
        }
        
        size_t operator * () const
        {
            return *(*_It);
        }
        
        bool operator == (const iterator &It) const
        {
            return *_It == *It._It;
        }
        
        bool operator != (const iterator &It) const
        {
            return *_It != *It._It;
        }
        
        ~iterator (){};
    };

	virtual void AddIncident (size_t Vertex) = 0;
	virtual void RemoveIncident (size_t Vertex) = 0;
	virtual bool CheckIncident (size_t Vertex) const = 0;
	virtual size_t IncidentNum () const = 0;
	virtual iterator begin () const = 0;
	virtual iterator end () const = 0;

	virtual ~Incident (){};
};

class VBoolIncident : public Incident
{
public: 
    class p_iterator : public Incident::p_iterator
    {
    public:
        p_iterator ()
        {
            _Index = -1;
        }
        p_iterator (const std::vector <bool> &_a, size_t index): _a(_a)
        {
            _Index = 0;
            while (_Index < _a.size() && _Index != index && _a[_Index] == false)
            {
                _Index++;
            }
        }
        void operator ++ ()
        {
            do
            {
                _Index++;
            } while (_Index != _a.size() && _a[_Index] == false);
        }
        size_t operator * () const
        {
            return _Index;
        }
        bool operator == (const VBoolIncident::p_iterator &It) const
        {
            return _Index == It._Index;
        }
        bool operator != (const VBoolIncident::p_iterator &It) const
        {
            return _Index != It._Index;
        }
        ~p_iterator (){};
    private:
        const std::vector <bool> _a;
        size_t _Index;
    };

    VBoolIncident ():_IncidentNum(0), _a(){}
    
	virtual void AddIncident (size_t Vertex)
	{
        if (Vertex >= _a.size())
            _a.resize(Vertex + 1, false);
        if (!_a[Vertex]) _IncidentNum++;
        _a[Vertex] = true;
    }
    
	virtual void RemoveIncident (size_t Vertex)
	{
        if (Vertex < _a.size())
        {
            if (_a[Vertex]) _IncidentNum--;
            _a[Vertex] = false;
        }
    }
    
	virtual bool CheckIncident (size_t Vertex) const
	{
        if (Vertex >= _a.size())
            return false;
        return _a[Vertex];
    }
    
	virtual size_t IncidentNum () const
	{
        return _IncidentNum;
    }
    
	virtual iterator begin () const
	{
	    p_iterator* it = new VBoolIncident::p_iterator();//(_a, 0);
        //return iterator(std::unique_ptr<Incident::p_iterator>(NULL));//new VBoolIncident::p_iterator(_a, 0)));
               //Iterator(std::unique_ptr<Incidents::BaseIterator>(new BaseIterator(incidents)));
    }

	virtual iterator end () const
	{
        //return iterator(std::unique_ptr<p_iterator>(new p_iterator(_a, _IncidentNum)));
    }
    
    virtual ~VBoolIncident() {}
private:
    size_t _IncidentNum;
    std::vector <bool> _a;
};

#endif /* INCIDENT_HPP */