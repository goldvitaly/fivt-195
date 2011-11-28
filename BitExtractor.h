#ifndef BITEXTRACTOR_H_INCLUDED
#define BITEXTRACTOR_H_INCLUDED

#include <vector>
#include <string>
#include <utility>
using namespace std;



class IntBitExtractor
{
    public:
        IntBitExtractor(int BitsNum)
        {
            BitsNum_ = BitsNum;
            BlocksNum_ = 32 / BitsNum;
        }

        int Extract(vector <int>::iterator x, int p);

        int GetCount()
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


int IntBitExtractor::Extract(vector <int>::iterator x, int p)
{
    return(((*x) >> (p * BitsNum_) & ((1 << BitsNum_) - 1)));
}

class StringBitExtrator
{
    public:
        StringBitExtrator(size_t BlocksNum = 5)
        {
            BitsNum_ = 8;
            BlocksNum_ = BlocksNum;
        }

        int Extract(vector<string>::iterator x, int p);

        int GetCount()
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



int StringBitExtrator::Extract(vector<string>::iterator x, int p)
{
    return int((*x)[BlocksNum_ - 1 - p]);
}



class PairBitExtractor
{
    public:
        PairBitExtractor (int BitsNum)
        {
            BitsNum_ = BitsNum;
            BlocksNum_ = 64 / BitsNum_;
        }

        int Extract(vector < pair<int, int> >::iterator x, int p);

        int GetCount()
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


int PairBitExtractor::Extract(vector< pair<int, int> >::iterator x, int p)
{
    if(p >= BlocksNum_ / 2)
    {
        p -= BlocksNum_;
        return((((*x).first) >> (p * BitsNum_)) & ((1 << BitsNum_) - 1));
    }
    return((((*x).second) >> (p * BitsNum_)) & ((1 << BitsNum_) - 1));
}


class LongLongBitExtractor
{
    public:
        LongLongBitExtractor (int BitsNum)
        {
            BitsNum_ = BitsNum;
            BlocksNum_ = 64 / BitsNum_;
        }

        int Extract(vector <long long>::iterator x, int p);

        int GetCount()
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

int LongLongBitExtractor::Extract(vector<long long>::iterator x, int p)
{
    return (((*x) >> (BitsNum_ * p)) & ((1LL << BitsNum_) - 1));
}
#endif // BITEXTRACTOR_H_INCLUDED
