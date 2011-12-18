#include <iostream>
#include <algorithm>

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
			_Value = _MergeFunc(_ModFunc(_Left->_Mod, _Left->_Value), _ModFunc(_Right->_Mod, _Right->_Value));
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
			_Left = new IntervalTree(_Begin, (_Begin + _End - 1) / 2 + 1, ValueDefault, ModDefault);
			_Right = new IntervalTree((_Begin + _End - 1) / 2 + 1, _End, ValueDefault, ModDefault);
		}
	}
	ValueType GetValue (size_t Begin, size_t End)
	{
		if (Begin < _Begin || End > _End)
			;//assert(0);
			
		Push();
		
		if (Begin == _Begin && End == _End)
			return _ModFunc(_Mod, _Value);
		
		if (End <= _Left->_End)
			return _Left->GetValue(Begin, End);
		else if (Begin >= _Right->_Begin)
			return _Right->GetValue(Begin, End);
		else
			return _MergeFunc(_Left->GetValue(Begin, _Left->_End), _Right->GetValue(_Right->_Begin, _End));
	}
	void UpdateValue (size_t Begin, size_t End, ModType Mod)
	{
		if (Begin < _Begin || End > _End)
			;//assert(0);
		
		Push();
		
		if (Begin == _Begin && End == _End)
		{
			_Mod = _PushFunc(Mod, _Mod);
			return;
		}		
		
		if (End <= _Left->_End)
			_Left->UpdateValue(Begin, End, Mod);
		else if (Begin >= _Right->_Begin)
			return _Right->UpdateValue(Begin, End, Mod);
		else
		{
			_Left->UpdateValue(Begin, _Left->_End, Mod);
			_Right->UpdateValue(_Right->_Begin, _End, Mod);
		}
		
		_Value = _MergeFunc(_ModFunc(_Left->_Mod, _Left->_Value), _ModFunc(_Right->_Mod, _Right->_Value));
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

int main ()
{
	IntervalTree<int, int, PLUS, MAX, PLUS > r(0, 4, int(NULL), int(NULL));
	r.UpdateValue(1, 4, 1);
	r.UpdateValue(3, 4, -1);
	std::cout << r.GetValue(0, 3);
	return 0;
}
