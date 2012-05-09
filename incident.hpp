#ifndef INCIDENT_HPP
#define INCIDENT_HPP

#include <typeinfo>
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
        virtual bool operator != (const p_iterator &It) const = 0;
        virtual ~p_iterator (){};
    };
    
    class iterator
    {
    public:
        
        std::unique_ptr<p_iterator> _It;
        
        iterator (const iterator&) = delete;
		iterator& operator = (const iterator&) = delete;
		
		iterator (iterator&& It)
		{
			_It = std::move(It._It);
		}
		
		iterator& operator = (iterator&& It)
		{
			_It = std::move(It._It);
			return *this;
		}
		
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
        
        bool operator != (iterator &It) const
        {
            return *_It != *(It._It);
        }
        
        ~iterator (){}
    };

	virtual void AddIncident (size_t Vertex) = 0;
	virtual void RemoveIncident (size_t Vertex) = 0;
	virtual bool CheckIncident (size_t Vertex) const = 0;
	virtual size_t IncidentNum () const = 0;
	virtual iterator begin () const = 0;
	virtual iterator end () const = 0;
	virtual void clear () = 0;

	virtual ~Incident (){}
};

class VBoolIncident : public Incident
{
public: 
    class p_iterator : public Incident::p_iterator
    {
    public:
        p_iterator (const std::vector <bool> &_a, size_t index = 0): _a(_a)
        {
            _Index = 0;
            while (_Index != _a.size() && (index != 0 || _a[_Index] != true))
            {
                index -= _a[_Index];
                _Index++;
            }
        }
        void operator ++ ()
        {
            do
            {
                _Index++;
            } while (_Index != _a.size() && _a[_Index] != true);
        }
        size_t operator * () const
        {
            return _Index;
        }
        bool operator != (const Incident::p_iterator &It) const
        {
            return _Index != dynamic_cast<const VBoolIncident::p_iterator&>(It)._Index;
        }
        ~p_iterator (){}
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
        return iterator(std::unique_ptr<p_iterator>(new p_iterator(_a)));
    }

	virtual iterator end () const
	{
        return iterator(std::unique_ptr<p_iterator>(new p_iterator(_a, _IncidentNum)));
    }
    
    virtual void clear ()
    {
        _a.clear();
        _IncidentNum = 0;
    }
    
    virtual ~VBoolIncident() {}
private:
    size_t _IncidentNum;
    std::vector <bool> _a;
};

class VIntIncident : public Incident
{
public: 
    class p_iterator : public Incident::p_iterator
    {
    public:
        size_t _Index;
        
        p_iterator (const std::vector <size_t> &_a, size_t index = 0): _a(_a)
        {
            _Index = index;
        }
        void operator ++ ()
        {
            _Index++;
        }
        size_t operator * () const
        {
            return _a[_Index];
        }
        bool operator != (const Incident::p_iterator &It) const
        {
            return _Index != dynamic_cast<const VIntIncident::p_iterator&>(It)._Index;
        }
        ~p_iterator (){}
        
    private:
        const std::vector <size_t> _a;
    };

    VIntIncident ():_a(){}
    
	virtual void AddIncident (size_t Vertex)
	{
        _a.push_back(Vertex);
    }
    
	virtual void RemoveIncident (size_t Vertex)
	{
	    std::vector <size_t> _newa;
        for (size_t i = 0; i < _a.size(); ++i)
            if (_a[i] != Vertex)
                _newa.push_back(_a[i]);
        _a = _newa;
    }
    
	virtual bool CheckIncident (size_t Vertex) const
	{
        for (size_t i = 0; i < _a.size(); ++i)
            if (_a[i] == Vertex)
                return true;
        return false;
    }
    
	virtual size_t IncidentNum () const
	{
        return _a.size();
    }
    
	virtual VIntIncident::iterator begin () const
	{
        return iterator(std::unique_ptr<p_iterator>(new p_iterator(_a)));
    }

	virtual VIntIncident::iterator end () const
	{
        return iterator(std::unique_ptr<p_iterator>(new p_iterator(_a, _a.size())));
    }
    
    virtual void clear ()
    {
        _a.clear();
    }
    
    virtual ~VIntIncident() {}
private:
    std::vector <size_t> _a;
};

#endif /* INCIDENT_HPP */
