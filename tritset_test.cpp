#include "gtest/gtest.h"
#include "tritset.h"

TEST(TritSetTest, GetSize) {
    TritSet set(10);
    EXPECT_EQ(16, set.GetSize());
    set.Resize(17);
    EXPECT_EQ(32, set.GetSize());
}

TEST(TritSetTest, OverridedOperatorsEqAndTake) {
    TritSet set(10);
    EXPECT_EQ(Unknown, set[0].GetTritValue());
    set[0] = True;
    EXPECT_EQ(True, set[0].GetTritValue());
    set[0] = Unknown;
    EXPECT_EQ(Unknown, set[0].GetTritValue());
    set[0] = False;
    EXPECT_EQ(False, set[0].GetTritValue());
    set[1] = set[0];
    EXPECT_EQ(False, set[1].GetTritValue());
    set[1] = set[2];
    EXPECT_EQ(Unknown, set[1].GetTritValue());
    uint size_int_trits = set.GetSize();
    set[10000] = Unknown;
    EXPECT_EQ(size_int_trits, set.GetSize());
    set[10000] = set[3];
    EXPECT_EQ(size_int_trits, set.GetSize());
    set[100] = False;
    EXPECT_LT(size_int_trits, set.GetSize());
    size_int_trits = set.GetSize();
    set[100000] = set[100];
    EXPECT_LT(size_int_trits, set.GetSize());
}

TEST(TritSetTest, OverridedOperatorLogic) {
    TritSet set(10);
    EXPECT_EQ(Unknown, set[1].GetTritValue());
    set[1] = False & True;
    EXPECT_EQ(False, set[1].GetTritValue());
    set[1] = True & True;
    EXPECT_EQ(True, set[1].GetTritValue());
    set[1] = Unknown & True;
    EXPECT_EQ(Unknown, set[1].GetTritValue());
    set[1] = False & Unknown;
    EXPECT_EQ(False, set[1].GetTritValue());
    set[1] = False | Unknown;
    EXPECT_EQ(Unknown, set[1].GetTritValue());
    set[1] = Unknown | True;
    EXPECT_EQ(True, set[1].GetTritValue());
    set[1] = ~set[1];
    EXPECT_EQ(False, set[1].GetTritValue());
    set[1] = ~set[1];
    EXPECT_EQ(True, set[1].GetTritValue());
    set[1] = set[1] & set[0];
    EXPECT_EQ(Unknown, set[1].GetTritValue());
    set[20] = True;
    set[10] = Unknown;
    set[1] = False;
    TritSet set2(10);
    EXPECT_EQ(Unknown, set2[1].GetTritValue());
    set2[1] = False;
    EXPECT_EQ(False, set2[1].GetTritValue());
    TritSet set3 = set | set2;
    EXPECT_EQ(False, set3[1].GetTritValue());
    EXPECT_EQ(True, set3[20].GetTritValue());
    EXPECT_EQ(Unknown, set3[10].GetTritValue());
    EXPECT_EQ(32, set3.GetSize());
}

TEST(TritSetTest, ShrinkTest) {
    TritSet set(1000);
    EXPECT_EQ(1008, set.GetSize());
    set.Shrink();
    EXPECT_EQ(16, set.GetSize());
    set[1000] = True;
    EXPECT_EQ(1008, set.GetSize());
    set.Shrink();
    EXPECT_EQ(1008, set.GetSize());
}
TEST(TritSetTest, GetCountOfTritsWithType) {
    TritSet set(1000);
    EXPECT_EQ(0, set.GetCountOfTritsWithType(Unknown));
    set[1000] = True;
    EXPECT_EQ(1000, set.GetCountOfTritsWithType(Unknown));
    EXPECT_EQ(1, set.GetCountOfTritsWithType(True));
    EXPECT_EQ(0, set.GetCountOfTritsWithType(False));
    set[1000] = Unknown;
    unordered_map<Trit, uint> test = set.Cardinality();
    EXPECT_EQ(0, set.GetCountOfTritsWithType(False));
    EXPECT_EQ(0, set.GetCountOfTritsWithType(True));
    EXPECT_EQ(0, set.GetCountOfTritsWithType(Unknown));
    set[0] = True;
    set[500] = True;
    set[1007] = True;
    EXPECT_EQ(0, set.GetCountOfTritsWithType(False));
    EXPECT_EQ(3, set.GetCountOfTritsWithType(True));
    EXPECT_EQ(1005, set.GetCountOfTritsWithType(Unknown));
    set[500] = False;
    EXPECT_EQ(1, set.GetCountOfTritsWithType(False));
    EXPECT_EQ(2, set.GetCountOfTritsWithType(True));
    EXPECT_EQ(1005, set.GetCountOfTritsWithType(Unknown));
}

