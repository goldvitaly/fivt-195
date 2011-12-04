#ifndef COLLECTION_EXT_H_INCLUDED
#define COLLECTION_EXT_H_INCLUDED

#include <string>
#include <utility>

class int_BitExt{
public:
    explicit int_BitExt(int Bits_Num = 8){Bits_Num_ = Bits_Num;}

    int extract(unsigned int elem, int poz) const{
        return (elem >> poz) & ((1<<Bits_Num_) - 1);
    }
    int size() const{return 32;}
    int max_key() const{return (1LL << Bits_Num_);}
    int blok() const{return Bits_Num_;}
private:
    int Bits_Num_;
};

class SubstrExt{
public:
    explicit SubstrExt(int len, int Bits_Num = 1){Bits_Num_ = Bits_Num; len_ = len;}

    int extract(const std::string& elem, int pos) const{
        int key = 0;
        int pow = 1;
        for(int j = len_ - pos - 1; j > len_ - pos - 1 - Bits_Num_; j--)
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
    int size() const{return len_;}
    int max_key() const{
        const int size_letter = 256; //2**(8*sizeof(char))
        int key = 1;
        for(int j = 0; j < Bits_Num_; j++) // size_letter**(Bits_Num_)
            key *= size_letter;
        return key;
    }
    int blok() const{return Bits_Num_;}
private:
    int Bits_Num_;
    int len_;
};

class long_long_BitExt{
public:
    explicit long_long_BitExt(int Bits_Num){Bits_Num_ = Bits_Num;}
    long_long_BitExt(){Bits_Num_ = 8;}

    int extract(unsigned long long elem, int poz) const{
        return (elem >> poz) & ((1<<Bits_Num_) - 1);
    }
    int size() const{return 64;}
    int blok() const{return Bits_Num_;}
    int max_key()const{return (1LL << Bits_Num_);}
private:
    int Bits_Num_;
};

class pair_BitExt{
public:
    explicit pair_BitExt(int Bits_Num){Bits_Num_ = Bits_Num;}
    pair_BitExt(){Bits_Num_ = 8;}

    int extract(const std::pair<unsigned int, unsigned int>& elem, int poz) const{
        unsigned long long elem2 = ((unsigned long long)elem.first << 32) + elem.second;
        return (elem2 >> poz) & ((1<<Bits_Num_) - 1);
    }
    int size() const{return 64;}
    int blok() const{return Bits_Num_;}
    int max_key() const{return (1LL << Bits_Num_);}
private:
    int Bits_Num_;
};

#endif // COLLECTION_EXT_H_INCLUDED
