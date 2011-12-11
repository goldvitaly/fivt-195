
template <class T>
class IntervalTree
{
	public:
		IntervalTree(int size);
		~IntervalTree();
	private:
		T* data;
};

template <class T>
IntervalTree<T>::IntervalTree(int size)
{
	data = new T[size];
}

template <class T>
IntervalTree<T>::~IntervalTree()
{
	delete data;
}

