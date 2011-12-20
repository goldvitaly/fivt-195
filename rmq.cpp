#include <iostream>
#include <algorithm>
#include <cassert>

template<typename ValueType,
typename ModType,
typename ModFunc,
typename MergeFunc,
typename PushFunc>
class IntervalTree
{
private:
	ValueType _ValueDefault;
	ModType _ModDefault;

	ModFunc _ModFunc;
	MergeFunc _MergeFunc;
	PushFunc _PushFunc;

	size_t _Begin, _End;
	IntervalTree *_Left, *_Right;
	
	ValueType _Value;
	ModType _Mod;
	
	void Push ()
	{
		if (_Begin + 1 != _End)
		{
			_Left->_Mod = _PushFunc(_Mod, _Left->_Mod);
			_Right->_Mod = _PushFunc(_Mod, _Right->_Mod);
			_Mod = _ModDefault;
			_Value = _MergeFunc(_ModFunc(_Left->_Mod, _Left->_Value),
								_ModFunc(_Right->_Mod, _Right->_Value));
		}
	}
public:
	IntervalTree(size_t Begin, size_t End, 
	ValueType ValueDefault, 
	ModType ModDefault
	): _Begin(Begin), _End(End)
	{		
		_ValueDefault = ValueDefault;
		_ModDefault = ModDefault;
		
		ModFunc _ModFunc = ModFunc();
		MergeFunc _MergeFunc = MergeFunc();
		PushFunc _PushFunc = PushFunc();
		
		_Value = _ValueDefault;
		_Mod = _ModDefault;
		
		_Left = _Right = NULL;
		
		if (_End > _Begin + 1)
		{
			size_t Middle = (_Begin + _End - 1) / 2 + 1;
			_Left = new IntervalTree(_Begin, Middle, ValueDefault, ModDefault);
			_Right = new IntervalTree(Middle, _End, ValueDefault, ModDefault);
		}
	}
	ValueType GetValue (size_t Begin, size_t End)
	{			
		Push();
		
		if (Begin == _Begin && End == _End)
			return _ModFunc(_Mod, _Value);
		
		if (End <= _Left->_End)
			return _Left->GetValue(Begin, End);
		else if (Begin >= _Right->_Begin)
			return _Right->GetValue(Begin, End);
		else
			return _MergeFunc(_Left->GetValue(Begin, _Left->_End), 
								_Right->GetValue(_Right->_Begin, End));
	}
	void UpdateValue (size_t Begin, size_t End, ModType Mod)
	{	
		Push();
		
		if (Begin == _Begin && End == _End)
		{
			_Mod = _PushFunc(Mod, _Mod);
			return;
		}		

		if (End <= _Left->_End)
			_Left->UpdateValue(Begin, End, Mod);
		else if (Begin >= _Right->_Begin)
			_Right->UpdateValue(Begin, End, Mod);
		else
		{
			_Left->UpdateValue(Begin, _Left->_End, Mod);
			_Right->UpdateValue(_Right->_Begin, End, Mod);
		}
		
		
		_Value = _MergeFunc(_ModFunc(_Left->_Mod, _Left->_Value),
							_ModFunc(_Right->_Mod, _Right->_Value));
	}
};

class PLUS 
{
public:
	int operator () (const int a, const int b)
	{
		return a + b;
	}
};

class MAX 
{
public:
	int operator () (const int a, const int b)
	{
		return std::max(a,b);
	}
};

class IntervalTreeChecker
{
private:
	int *_a;
	size_t _Size;
public:
	explicit IntervalTreeChecker (size_t Size)
	{
		_Size = Size;
		_a = new int[_Size];
		for (size_t i = 0; i < _Size; ++i)
			_a[i] = 0;
	}
	~IntervalTreeChecker ()
	{
		delete [] _a;
	}
	void UpdateValue (size_t Begin, size_t End, int Value)
	{
		for (size_t i = Begin; i < End; ++i)
			_a[i] += Value;
	}
	int GetValue (size_t Begin, size_t End)
	{
		int Max = _a[Begin];
		for (size_t i = Begin; i < End; ++i)
			Max = std::max(Max, _a[i]);
		return Max;
	}
};

int main ()
{
	const size_t TestNum = 100000, TestSize = 1000;
	
	IntervalTree<int, int, PLUS, MAX, PLUS > r(0, TestSize, int(NULL), int(NULL));
	IntervalTreeChecker InChk(TestSize);
	for (size_t t = 0; t < TestNum; ++t)
	{
		int a = std::rand() % TestSize;
		int b = a + std::rand() % (TestSize - a) + 1;
		assert(a < b);
		assert(b <= TestSize);
		assert(a >= 0);
		
		int x = std::rand() % TestSize - (TestSize / 2);
		if (std::rand() % 2 == 0)
		{
			r.UpdateValue(a, b, x);
			InChk.UpdateValue(a, b, x);
		}
		else
		{
			int resA, resB;
			resA = r.GetValue(a, b);
			resB = InChk.GetValue(a, b);
			assert(resA == resB);
		}
	}
	return 0;
}
