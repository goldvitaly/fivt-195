/*
 * =====================================================================================
 *
 *       Filename:  VectorIncidence.hpp
 *
 *    Description:  VectorIncidence class implements vertex incidence that stores edges in vector
 *
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */
#ifndef VECTORINCIDENCE_HPP
#define VECTORINCIDENCE_HPP 

#include <vector>
#include <stdexcept>

#include "IncidenceType.hpp"

template<typename EdgeType>
class VectorIncidence : public IncidenceType<EdgeType>
{
 public: 
  void addEdge(EdgeType edge)
  {
    incident_.push_back(edge);
  }

  void removeEdge(EdgeType edge)
  {
    typename std::vector<EdgeType>::iterator it = find(incident_.begin(), incident_.end(), edge);
    if (it != incident_.end())
      incident_.erase(it);
  }

  void clear()
  {
    incident_.clear();
  }

  bool hasEdge(EdgeType edge) const
  {
    return find(incident_.begin(), incident_.end(), edge) != incident_.end();
  }

  size_t degree() const
  {
    return incident_.size();
  }

  typename IncidenceType<EdgeType>::Iterator begin() const
  {
    return typename IncidenceType<EdgeType>::Iterator(new VectorBaseIterator(incident_.begin()));
  }

  typename IncidenceType<EdgeType>::Iterator end() const
  {
    return typename IncidenceType<EdgeType>::Iterator(new VectorBaseIterator(incident_.end()));
  }
  
  class VectorBaseIterator : public IncidenceType<EdgeType>::BaseIterator
  {
   public:
    explicit VectorBaseIterator(typename std::vector<EdgeType>::const_iterator it) : vector_iterator(it)
    {
    }

    EdgeType operator *() const
    {
      return *vector_iterator;
    }

    VectorBaseIterator& operator ++()
    {
      vector_iterator++;
      return *this;
    }

    bool operator ==(const typename IncidenceType<EdgeType>::BaseIterator& it) const
    {
      const VectorBaseIterator& v_it = dynamic_cast<const VectorBaseIterator&> (it);
      return (vector_iterator == v_it.vector_iterator);
    }

    bool operator !=(const typename IncidenceType<EdgeType>::BaseIterator& it) const
    {
      return !(*this == it);
    }

    ~VectorBaseIterator()
    {
    }

   private:
    typename std::vector<EdgeType>::const_iterator vector_iterator;
    
  }; // VectorBaseIterator

 private:
  std::vector<EdgeType> incident_;

}; // VectorIncidence

#endif /* VECTORINCIDENCE_HPP */