TEST(TritSetTest, GetLastSettedTritInd) {
    TritSet set(1000);
    set[15] = False;
    EXPECT_EQ(15, set.GetLastSettedTritInd());
    set[499] = True;
    EXPECT_EQ(499, set.GetLastSettedTritInd());
    set[0] = True;
    EXPECT_EQ(499, set.GetLastSettedTritInd());
    set[499] = False;
    EXPECT_EQ(499, set.GetLastSettedTritInd());
    set[500] = False;
    EXPECT_EQ(500, set.GetLastSettedTritInd());
    set[501] = True;
    EXPECT_EQ(501, set.GetLastSettedTritInd());
    set[501] = Unknown;
    EXPECT_EQ(500, set.GetLastSettedTritInd());
}
TEST(TritSetTest, Trim) {
    TritSet set(1000);
    uint size = set.GetSize();
    set[1004] = False;
    set[1005] = True;
    set[1006] = True;
    set[1007] = True;
    set.Trim(1004);
    EXPECT_EQ(False, set[1004].GetTritValue());
    EXPECT_EQ(Unknown, set[1005].GetTritValue());
    EXPECT_EQ(Unknown, set[1006].GetTritValue());
    EXPECT_EQ(Unknown, set[1007].GetTritValue());
    EXPECT_EQ(size, set.GetSize());
    set[15] = False;
    set.Trim(15);
    EXPECT_EQ(False, set[15].GetTritValue());
    EXPECT_EQ(set.GetSize(), 16);
}

TEST(TritSetTest, Cardinality) {
    TritSet set(1000);
    unordered_map<Trit, uint> test = set.Cardinality();
    EXPECT_EQ(0, test[False]);
    EXPECT_EQ(0, test[True]);
    EXPECT_EQ(0, test[Unknown]);
    set[0] = True;
    set[500] = True;
    set[1007] = True;
    test = set.Cardinality();
    EXPECT_EQ(0, test[False]);
    EXPECT_EQ(3, test[True]);
    EXPECT_EQ(1005, test[Unknown]);
    set[500] = False;
    test = set.Cardinality();
    EXPECT_EQ(1, test[False]);
    EXPECT_EQ(2, test[True]);
    EXPECT_EQ(1005, test[Unknown]);
}

TEST(TritSetTest, ostreamOperator) {
    TritSet set(1000);
    set[0] = False;
    set[2] = True;
    cout << set[0] << set[1] << set[2];
}

TritSet Function(TritSet my_trit) {

    return my_trit;
}

TEST(TritSetTest, IteratorTest) {
    TritSet setA(100);

    for (auto iter : setA) {
        iter = True;
    }

    for (auto iter : setA) {
        EXPECT_EQ(iter, True);
    }
    TritSet setB(100);

    EXPECT_EQ(setB[0], True);
    EXPECT_EQ(setB[99], True);

    const TritSet c = setB;
    EXPECT_EQ(c[0], False);
    EXPECT_EQ(c.Capacity(), setB.Capacity());

    TritSet setD = Function(setA);
    EXPECT_EQ(setA.Capacity(), setD.Capacity());
}

