/*
 * =====================================================================================
 *
 *       Filename:  SetIncidence.hpp
 *
 *    Description:  SetIncidence class implements vertex incidence that stores edges in unordered_map
 *
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */
#ifndef SETINCIDENCE_HPP
#define SETINCIDENCE_HPP 

#include <vector>
#include <stdexcept>

#include "IncidenceType.hpp"

template<typename EdgeType>
class SetIncidence : public IncidenceType<EdgeType>
{
 public: 
  void addEdge(EdgeType edge)
  {
    incident_.insert(std::make_shared<EdgeType> (edge));
  }

  void removeEdge(EdgeType edge)
  {
    incident_.erase(std::make_shared<EdgeType> (edge));
  }

  void clear()
  {
    incident_.clear();
  }

  bool hasEdge(EdgeType edge) const
  {
    return (incident_.find(std::make_shared<EdgeType> (edge)) != incident_.end());
  }

  size_t degree() const
  {
    return incident_.size();
  }

  typename IncidenceType<EdgeType>::Iterator begin() 
  {
    return typename IncidenceType<EdgeType>::Iterator(new SetBaseIterator(incident_.begin()));
  }

  typename IncidenceType<EdgeType>::Iterator end() 
  {
    return typename IncidenceType<EdgeType>::Iterator(new SetBaseIterator(incident_.end()));
  }

  class SetBaseIterator : public IncidenceType<EdgeType>::BaseIterator
  {
   public:
    explicit SetBaseIterator(typename std::unordered_set< std::shared_ptr<EdgeType> >::iterator it) 
      : set_iterator(it)
    {
    }

    EdgeType& operator *() 
    {
      return **set_iterator;
    }

    SetBaseIterator& operator ++()
    {
      set_iterator++;
      return *this;
    }

    bool operator ==(const typename IncidenceType<EdgeType>::BaseIterator& it) const
    {
      const SetBaseIterator& s_it = dynamic_cast<const SetBaseIterator&> (it);
      return (set_iterator == s_it.set_iterator);
    }

    bool operator !=(const typename IncidenceType<EdgeType>::BaseIterator& it) const
    {
      return !(*this == it);
    }

    ~SetBaseIterator()
    {
    }

   private:
    typename std::unordered_set< std::shared_ptr<EdgeType> >::iterator set_iterator;

  }; // SetBaseIterator

 private:
  struct EdgeEquality
    : std::binary_function<std::shared_ptr<EdgeType>, std::shared_ptr<EdgeType>, bool>
  {
      bool operator()(std::shared_ptr<EdgeType> i1, std::shared_ptr<EdgeType> i2) const {
          return *i1 == *i2;
      }
  };

  std::unordered_set< std::shared_ptr<EdgeType>, 
                      std::hash<std::shared_ptr<EdgeType> >, EdgeEquality > incident_;

}; // SetIncidence

#endif /* SETINCIDENCE_HPP */