#ifndef INTERVAL_H_
#define INTERVAL_H_

#include <algorithm>  // for std::max
#include <cassert>    // for assert
#include <iostream>

class Interval
{
  public:
	  inline Interval left_half() const
	  {
		  return Interval(left_bound, middle());
	  }
	  inline Interval right_half() const
	  {
		  return Interval(middle(), right_bound);
	  }
	  Interval()
	  {
		  left_bound = 0;
		  right_bound = 0;
	  }
	  int middle() const
	  {
		  return (left_bound + right_bound) / 2;
	  }
	  Interval(int left, int right)
	  {
		  left_bound = left;
		  right_bound = right;
		  assert(left_bound < right_bound);
	  }
	  bool contains(const Interval& interval) const 
	  {
		  return (left_bound <= interval.left_bound && interval.right_bound <= right_bound);
	  }
	  bool intersects(const Interval& interval) const 
	  {
		  return !(left_bound >= interval.right_bound || right_bound <= interval.left_bound);
	  }
	  size_t size() const
	  {
		  return std::max(0, right_bound - left_bound);
	  }
	  int left_bound, right_bound;
};

std::ostream& operator << (std::ostream& stream, const Interval& interval)
{
	stream << "(" << interval.left_bound << ", " << interval.right_bound << ")";
	return stream;
}

#endif // INTERVAL_H_
