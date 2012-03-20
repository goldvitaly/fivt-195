#ifndef INCIDENT_HPP
#define INCIDENT_HPP

#include <vector>
#include <memory>
#include <utility>

class Incident
{
public:
    class _iterator 
    {
    public:
        virtual _iterator& operator ++ ()                          = 0;
        virtual size_t     operator *  ()                    const = 0;
        virtual bool       operator == (const _iterator &It) const = 0;
        virtual bool       operator != (const _iterator &It) const = 0;
        virtual void       operator =  (const _iterator &It)       = 0;
        
        virtual            ~_iterator   (){};
    };
    
    class iterator
    {
    public:
        iterator& operator ++ ();
        size_t    operator *  ()                   const;
        bool      operator == (const iterator &It) const;
        bool      operator != (const iterator &It) const;
        void      operator =  (const iterator &It);
        
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
	       MyIncident     ();
	void   AddIncident    (size_t Vertex);
	void   RemoveIncident (size_t Vertex);
	bool   CheckIncident  (size_t Vertex) const;
	size_t IncidentNum    ()              const;
	size_t Begin          ()              const;
	size_t Next           (size_t Vertex) const;
	size_t End            ()              const;
	       ~MyIncident    (){}
private:
	size_t _IncidentNum;
	std::vector <bool> _a;
};

#endif /* INCIDENT_HPP */