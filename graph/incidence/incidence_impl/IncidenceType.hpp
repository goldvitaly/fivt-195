/*
 * =====================================================================================
 *
 *       Filename:  IncidenceType.hpp
 *
 *    Description:  IncidenceType class description
 *
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */
#ifndef INCIDENCETYPE_HPP
#define INCIDENCETYPE_HPP 

#include <cstdlib>
#include <cstdio>
#include <memory>

template<typename EdgeType>
class IncidenceType
{
 public:
  class Iterator;
  
  virtual void     addEdge(EdgeType edge) = 0;
  virtual void     removeEdge(EdgeType edge) = 0;
  virtual void     clear() = 0;
  virtual bool     hasEdge(EdgeType edge) const = 0;
  virtual size_t   degree() const = 0;
  virtual Iterator begin() const = 0;
  virtual Iterator end()   const = 0;
  virtual ~IncidenceType() {}

  class BaseIterator
  {
   public:
    virtual bool operator ==(const BaseIterator& it) const = 0;
    virtual bool operator !=(const BaseIterator& it) const = 0;
    virtual EdgeType      operator *() const   = 0;
    virtual BaseIterator& operator ++()  = 0;
    virtual ~BaseIterator() {};
  }; // BaseIterator
  
  class Iterator
  {
   public:
    explicit Iterator(BaseIterator* base_iterator)
    {
      iterator_ = std::unique_ptr<BaseIterator>(base_iterator);
    }

    Iterator(IncidenceType<EdgeType>::Iterator&& iterator)
    {
      iterator_ = std::move(iterator.iterator_);
    }

    EdgeType operator *() const
    {
      return **iterator_;
    }

    Iterator& operator ++()
    {
      ++(*iterator_);
      return *this;
    }
    
    bool operator ==(const Iterator& it) const
    {
      return *iterator_ == *it.iterator_;
    }

    bool operator !=(const Iterator& it) const
    {
      return *iterator_ != *it.iterator_;
    }

    ~Iterator()
    {
    }

   private:
    std::unique_ptr<BaseIterator> iterator_;

  }; // Iterator

 private:
  
}; // IncidenceType

#endif /* INCIDENCETYPE_HPP */
