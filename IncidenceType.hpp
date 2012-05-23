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
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <memory>

#define print(x) std::cout << x << std::endl;

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

template<typename EdgeType>
class VertexIncidenceType
{
 public:
  explicit VertexIncidenceType(IncidenceType<EdgeType>* incident_type)
  {
    incident_type_ = std::unique_ptr< IncidenceType<EdgeType> > (incident_type);
  }

  explicit VertexIncidenceType(VertexIncidenceType<EdgeType>&& vertex_incidence_type)
  {
    incident_type_ = std::move(vertex_incidence_type.incident_type_);
  }

  void addEdge(EdgeType edge)
  {
    incident_type_->addEdge(edge);
  }

  void removeEdge(EdgeType edge)
  {
    incident_type_->removeEdge(edge);
  }

  void clear()
  {
    incident_type_->clear();
  }

  bool hasEdge(EdgeType edge) const
  {
    return incident_type_->hasEdge(edge);
  }

  size_t degree() const
  {
    return incident_type_->degree();
  }

  typename IncidenceType<EdgeType>::Iterator begin() const
  {
    return incident_type_->begin();
  }

  typename IncidenceType<EdgeType>::Iterator end() const
  {
    return incident_type_->end();
  }

  std::string getType() const
  {
    return typeid(*incident_type_).name();
  }
  
 private:
  std::unique_ptr< IncidenceType<EdgeType> > incident_type_;

}; // VertexIncidenceType<EdgeType>

#endif /* INCIDENCETYPE_HPP */
