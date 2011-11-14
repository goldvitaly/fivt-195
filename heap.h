/* 
 * File:   heap.h
 * Author: riad
 *
 * Created on October 31, 2011, 1:19 PM
 */

template<typename KeyType, typename ValueType, typename Comp = std::less<KeyType> >
		  class LeftHeap {
	Comp comp_;

	struct Node {
		KeyType key;
		ValueType value;

		Node * l, *r;
		int height;

		Node(const KeyType& k, const ValueType & v) : key(k), value(v), l(0), r(0), height(0) {
		}

		~Node() {
			delete l;
			delete r;
		}
	};
	friend void validate(Node* node);
	friend void StressTest();

	Node* destroyAndMerge(Node* left, Node* right) {

		if (!left)
			return right;
		if (!right)
			return left;
		if (comp_(right->key, left->key))
			std::swap(left, right);
		left->r = destroyAndMerge(left->r, right);
		// if left->l not null, then left->r not null too. It's checked in test
		if ((!left->l) || left->l->height < left->r->height)
			std::swap(left->l, left->r);
		left->height = left->r ? left->r->height + 1 : 0;
		return left;
	}
	Node* root_;
	size_t size_;


public:

	class NoElements : public std::exception {
	};

	explicit LeftHeap(Comp comp = Comp()) : root_(0), size_(0) {
		comp_ = comp;
	}

	bool empty() const {
		return size_ == 0;
	}

	size_t size() const {
		return size_;
	}

	/**
	 * @throws NoElements
	 */
	ValueType top() const {
		if (empty())
			throw NoElements();
		return root_->value;
	}

	ValueType topKey() const{
		if(empty())
			throw NoElements();
		return root_->key;
	}

	/**
	 * @throws NoElements
	 */
	void pop() {
		if (empty())
			throw NoElements();
		--size_;
		Node *oldroot = root_;
		root_ = destroyAndMerge(root_->l, root_->r);
		//не удаляем детей этой вершины.
		oldroot->l = 0;
		oldroot->r = 0;
		delete oldroot;
	}

	void push(const KeyType& k, const ValueType& v) {
		++size_;
		root_ = destroyAndMerge(root_, new Node(k, v));
	}

	~LeftHeap() {
		delete root_;
	}
};