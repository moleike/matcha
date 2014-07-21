#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include "isequal.hpp"
#include "is.hpp"
#include "isnot.hpp"
#include "matcha_gtest.hpp"
#include "gtest/gtest.h"

using namespace matcha;

TEST(Matcha, IntegralValues) {
    int foo = 5;
    int bar = 5;
    assertThat(foo, is(not(equalTo(bar))));
    assertThat(4, is(equalTo(-4)));
}

TEST(Matcha, CArrays) {
    int array1[5] = {1,3,3,4,5};
    int array2[6] = {1,2,3,4,5,6};
    assertThat(array1, is(equalTo(array2)));
}

TEST(Matcha, CString) {
    assertThat("foo", is(equalTo("bar")));
}

TEST(Matcha, STLContainers) {
    std::vector<int> v;
    v.push_back(4);
    std::vector<int> w;
    w.push_back(4);
    assertThat(v, equalTo(w));

    std::map<int,int> mapval;
    mapval[0] = 0;
    mapval[1] = 3;
    mapval[2] = 3;
    std::map<int,int> mapval2;
    mapval2[0] = 0;
    mapval2[1] = 1;
    mapval2[2] = 3;
    assertThat(mapval, equalTo(mapval2));
}

TEST(Matcha, Sets) {
    int x[] = {1,2,5,3,4};
    int y[] = {1,2,3,5,5};
    std::set<int> xx(std::begin(x), std::end(x));
    std::set<int> yy(std::begin(y), std::end(y));
    assertThat(xx, equalTo(yy));
}


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
