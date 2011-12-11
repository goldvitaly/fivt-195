#include <cstdlib>
#include <iostream>
#include <boost/optional.hpp>
#include <cassert>

using namespace std;

template <class T, class Op>
class IntervalTree
{
	public:
		IntervalTree(size_t size);
		~IntervalTree();
		
		void print(std::ostream& stream);
		void assign(const T& value, int index);
		T query(int left_index, int right_index);
	private:
		boost::optional<T>* tree;
		size_t data_size;
		boost::optional<T> query(int left_index, int right_index, int v_index, int v_left, int v_right);
		template <class Elem>
		struct OptionalOp{
			boost::optional<Elem> operator()(boost::optional<Elem> a, boost::optional<Elem> b)
			{
				if(!a)
					return b;
				if(!b)
					return a;
				return Op()(*a,*b);
			}
		};
};

template <class T, class Op>
IntervalTree<T, Op>::IntervalTree(size_t size)
{
	data_size = 1;
	while(data_size < size)
		data_size <<= 1;
	tree = new boost::optional<T>[(data_size << 1) - 1];
}

template <class T, class Op>
IntervalTree<T, Op>::~IntervalTree()
{
	delete[] tree;
}

template <class T, class Op>
void IntervalTree<T, Op>::print(std::ostream& stream)
{
	stream << "Printing tree" << std::endl;
	for(unsigned int i = 0; i < (data_size << 1) - 1; i++)
	{
		if(tree[i])
			stream << *tree[i] << " ";
		else
			stream << "x ";
		if(!((i + 1) & (i + 2)))
		{
			stream << std::endl;
		}
	}
}

template <class T, class Op>
void IntervalTree<T, Op>::assign(const T& value, int index)
{
	int check_index = index + data_size - 1;
	tree[check_index] = value;
	int parent_index;
	boost::optional<T> op_result;
	while(check_index > 0)
	{
		parent_index = (check_index - 1) / 2;
		op_result = OptionalOp<T>()(tree[parent_index * 2 + 1], tree[parent_index * 2 + 2]);
		if(op_result != tree[parent_index])
		{
			tree[parent_index] = op_result;
			check_index = parent_index;
		}
		else
			break;
	}
}

template <class T, class Op>
T IntervalTree<T, Op>::query(int left_index, int right_index)
{
	boost::optional<T> ret = query(data_size + left_index - 1, data_size + right_index - 1, 0, data_size - 1, data_size * 2 - 2);
	assert(ret != NULL);
	return *ret;
}

template <class T, class Op>
boost::optional<T> IntervalTree<T, Op>::query(int left_index, int right_index, int v_index, int v_left, int v_right)
{
	assert(left_index <= right_index);
	if(v_right < left_index || v_left > right_index)
	{
		return boost::optional<T>();
	}
	if(v_left >= left_index && v_right <= right_index)
	{
		return tree[v_index];
	}
	boost::optional<T> left_query = query(left_index, right_index, v_index * 2 + 1, v_left, (v_left + v_right) / 2);
	boost::optional<T> right_query = query(left_index, right_index, v_index * 2 + 2, (v_left + v_right) / 2 + 1, v_right);
	return OptionalOp<T>()(left_query, right_query);
}
