#ifndef FUNCTORS_H_
#define FUNCTORS_H_

namespace functors
{
	template <class T, class Comparator = std::less<T> >
	class min
	{		
		Comparator comparator;
	  public:
		explicit min(const Comparator& comparator): comparator(comparator)
		{

		}
		T operator()(const T& a, const T& b) const
		{
			if (comparator(a,b))
				return a;
			else
				return b;
		}
	};

	template <class T>
	class assign
	{
	  public:
		void operator()(T& a, const T& b) const
		{
			a = b;
		}
	};

	template <class T, class BinaryOperation = std::plus<T> > 
	class plus_equal
	{
      private:
		BinaryOperation binary_operation;
	  public:
		explicit plus_equal(const BinaryOperation& binary_operation = BinaryOperation()): binary_operation(binary_operation) {};
		void operator()(T&a, const T& b) const
		{
			a = binary_operation(a,b);
		}
	};
}


#endif // FUNCTORS_H_
