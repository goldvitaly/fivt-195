#include <stdexcept>

template <class Type>
class BHeap
{
public:
	BHeap()
	{
		head = NULL;
		size = 0;
	}

	~BHeap()
	{
		if (head != NULL)
			ClearTree(head);
	}

	void Push(const Type& key)
	{
		BHeap<Type> pq;
		Node* node = new Node;
		node->key = key;
		node->degree = 0;
		node->child = node->parent = node->sibling = NULL;
		pq.head = node;
		pq.size = 1;
		BHeapUnion(pq);
	}

	const Type& Top() const
	{
		if (head == NULL)
			throw (std::logic_error("BHeap:Top, Heap is empty"));
		Node* minNode = head;
		for (Node* cur = head->sibling; cur != NULL; cur = cur->sibling)
			if (cur->key < minNode->key)
				minNode = cur;
		return minNode->key;
	}

	void PopTop()
	{
		if (head == NULL)
			throw (std::logic_error("BHeap:PopTop, Heap is empty"));

		Node* minNode = head;
		Node* minNodePrev = NULL;
		Node* prev = head;
		Node* cur = NULL;
		for (cur = head->sibling; cur != NULL; cur = cur->sibling)
		{
			if (cur->key < minNode->key)
			{
				minNode = cur;
				minNodePrev = prev;
			}
			prev = cur;
		}

		if (minNodePrev == NULL)
			head = minNode->sibling;
		else
			minNodePrev->sibling = minNode->sibling;

		size -= 1 << minNode->degree;

		BHeap<Type> pq;
		Node* next = NULL;
		for (cur = minNode->child; cur != NULL; cur = next)
		{
			next = cur->sibling;
			cur->sibling = pq.head;
			cur->parent = NULL;
			pq.head = cur;
		}

		pq.size = (1 << minNode->degree) - 1;
		delete minNode;
		BHeapUnion(pq);
	}

	int Size() const
	{
		return size;
	}

	struct Node
	{
		Type key;
		int degree;
		Node* parent;
		Node* child;
		Node* sibling;
	};

    void Add(BHeap<Type> &cur)
    {
        BHeapUnion(cur);
    }

private:
	Node* head;
	int size;

	void ClearTree(Node* node)
	{
		Node* next;
		for (Node* cur = node->child; cur != NULL; cur = next)
		{
			next = cur->sibling;
			ClearTree(cur);
		}
		delete node;
	}

	void BHeapLink(Node* a, Node* b)
	{
		a->parent = b;
		a->sibling = b->child;
		b->child = a;
		b->degree++;
	}
	Node& BHeapMerge(Node *cur1, Node *cur2)
	{
	    static Node beg;
		beg.sibling = NULL;
		Node* prev = &beg;
	    while (cur1 != NULL && cur2 != NULL)
		{
			if (cur1->degree <= cur2->degree)
			{
				prev->sibling = cur1;
				cur1 = cur1->sibling;
			}
			else
			{
				prev->sibling = cur2;
				cur2 = cur2->sibling;
			}
			prev = prev->sibling;
		}
		while (cur1 != NULL)
		{
			prev->sibling = cur1;
			cur1 = cur1->sibling;
			prev = prev->sibling;
		}
		while (cur2 != NULL)
		{
			prev->sibling = cur2;
            cur2 = cur2->sibling;
			prev = prev->sibling;
		}
		return beg;
	}
	void BHeapUnion(BHeap<Type>& pq)
	{
	    Node &new_head = BHeapMerge(head, pq.head);

		head = new_head.sibling;
		size += pq.size;
		pq.head = NULL;
		pq.size = 0;

		if (head == NULL)
			return;
		Node* prevX = &new_head;
		Node* x = head;
		Node* nextX = x->sibling;
		while (nextX != NULL)
		{
			if (x->degree != nextX->degree ||
				nextX->sibling != NULL && nextX->sibling->degree == x->degree)
			{
				prevX = x;
				x = nextX;
			}
			else if ( !(nextX->key < x->key) )
            {
				x->sibling = nextX->sibling;
				BHeapLink(nextX, x);
			}
			else
			{
				prevX->sibling = nextX;
				BHeapLink(x, nextX);
				x = nextX;
			}
			nextX = x->sibling;
		}
		head = new_head.sibling;
	}

	//BHeap(const BHeap&) {}

};
