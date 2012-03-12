/*
 * =====================================================================================
 *
 *       Filename:  IncidentType.hpp
 *
 *    Description:  IncidentType class description
 *
 *       Compiler:  gcc
 *
 *         Author:  IIoTeP9HuY, IIoTeP9HuY@gmail.com
 *        Company:  MIPT
 *
 * =====================================================================================
 */
#ifndef INCIDENTTYPE_HPP
#define INCIDENTTYPE_HPP 

#include <cstdlib>
#include <cstdio>
#include <vector>
#include <set>
#include <algorithm>

class IncidentType
{
 public:
  class Iterator;
  
  virtual void     AddEdge(int destination)       = 0;
  virtual void     RemoveEdge(int destination)    = 0;
  virtual bool     IsConnectedTo(int destination) = 0;
  virtual size_t   EdgeCount() = 0;
  virtual Iterator begin()     = 0;
  virtual Iterator end()       = 0;

  class BaseIterator
  {
   public:
    virtual bool operator ==(BaseIterator& it)  = 0;
    virtual bool operator !=(BaseIterator& it)  = 0;
    virtual size_t        operator *()   = 0;
    virtual BaseIterator& operator ++()  = 0;
    virtual ~BaseIterator()              = 0;
  };
  
  class Iterator
  {
   public:
    BaseIterator* iterator; // unique_ptr<BaseIterator>

    Iterator(BaseIterator* base_iterator)
    {
      iterator = base_iterator;
    }
    ~Iterator()
    {
      delete iterator;
    }
    size_t operator *()
    {
      return **iterator;
    }
    Iterator operator ++()
    {
      printf("Iterator increment\n");
      ++(*iterator);
      return iterator;
    }
    bool operator ==(Iterator it)
    {
      return *iterator == *it.iterator;
    }
    bool operator !=(Iterator it)
    {
      return *iterator != *it.iterator;
    }
  };

 private:
  
}; // IncidentType

IncidentType::BaseIterator::~BaseIterator() {}

/*
class SetIncident : public IncidentType
{
 public: 
  void AddEdge(int destination)
  {
    incident_.insert(destination);
  }
  void RemoveEdge(int destination)
  {
    incident_.erase(destination);
  }
  bool IsConnectedTo(int destination)
  {
    return (incident_.find(destination) != incident_.end());
  }
  size_t EdgeCount()
  {
    return incident_.size();
  }
  class iterator : public std::set<int>::iterator, IncidentType::iterator {};

 private:
  std::set<int> incident_;
}; // SetIncidence
*/

class VectorIncident : public IncidentType
{
 public: 
  void AddEdge(int destination)
  {
    incident_.push_back(destination);
  }
  void RemoveEdge(int destination)
  {
    std::vector<int>::iterator it = find(incident_.begin(), incident_.end(), destination);
    if (it != incident_.end())
      incident_.erase(it);
  }
  bool IsConnectedTo(int destination)
  {
    return (find(incident_.begin(), incident_.end(), destination) != incident_.end());
  }
  size_t EdgeCount()
  {
    return incident_.size();
  }
  Iterator begin()
  {
    return Iterator(new VectorBaseIterator(incident_.begin()));
  }
  Iterator end()
  {
    return Iterator(new VectorBaseIterator(incident_.end()));
  }
  
  class VectorBaseIterator : public BaseIterator
  {
   public:
    std::vector<int>::iterator vector_iterator;

    VectorBaseIterator(std::vector<int>::iterator it)
    {
      vector_iterator = it;
    }
    size_t operator *()
    {
      return *vector_iterator;
    }
    VectorBaseIterator& operator ++()
    {
      printf("VectorBaseIterator increment\n");
      vector_iterator++;
      return *this;
    }
    bool operator ==(BaseIterator& it)
    {
      VectorBaseIterator& v_it = dynamic_cast<VectorBaseIterator&> (it);
      return vector_iterator == v_it.vector_iterator;
    }
    bool operator !=(BaseIterator& it)
    {
      printf("Calling Comparator\n");
      VectorBaseIterator& v_it = dynamic_cast<VectorBaseIterator&> (it);
      printf("dynamic_cast no problems :) \n");
      if (vector_iterator != v_it.vector_iterator)
      {
        printf("They are different!\n");
        return 1;
      }
      else
      {
        printf("They are equal!\n");
        return 0;
      }
      //return vector_iterator != v_it.vector_iterator;
    }
    ~VectorBaseIterator()
    {
    }
  };

 private:
  std::vector<int> incident_;
}; // ListIncidence

/*
class BitsIncident : public IncidentType
{
 public: 
  void AddEdge(int destination)
  {
    if (incident_.size() < destination + 1)
      incident_.resize(destination + 1);
    incident_[destination] = 1;
  }
  void RemoveEdge(int destination)
  {
    if (incident_.size() > destination)
      incident_[destination] = 0;
  }
  bool IsConnectedTo(int destination)
  {
    if (incident_.size() < destination + 1)
      return 0;
    return incident_[destination];
  }
  size_t EdgeCount()
  {
    return count(incident_.begin(), incident_.end(), 1);
  }
  class iterator : public IncidentType::iterator
  {
   public:
    int operator*()
    {
      return iterator_;
    }
    iterator operator++()
    {
      iterator_++;
      if (iterator_ >= incident_->size())
        iterator_ = incident_->size();
      else
        while (iterator_ < incident_->size() && (incident_->at(iterator_)))
          iterator_++;
      return *this;
    }
    bool operator==(iterator it)
    {
      return (iterator_ == it.iterator_);
    }
   public:
    iterator()
    {
      iterator_ = 0;
    }
    iterator(int index, std::vector<bool>* incident_)
    {
      iterator_ = index;
    }
    int iterator_;
    std::vector<bool>* incident_;
  };

  iterator begin()
  {
    return ++iterator(-1, &incident_);
  }
  iterator end()
  {
    return iterator(incident_.size(), &incident_);
  }

 private:
  std::vector<bool> incident_;
}; // BitsIncidence
*/

#endif /* INCIDENTTYPE_HPP */
