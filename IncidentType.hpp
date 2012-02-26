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
  void   AddEdge(int destination);
  void   RemoveEdge(int destination);
  bool   IsConnectedTo(int destination);
  size_t EdgeCount();

  class iterator
  {
   /*public:
    iterator()
    {
      pointer = NULL;
    }
    iterator(IncidentType* ptr)
    {
      pointer = ptr;
    }
    operator
   private:    
    IncidentType* pointer;*/
  };
 private:
  
}; // IncidentType

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

class ListIncident : public IncidentType
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
  class iterator : public std::vector<int>::iterator, public IncidentType::iterator {};

 private:
  std::vector<int> incident_;
}; // ListIncidence

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

#endif /* INCIDENTTYPE_HPP */
