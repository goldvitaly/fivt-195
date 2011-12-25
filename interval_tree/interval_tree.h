#ifndef INTERVAL_TREE_H_
#define INTERVAL_TREE_H_

#include "interval.h"

#include <boost/optional.hpp>
#include <stdexcept>
#include <vector>

/*
   ElementType
   MergeElementFunc(ElementType, ElementType) -> ElementType
   ModificationType
   MergeModificationFunc(ModificationType&, ModificationType) 
   ApplyModificationFunc(ElementType&, ModificationType)
*/

template <class ElementType, class MergeElementFunc, class ModificationType, class MergeModificationFunc, class ApplyModificationFunc>
class IntervalTree
{
	public:
		size_t size() const 
		{
			return size_;
		}
		IntervalTree(const std::vector<ElementType>& v, 
			MergeElementFunc merge_element_func_, MergeModificationFunc merge_modification_func_, ApplyModificationFunc apply_modification_func_):
				size_(v.size()), merge_element_func_(merge_element_func_), 
				merge_modification_func_(merge_modification_func_), apply_modification_func_(apply_modification_func_) 
		{
			init();
			for (int i = 0; i < size_; i ++) 
				tree[leaves_begin_position_ + i].value = v[i];
			recount();
		}	
		IntervalTree(size_t size_, const ElementType& element, 
			MergeElementFunc merge_element_func_, MergeModificationFunc merge_modification_func_, ApplyModificationFunc apply_modification_func_):
				size_(size_), merge_element_func_(merge_element_func_), 
				merge_modification_func_(merge_modification_func_), apply_modification_func_(apply_modification_func_) 
		{
			init();
			for (int i = leaves_begin_position_; i < leaves_begin_position_ + leaves_number_; i ++) 
				tree[i].value = element;
			recount();
		}	
		// TODO: provide ability to use any type for requesting
		// TODO: 0- or 1- numeration? Current - 0. now using [) - intervals
		ElementType request(size_t left, size_t right)
		{
			if (!valid_range(left, right)) throw std::logic_error("invalid range");
			return request(0, Interval(0, leaves_number_), Interval(left, right));
		}
		void apply(size_t left, size_t right, const ModificationType& modification)
		{
			if (!valid_range(left,right)) throw std::logic_error("invalid range");
			apply(0, Interval(0, leaves_number_ ), Interval(left, right), modification);
		}
	private:
		bool valid_range(size_t left, size_t right) const 
		{
			return !(left > size_ || right > size_ || left >= right);
		}
		void init()
		{
			leaves_number_ = 2;
			while (leaves_number_ < size_)  // TODO: do something for avoiding overflowing
				leaves_number_ *= 2;
			tree.resize(2 * leaves_number_ - 1);
			leaves_begin_position_ = leaves_number_ - 1;
		}
		void recount()
		{
			for (int i = leaves_begin_position_ - 1; i >= 0; i --)
			{
				bool has_left_value = tree[left(i)].value;
				bool has_right_value = tree[right(i)].value;
				if (has_left_value && has_right_value)
				{
					tree[i].value = merge_element_func_(tree[left(i)].value.get(), tree[right(i)].value.get());
					continue;
				}
				if (has_left_value)
					tree[i].value = tree[left(i)].value.get();
				if (has_right_value)
					tree[i].value = tree[right(i)].value.get();
			}
		}
		size_t size_;
		MergeElementFunc merge_element_func_;
		MergeModificationFunc merge_modification_func_;
		ApplyModificationFunc apply_modification_func_;
		int leaves_number_;
		int leaves_begin_position_;
		int left(int v) const
		{
			return v * 2 + 1;
		}

		int right(int v) const
		{
			return v * 2 + 2;
		}

		class node
		{
		  public:
			boost::optional<ElementType> value;
			boost::optional<ModificationType> modification;
			node(){};
			explicit node(ElementType value): value(value)
			{
			}node(ElementType value, ModificationType modification): value(value), modification(modification)
			{
			}
		};
		void push_modification(int source, int destination)
		{
			assert(tree[source].modification);
			if (tree[destination].modification)
				merge_modification_func_(*tree[destination].modification, tree[source].modification.get()); 
			else
				tree[destination].modification = tree[source].modification;
		}
		// TODO: iterators needed. 
		bool is_valid_position(size_t position) const 
		{
			return 0 <= position && position < tree.size();
		}
		void update(int position)
		{
			if (tree[position].modification)
			{
				if (is_valid_position(left(position)))
					push_modification(position, left(position));
				if (is_valid_position(right(position)))
					push_modification(position, right(position));
				apply_modification_func_(*tree[position].value, tree[position].modification.get());
			}
			tree[position].modification.reset();
		}
		ElementType get_value(int position)
		{
			update(position);
			return tree[position].value.get();
		}
		std::vector <node> tree; // [0, leaves_begin_position_) - additional values
								 // [leaves_begin_position_, leaves_begin_position_ + leaves_number_) - initial values

		
								 // TODO: count node from current_range

		ElementType request(int node, const Interval& current_range, const Interval& request_range)
		{
			update(node);
			if (request_range.contains(current_range))
				return tree[node].value.get();
			assert(request_range.intersects(current_range));
			Interval left_range = current_range.left_half();
			Interval right_range = current_range.right_half();
			bool need_left_request = left_range.intersects(request_range) && tree[left(node)].value;
			bool need_right_request = right_range.intersects(request_range) && tree[right(node)].value;
			if (need_left_request && need_right_request)
				return merge_element_func_(request(left(node), left_range, request_range), request(right(node), right_range, request_range));
			if (need_left_request)
				return request(left(node), left_range, request_range);
			if (need_right_request)
				return request(right(node), right_range, request_range);
			assert(false); // it's impossible to return nothing
		}
		void apply(int node, const Interval& current_range, const Interval& request_range, const ModificationType& modification)
		{
			update(node);
			if (request_range.contains(current_range))
			{
				tree[node].modification = modification;
				return;
			}
			if (!tree[node].value) 
				return;
			if (!request_range.intersects(current_range))
				return;
			apply(left(node), current_range.left_half(), request_range, modification);
			apply(right(node), current_range.right_half(), request_range, modification);
			bool has_left_value = tree[left(node)].value;
			bool has_right_value = tree[right(node)].value;
			if (has_left_value && has_right_value)
			{
				tree[node].value = merge_element_func_(get_value(left(node)), get_value(right(node)));
				return;
			}
			if (has_left_value)
				tree[node].value = get_value(left(node));
			if (has_right_value)
				tree[node].value = get_value(right(node));
		}
}; 

#endif // INTERVAL_TREE_H_
