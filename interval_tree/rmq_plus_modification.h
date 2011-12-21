#ifndef RMQ_ASSIGN_H_
#define RMQ_ASSIGN_H_

#include <stdexcept>

#include "interval_tree.h"
#include "functors.h"

template <class T, class Comparator = std::less<T>, class BinaryOperation = std::plus<T> > 
class RMQPlusModification
{
  public:
	Comparator comparator;
	explicit RMQPlusModification(int size_, const T& initial_value = T(), Comparator comparator = Comparator()): 
		comparator(comparator), size_(size_),
		tree(size_, initial_value, functors::min<T,Comparator>(comparator), functors::plus_equal<T>(), functors::plus_equal<T>())
	{
	}
	void apply(int l, int r, const T& modification)
	{
		if (l >= r || l < 0 || r < 0 || l > size_ || r > size_) throw std::logic_error("invalid range");
		tree.apply(l, r, modification);
	}
	T request(int l, int r)
	{
		if (l >= r || l < 0 || r < 0 || l > size_ || r > size_) throw std::logic_error("invalid range");
		return tree.request(l, r);
	}
	int size() const { return size_; };
  private:
	int size_;
	IntervalTree <T, functors::min<T,Comparator>, T, functors::plus_equal<T>, functors::plus_equal<T> > tree;
};

#endif // RMQ_ASSIGN_H_
