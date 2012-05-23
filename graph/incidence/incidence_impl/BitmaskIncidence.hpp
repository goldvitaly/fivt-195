/*
 * =====================================================================================
 *
 *       Filename:  BitmaskIncidence.hpp
 *
 *    Description:  BitmaskIncidence class implements vertex incidence that efficiently stores boolean incidence matrix
 *
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */
#ifndef BITMASKINCIDENCE_HPP
#define BITMASKINCIDENCE_HPP 

#include <vector>
#include <stdexcept>
#include <algorithm>

#include "IncidenceType.hpp"

template<typename EdgeType>
class BitmaskIncidence : public IncidenceType<EdgeType>
{
 public: 
  void addEdge(EdgeType edge)
  {
    source = edge.source;
    if (incident_.size() < edge.destination + 1)
      incident_.resize(edge.destination + 1);
    incident_[edge.destination] = 1;
  }
  
  void removeEdge(EdgeType edge)
  {
    incident_[edge.destination] = 0;
  }

  void clear()
  {
    incident_.clear();
  }

  bool hasEdge(EdgeType edge) const
  {
    return incident_[edge.destination];
  }

  size_t degree() const
  {
    return count(incident_.begin(), incident_.end(), 1);
  }

  typename IncidenceType<EdgeType>::Iterator begin() const
  {
    BitmaskBaseIterator* it = new BitmaskBaseIterator(-1, incident_, source);
    ++(*it);
    return typename IncidenceType<EdgeType>::Iterator(it);
  }

  typename IncidenceType<EdgeType>::Iterator end() const
  {
    return typename IncidenceType<EdgeType>::Iterator(new BitmaskBaseIterator(incident_.size(), incident_, source));
  }

  class BitmaskBaseIterator : public IncidenceType<EdgeType>::BaseIterator
  {
   public:
    explicit BitmaskBaseIterator(int index, const std::vector<bool>& incident, int source) : incident_(incident), 
                                                                                             iterator_(index),
                                                                                             source(source)
    {
    }

    EdgeType operator *() const
    {
      return EdgeType(source, iterator_);
    }

    BitmaskBaseIterator& operator++()
    {
      iterator_++;
      if (iterator_ >= incident_.size())
        iterator_ = incident_.size();
      else
        while (iterator_ < incident_.size() && !(incident_.at(iterator_)))
          iterator_++;
      return *this;
    }

    bool operator ==(const typename IncidenceType<EdgeType>::BaseIterator& it) const
    {
      const BitmaskBaseIterator& b_it = dynamic_cast<const BitmaskBaseIterator&> (it);
      return (iterator_ == b_it.iterator_);
    }

    bool operator !=(const typename IncidenceType<EdgeType>::BaseIterator& it) const
    {
      return !(*this == it);
    }

    ~BitmaskBaseIterator()
    {
    }

   private:
    int iterator_;
    const std::vector<bool>& incident_;
    size_t source;

  }; // BitmaskBaseIterator

 private:
  size_t source;
  std::vector<bool> incident_;

}; // BitsIncidence

#endif /* BITMASKINCIDENCE_HPP */