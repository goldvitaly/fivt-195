
#ifndef RSQ_ASSIGN_H_
#define RSQ_ASSIGN_H_

#include <stdexcept>

#include "interval_tree.h"
#include "functors.h"


template <class T, class BinaryOperation = std::less<T> > 
class RSQAssignModification
{
  private:
	struct ValueType
	{
		T element;
		int left_bound;
		int right_bound;
	};
	class merge_value
	{
	   public:
		ValueType operator()(const ValueType& first, const ValueType& second)
		{
			ValueType res;
			res.left_bound = first.left_bound;
			res.right_bound = second.right_bound;
			res.element = first.element + second.element;
			return res;
		}
	};
	class apply_modication
	{
	  public:
		void operator ()(ValueType& v, const T& modication)
		{
			v.element = (v.right_bound - v.left_bound + 1) * modication;
		}
	};
	std::vector<ValueType> generate_tmp_array(size_t size, const T& value)
	{
		std::vector<ValueType> result(size);
		for (int i = 0; i < size; i ++)
		{
			result[i].left_bound = result[i].right_bound = i;
			result[i].element = value;
		}
		return result;
	}
  public:
	BinaryOperation binary_operation;
	explicit RSQAssignModification(int size_, const T& initial_value = T(), BinaryOperation binary_operation = BinaryOperation()): 
		binary_operation(binary_operation), size_(size_),
		tree(generate_tmp_array(size_, initial_value), merge_value(), functors::assign<T>(), apply_modication())
	{
	}
	void apply(int l, int r, const T& modification)
	{
		tree.apply(l, r, modification);
	}
	T request(int l, int r)
	{
		return tree.request(l, r).element;
	}
	int size() const { return size_; };
  private:
	int size_;
	IntervalTree <ValueType, merge_value, T, functors::assign<T>, apply_modication> tree;
};

#endif // RSQ_ASSIGN_H_
