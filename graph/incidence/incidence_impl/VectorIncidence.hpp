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
    incident_.push_back(std::make_shared<EdgeType>(edge));
  }

  void removeEdge(EdgeType edge)
  {
    /*
    typename std::vector<EdgeType>::iterator it = find(incident_.begin(), incident_.end(), edge);
    if (it != incident_.end())
      incident_.erase(it);
    */
  }

  void clear()
  {
    incident_.clear();
  }

  bool hasEdge(EdgeType edge) const
  {
    return false;
    /*
    return find(incident_.begin(), incident_.end(), edge) != incident_.end();
    */
  }

  size_t degree() const
  {
    return incident_.size();
  }

  typename IncidenceType<EdgeType>::Iterator begin()
  {
    return typename IncidenceType<EdgeType>::Iterator(new VectorBaseIterator(incident_.begin()));
  }

  typename IncidenceType<EdgeType>::Iterator end()
  {
    return typename IncidenceType<EdgeType>::Iterator(new VectorBaseIterator(incident_.end()));
  }
  
  class VectorBaseIterator : public IncidenceType<EdgeType>::BaseIterator
  {
   public:
    explicit VectorBaseIterator(typename std::vector<std::shared_ptr<EdgeType> >::iterator it) 
      : vector_iterator(it)
    {
    }

    EdgeType& operator *() 
    {
      return **vector_iterator;
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
    typename std::vector< std::shared_ptr<EdgeType> >::iterator vector_iterator;
    
  }; // VectorBaseIterator

 private:
  std::vector< std::shared_ptr<EdgeType> > incident_;

}; // VectorIncidence

#endif /* VECTORINCIDENCE_HPP */
