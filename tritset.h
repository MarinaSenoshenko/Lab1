#ifndef TRITSET_H_
#define TRITSET_H_
#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>

using uint = unsigned int;
using namespace std;


/*enum -  enumeration type*/
enum Trit : uint { False = 1, Unknown = 0, True = 2 };
Trit operator&(Trit trit1, Trit trit2);
Trit operator~(Trit trit);
Trit operator|(Trit trit1, Trit trit2);
ostream& operator<<(ostream& out, Trit trit);

/*size of trit, which we can get*/
const uint kTritBitesSize = 2;
/*we count the number of trits in the array type uint*/
const uint kTritsInUint = sizeof(uint) * CHAR_BIT / kTritBitesSize;

class TritSet {
    class ProxyTrit {
    private:
        TritSet& set;
        uint cur_uint;
        Trit trit_value;
        uint trit_ind;

    public:
        /*constructor*/
        ProxyTrit(TritSet& set, uint ind_of_uint_with_trit, uint trit_ind);
        ProxyTrit(Trit trit);
        Trit GetTritValue() {
            return trit_value;
        }
        operator Trit() {
            return trit_value;
        }
        void operator=(Trit new_trit_value);
        void operator=(ProxyTrit new_trit);
    };

private:
    uint size{};
    uint* set;
    vector<uint> trits_array_;
    uint array_size_ = 0;
    uint GetUintCountFromTritsCount(const uint trits_count) const;
    uint GetTritIndInUint(uint trit_ind) const;
    uint GetUintIndFromTritInd(const uint trit_ind) const;
    Trit GetTritValue(const uint trit_ind) const;
    uint GetUintIndWithLastTrit() const;
    uint GetLastSettedTritIndInUint(uint uint_with_trits) const;
    uint PutTritToIndInUint(uint trit, uint trit_ind_in_uint, uint uint_to_change);
    void TrimUintAfterTritInd(uint trit_ind);
    void SetTritValue(uint trit_ind, Trit new_value);

public:
    explicit TritSet(const uint size);
    TritSet(const TritSet& set);
    uint GetSize() const;
    uint GetCountOfTritsWithType(Trit type) const;
    unordered_map<Trit, uint> Cardinality();
    uint GetLastSettedTritInd();
    void Resize(const uint new_size_in_trits);
    void Shrink();
    void Trim(uint trit_ind);
    TritSet operator&(TritSet& set);
    TritSet operator|(TritSet& set);
    TritSet operator~();
    friend ostream& operator<<(ostream& out, TritSet::ProxyTrit proxy_trit);
    ProxyTrit operator[](const uint trit_ind);
    Trit operator[](const uint trit_ind) const;
    uint Capacity() const;

    class Iterator {
    private:
        TritSet* set_iterator;
        size_t index_iterator;
    public:
        explicit Iterator(TritSet* my_set, size_t index);
        Iterator operator++ ();
        Iterator operator-- ();
        bool operator== (const Iterator& it) const;
        bool operator!= (const Iterator& it) const;

        ProxyTrit operator* (); //dereference
    };

    Iterator begin();
    Iterator end();

};

Trit operator&(Trit trit1, Trit trit2);
#endif  // TRITSET_H_
