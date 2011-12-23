#include <vector>

template<class T>
struct Extractor
{
	public:
		static const unsigned digit_count;
		static const unsigned digit_range;
		int operator()(T value, unsigned digit);
};

template<typename T, typename Extractor>
void digitSort(T* begin, T* end, Extractor ext)
{
	std::vector<T> bins[Extractor::digit_range];
	for(unsigned digit = 0; digit < Extractor::digit_count; digit++)
	{
		for(unsigned bin = 0; bin < Extractor::digit_range; bin++)
			bins[bin].clear();
		for(T* iter = begin; iter < end; iter++)
			bins[ext(*iter, digit)].push_back(*iter);
		int counter = 0;
		for(unsigned bin = 0; bin < Extractor::digit_range; bin++)
			for(unsigned ind = 0; ind < bins[bin].size(); ind++)
			{
				*(begin + counter) = bins[bin][ind];
				counter++;
			}
	}
}
