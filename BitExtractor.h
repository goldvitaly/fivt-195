#ifndef BITEXTRACTOR_H_INCLUDED
#define BITEXTRACTOR_H_INCLUDED

#include <vector>
#include <string>
#include <utility>


template <class T>
class IntBitExtractor
{
    public:
        explicit IntBitExtractor(int BitsNum)
        {
            BitsNum_ = BitsNum;
            BlocksNum_ = sizeof(T) * 8 / BitsNum;
        }

        T Extract(T x, int p);

        int NumberOfBlockValues()
        {
            return(1 << BitsNum_);
        }

        int GetBlocksNum()
        {
            return BlocksNum_;
        }

    private:
        int BitsNum_, BlocksNum_;
};

template <class T>
T IntBitExtractor<T>::Extract(T x, int p)
{
    return(x >> (p * BitsNum_) & ((1 << BitsNum_) - 1));
}

class StringBitExtrator
{
    public:
        explicit StringBitExtrator(size_t BlocksNum = 5)
        {
            BitsNum_ = 8;
            BlocksNum_ = BlocksNum;
        }

        int Extract(std::string x, int p);

        int NumberOfBlockValues()
        {
            return (1 << BitsNum_);
        }

        int GetBlocksNum()
        {
            return BlocksNum_;
        }


    private:
        int BitsNum_;
        size_t BlocksNum_;
};



int StringBitExtrator::Extract(std::string x, int p)
{
    return int(x[BlocksNum_ - 1 - p]);
}



class PairBitExtractor
{
    public:
        explicit PairBitExtractor (int BitsNum)
        {
            BitsNum_ = BitsNum;
            BlocksNum_ = 64 / BitsNum_;
        }

        int Extract(std::pair<int, int> x, int p);

        int NumberOfBlockValues()
        {
            return (1 << BitsNum_);
        }

        int GetBlocksNum()
        {
            return BlocksNum_;
        }

    private:
        int BitsNum_, BlocksNum_;
};


int PairBitExtractor::Extract(std::pair<int, int> x, int p)
{
    if(p >= BlocksNum_ / 2)
    {
        p -= BlocksNum_;
        return(((x.first) >> (p * BitsNum_)) & ((1 << BitsNum_) - 1));
    }
    return(((x.second) >> (p * BitsNum_)) & ((1 << BitsNum_) - 1));
}
#endif // BITEXTRACTOR_H_INCLUDED
