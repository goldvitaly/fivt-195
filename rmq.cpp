#include <iostream>
#include <algorithm>

using namespace std;

template<typename ValueType,
typename ModType,
typename ModFunc,
typename MergeFunc,
typename PushFunc>
class IntervalTree
{
private:
	static ValueType _ValueDefault;
	static ModType _ModDefault;

	static ModFunc _ModFunc;
	static MergeFunc _MergeFunc;
	static PushFunc _PushFunc;

	/*ValueType _ValueDefault;
	ModType _ModDefault;

	ModFunc _ModFunc;
	MergeFunc _MergeFunc;
	PushFunc _PushFunc;*/

	size_t _Begin, _End;
	IntervalTree *_Left, *_Right;
	
	ValueType _Value;
	ModType _Mod;
	
	IntervalTree(size_t Begin, size_t End): _Begin(Begin), _End(End)
	{		
		_Value = _ValueDefault;
		_Mod = _ModDefault;
		
		_Left = _Right = NULL;
		
		if (_End > _Begin + 1)
		{
			_Left = new IntervalTree(_Begin, (_Begin + _End - 1) / 2 + 1);
			_Right = new IntervalTree((_Begin + _End - 1) / 2 + 1, _End);
		}
	}
	void Push ()
	{
		if (_Begin + 1 != _End)
		{
			_Left->_Mod = _PushFunc(_Mod, _Left->_Mod);
			_Right->_Mod = _PushFunc(_Mod, _Right->_Mod);
			_Mod = _ModDefault;
		}
	}
public:
	IntervalTree(size_t Size, 
	ValueType ValueDefault, 
	ModType ModDefault
	): _Begin(0), _End(Size)
	{
		ValueType _ValueDefault(ValueDefault);// = ValueType();
		ModType _ModDefault(ModDefault);// = ModType();
		
		_ValueDefault = ValueDefault;
		_ModDefault = ModDefault;
		
		ModFunc _ModFunc = ModFunc();
		MergeFunc _MergeFunc = MergeFunc();
		PushFunc _PushFunc = PushFunc();
		
		/*_Value = _ValueDefault;
		_Mod = _ModDefault;
		
		_Left = _Right = NULL;
		
		if (_End > _Begin + 1)
		{
			_Left = new IntervalTree(_Begin, (_Begin + _End - 1) / 2 + 1);
			_Right = new IntervalTree((_Begin + _End - 1) / 2 + 1, _End);
		}*/
		IntervalTree(0, Size);
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
		
		_Value = _MergeFunc(_ModFunc(_Left->_Mod, _Left->Value), _ModFunc(_Right->_Mod, _Right->Value));
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

//typedef IntervalTree<int, int, std::plus<int>, std::plus<int>, std::plus<int> > pt;

int main ()
{
	 IntervalTree<int, int, PLUS, PLUS, PLUS > r(int(100), int(NULL), int(NULL));
	
	return 0;
}
