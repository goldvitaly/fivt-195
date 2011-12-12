#ifndef COLLECTION_EXT_H_INCLUDED
#define COLLECTION_EXT_H_INCLUDED

#include <string>
#include <utility>

template<class T>
class IntegerBitExt{
public:
    explicit IntegerBitExt(int bitsNum = 8){bitsNum_ = bitsNum;}
    T extract(const T& elem, int pos) const
    {
        return (elem >> pos) & ((1 << bitsNum_) - 1);
    }
    size_t size() const{return sizeof(T)*8;}
    size_t max_key() const{return (1LL << bitsNum_);}
    int blok() const{return bitsNum_;}
private:
    int bitsNum_;
};

class SubStrExt{
public:
    explicit SubStrExt(int len, int bitsNum = 1)
    {
            bitsNum_ = bitsNum;
            len_ = len;
    }
    int extract(const std::string& elem, int pos) const
    {
        int key = 0;
        int pow = 1;
        int upperBound = len_ - pos - 1;
        int lowerBound = upperBound - bitsNum_;
        for(int j = upperBound; j > lowerBound; j--)
        {
            if(j >= 0)
            {
                key += elem[j]*pow;
                pow *= 256;
            }
            else
                key += 0;
        }
        return key;
    }
    size_t size() const{return len_;}
    int max_key() const{
        const int sizeLetter = 256; //2**(8*sizeof(char))
        int key = 1;
        for(int j = 0; j < bitsNum_; j++) // size_letter**(Bits_Num_)
            key *= sizeLetter;
        return key;
    }
    int blok() const{return bitsNum_;}
private:
    int bitsNum_;
    int len_;
};

class PairBitExt{
public:
    explicit PairBitExt(int bitsNum = 8)
    {
        bitsNum_ = bitsNum;
    }
    int extract(const std::pair<unsigned int, unsigned int>& elem, int pos) const{
        unsigned long long elem2 = ((unsigned long long)elem.first << 32) + elem.second;
        return (elem2 >> pos) & ((1 << bitsNum_) - 1);
    }
    size_t size() const{return 64;}
    int blok() const{return bitsNum_;}
    int max_key() const{return (1LL << bitsNum_);}
private:
    int bitsNum_;
};

#endif // COLLECTION_EXT_H_INCLUDED
