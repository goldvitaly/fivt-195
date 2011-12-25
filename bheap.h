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
		return FindMin(head).first->key;
	}

	void PopTop()
	{
		if (head == NULL)
			throw (std::logic_error("BHeap:PopTop, Heap is empty"));

		std::pair<Node*, Node*> temp = FindMin(head);
		Node* minNode = temp.first;
		Node* minNodePrev = temp.second;

		if (minNodePrev == NULL)
			head = minNode->sibling;
		else
			minNodePrev->sibling = minNode->sibling;

		size -= 1 << minNode->degree;

		BHeap<Type> pq;
		Node* next = NULL;
		Node *cur;
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

    void Merge(BHeap<Type> &cur)
    {
        BHeapUnion(cur);
    }

private:
	Node* head;
	int size;
	std::pair<Node*, Node*> FindMin(Node* minNode) const
	{
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
		return make_pair(minNode, minNodePrev);

	}

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
		Node* prevCur = &new_head;
		Node* cur = head;
		Node* nextCur = cur->sibling;
		while (nextCur != NULL)
		{
			if (cur->degree != nextCur->degree ||
				nextCur->sibling != NULL && nextCur->sibling->degree == cur->degree)
			{
				prevCur = cur;
				cur = nextCur;
			}
			else if ( !(nextCur->key < cur->key) )
            {
				cur->sibling = nextCur->sibling;
				BHeapLink(nextCur, cur);
			}
			else
			{
				prevCur->sibling = nextCur;
				BHeapLink(cur, nextCur);
				cur = nextCur;
			}
			nextCur = cur->sibling;
		}
		head = new_head.sibling;
	}


};
