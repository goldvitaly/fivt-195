#ifndef INCIDENT_HPP
#define INCIDENT_HPP

#include <typeinfo>
#include <vector>
#include <memory>
#include <utility>

template<typename EdgeWeight>
class Incident
{
public:
    class p_iterator
    {
    public:
        virtual void operator ++ () = 0;
        virtual std::pair<size_t, EdgeWeight> operator * () const = 0;
        virtual bool operator != (const p_iterator &It) const = 0;
        virtual ~p_iterator () {};
    };

    class iterator
    {
    public:

        std::unique_ptr<p_iterator> _It;

        iterator (const iterator&) = delete;
        iterator& operator = (const iterator&) = delete;

        iterator (iterator&& It) = default;

        iterator& operator = (iterator&& It)
        {
            _It = std::move(It._It);
            return *this;
        }

        explicit iterator (std::unique_ptr<p_iterator> It):_It(std::move(It)) {}

        iterator& operator ++ ()
        {
            ++(*_It);
            return *this;
        }

        std::pair<size_t, EdgeWeight> operator * () const
        {
            return *(*_It);
        }

        bool operator != (iterator &It) const
        {
            return *_It != *(It._It);
        }

        ~iterator () {}
    };

    virtual void AddIncident (std::pair<size_t, EdgeWeight> Vertex) = 0;
    virtual void RemoveIncident (size_t Vertex) = 0;
    virtual bool CheckIncident (size_t Vertex) const = 0;
    virtual std::pair<size_t, EdgeWeight> GetIncident (size_t EdgeIndex) const = 0;
    virtual size_t IncidentNum () const = 0;
    virtual Incident<EdgeWeight>::iterator begin () const = 0;
    virtual Incident<EdgeWeight>::iterator end () const = 0;
    virtual void clear () = 0;

    virtual ~Incident () {}
};

template<typename EdgeWeight>
class VIntIncident : public Incident<EdgeWeight>
{
public:
    class p_iterator : public Incident<EdgeWeight>::p_iterator
    {
    public:
        size_t _Index;

        p_iterator (const std::vector <std::pair<size_t, EdgeWeight>> &Inc, size_t IncIndex): _Inc(Inc)
        {
            _Index = IncIndex;
        }
        void operator ++ ()
        {
            _Index++;
        }
        std::pair<size_t, EdgeWeight> operator * () const
        {
            return _Inc[_Index];
        }
        bool operator != (const typename Incident<EdgeWeight>::p_iterator &It) const
        {
            return _Index != dynamic_cast<const VIntIncident::p_iterator&>(It)._Index;
        }
        ~p_iterator () {}

    private:
        const std::vector <std::pair<size_t, EdgeWeight>> _Inc;
    };

    VIntIncident ():_Incident() {}

    virtual void AddIncident (std::pair<size_t, EdgeWeight> Vertex)
    {
        _Incident.push_back(Vertex);
    }

    virtual void RemoveIncident (size_t Vertex)
    {
        std::vector <std::pair<size_t, EdgeWeight>> _newa;
        for (size_t i = 0; i < _Incident.size(); ++i)
            if (_Incident[i].first != Vertex)
                _newa.push_back(_Incident[i]);
        _Incident = _newa;
    }

    virtual bool CheckIncident (size_t Vertex) const
    {
        for (size_t i = 0; i < _Incident.size(); ++i)
            if (_Incident[i].first == Vertex)
                return true;
        return false;
    }

    virtual std::pair<size_t, EdgeWeight> GetIncident (size_t EdgeIndex) const
    {
        if (EdgeIndex < _Incident.size()) return _Incident[EdgeIndex];
        return std::make_pair(0, EdgeWeight());
    }

    virtual size_t IncidentNum () const
    {
        return _Incident.size();
    }

    virtual typename Incident<EdgeWeight>::iterator begin () const
    {
        return typename Incident<EdgeWeight>::iterator(std::unique_ptr<p_iterator>(new p_iterator(_Incident, 0)));
    }

    virtual typename Incident<EdgeWeight>::iterator end () const
    {
        return typename Incident<EdgeWeight>::iterator(std::unique_ptr<p_iterator>(new p_iterator(_Incident, _Incident.size())));
    }

    virtual void clear ()
    {
        _Incident.clear();
    }

    virtual ~VIntIncident() {}
private:
    std::vector <std::pair<size_t, EdgeWeight>> _Incident;
};

#endif /* INCIDENT_HPP */
