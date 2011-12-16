#include <cstdlib>
#include <iostream>
#include <boost/optional.hpp>
#include <cassert>

template <typename Elem, typename Operation>
boost::optional<Elem> OptionalOperation(boost::optional<Elem> a, boost::optional<Elem> b, Operation op)
{
	if(!a)
		return b;
	if(!b)
		return a;
	return op(*a,*b);
}


template <class T, class Op, class ModOp, class ModUpd>
class IntervalTree
{
	public:
		IntervalTree(size_t size, Op oper, ModOp mod_oper, ModUpd mod_update);
		~IntervalTree();
		
		void print(std::ostream& stream) const;
		void assign(const T& value, int index);
		T query(int left_index, int right_index);
		void modify(int left_index, int rigth_index, const T& modification);
	private:
		Op operation;
		ModOp modoperation;
		ModUpd modupdate;
		boost::optional<T>* tree;
		boost::optional<T>* mods;
		size_t data_size;
		
		boost::optional<T> query(int left_index, int right_index, int v_index, int v_left, int v_right);
		void modify(int left_index, int right_index, int v_index, int v_left, int v_right, const T& modification);
		void update_node(int v_index, int v_left, int v_right);
		
};

template <class T, class Op, class ModOp, class ModUpd>
IntervalTree<T, Op, ModOp, ModUpd>::IntervalTree(size_t size, Op oper, ModOp mod_oper, ModUpd mod_update)
{
	data_size = 1;
	while(data_size < size)
		data_size <<= 1;
	tree = new boost::optional<T>[(data_size << 1) - 1];
	mods = new boost::optional<T>[(data_size << 1) - 1];
}

template <class T, class Op, class ModOp, class ModUpd>
IntervalTree<T, Op, ModOp, ModUpd>::~IntervalTree()
{
	delete[] tree;
}

template <class T, class Op, class ModOp, class ModUpd>
void IntervalTree<T, Op, ModOp, ModUpd>::print(std::ostream& stream) const
{
	stream << "Printing tree" << std::endl;
	for(unsigned int i = 0; i < (data_size << 1) - 1; i++)
	{
		if(tree[i])
			stream << *tree[i] << " ";
		else
			stream << "x ";
		stream << "(";
		if(mods[i])
			stream << *mods[i] << ") ";
		else
			stream << "x) ";
		
		
		if(!((i + 1) & (i + 2)))
			stream << std::endl;
	}
}

template <class T, class Op, class ModOp, class ModUpd>
void IntervalTree<T, Op, ModOp, ModUpd>::assign(const T& value, int index)
{
	int check_index = index + data_size - 1;
	tree[check_index] = value;
	
	int parent_index;
	boost::optional<T> op_result;
	while(check_index > 0)
	{
		parent_index = (check_index - 1) / 2;
		op_result = OptionalOperation<T, Op>(tree[(parent_index << 1) + 1], tree[(parent_index << 1) + 2]);
		if(op_result != tree[parent_index])
		{
			tree[parent_index] = op_result;
			check_index = parent_index;
		}
		else
			break;
	}
}

template <class T, class Op, class ModOp, class ModUpd>
T IntervalTree<T, Op, ModOp, ModUpd>::query(int left_index, int right_index)
{
	boost::optional<T> ret = query(data_size + left_index - 1, data_size + right_index - 1, 0, data_size - 1, (data_size << 1) - 2);
	assert(ret != NULL);
	return *ret;
}

template <class T, class Op, class ModOp, class ModUpd>
boost::optional<T> IntervalTree<T, Op, ModOp, ModUpd>::query(int left_index, int right_index, int v_index, int v_left, int v_right)
{
	assert(left_index <= right_index);
	
	if(v_right < left_index || v_left > right_index)
		return boost::optional<T>();
	if(v_left >= left_index && v_right <= right_index)
		return tree[v_index];
	
	update_node(v_index, v_left, v_right);
	boost::optional<T> left_query = query(left_index, right_index, (v_index << 1) + 1, v_left, ((v_left + v_right) >> 1));
	boost::optional<T> right_query = query(left_index, right_index, (v_index << 1) + 2, ((v_left + v_right) >> 1) + 1, v_right);
	return OptionalOperation<T, Op>(left_query, right_query);
}

template <class T, class Op, class ModOp, class ModUpd>
void IntervalTree<T, Op, ModOp, ModUpd>::modify(int left_index, int right_index, int v_index, int v_left, int v_right, const T& modification)
{
	assert(left_index <= right_index);
	
	if(v_right < left_index || v_left > right_index)
		return;
	if(v_left >= left_index && v_right <= right_index)
	{
		if(mods[v_index])
			mods[v_index] = OptionalOperation<T, ModUpd>(mods[v_index], modification);//TODO!!!
		else
			mods[v_index] = modification;
	}
	if(v_left >= left_index && v_right <= right_index)
		return;
	
	modify(left_index, right_index, (v_index << 1) + 1, v_left, ((v_left + v_right) >> 1), modification);
	modify(left_index, right_index, (v_index << 1) + 2, ((v_left + v_right) >> 1) + 1, v_right, modification);	
}

template <class T, class Op, class ModOp, class ModUpd>
void IntervalTree<T, Op, ModOp, ModUpd>::modify(int left_index, int right_index, const T& modification)
{
	modify(data_size + left_index - 1, data_size + right_index - 1, 0, data_size - 1, (data_size << 1) - 2, modification);
}

template <class T, class Op, class ModOp, class ModUpd>
void IntervalTree<T, Op, ModOp, ModUpd>::update_node(int v_index, int v_left, int v_right)
{
	if(!mods[v_index])
		return;
	//std::cerr << "Updating.." << *tree[v_index] << std::endl;
	tree[v_index] = ModOp(*tree[v_index],*mods[v_index], v_left, v_right);
	//std::cerr << "Updated" << *tree[v_index] << std::endl;
	if(v_index * 2 + 1 < (data_size << 1) - 1)
	{
		mods[v_index * 2 + 1] = OptionalOperation<T, ModUpd>(mods[v_index * 2 + 1], mods[v_index]);
		mods[v_index * 2 + 2] = OptionalOperation<T, ModUpd>(mods[v_index * 2 + 2], mods[v_index]);
	}
	mods[v_index] = boost::optional<T>();
}
