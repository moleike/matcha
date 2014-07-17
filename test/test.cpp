#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include "matcherassert.hpp"
#include "isequal.hpp"
#include "is.hpp"
#include "isnot.hpp"

int main()
{
    using namespace matcha;
    using namespace matcha::MatcherAssert;

    int foo = 5;
    int bar = 5;
    assertThat(foo, is(not(equalTo(bar))));
    int array1[5] = {1,3,3,4,5};
    int array2[6] = {1,2,3,4,5,6};
    assertThat(array1, is(equalTo(array2)));
    assertThat("foo", is(equalTo("foo")));
    std::vector<int> v;
    v.push_back(3);
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
    return 0;
}
