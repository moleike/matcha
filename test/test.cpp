/* vim: set sw=4 ts=4 et : */
/* test.cpp: Google Test example
 *
 * Copyright (C) 2014 Alexandre Moreno
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include "matcha/matcha.hpp"

using namespace matcha;

TEST(Matcha, IntegralValues) {
    int foo = 5;
    int bar = 5;
    assertThat(foo, is(not(equalTo(bar))));
    assertThat(4, is(equalTo(-4)));
}

TEST(Matcha, CArrays) {
    int w[] = {1,2,5,3,6};
    assertThat(w, is(not(containing(6))));

    int array1[5] = {1,3,3,4,5};
    int array2[6] = {1,2,3,4,5,6};
    assertThat(array1, is(equalTo(array2)));
}

TEST(Matcha, CString) {
    assertThat("foo", is(equalTo("bar")));
    assertThat("hello, world", is(not(containing("hello"))));
}

TEST(Matcha, STLContainers) {
    std::vector<int> v;
    v.push_back(4);
    std::vector<int> w;
    w.push_back(4);
    assertThat(v, is(not(equalTo(w))));

    v.push_back(3);
    v.push_back(5);
    v.push_back(1);
    assertThat(v, is(containing(6)));

    std::map<int,int> mapval;
    mapval[0] = 0;
    mapval[1] = 3;
    mapval[2] = 3;
    std::map<int,int> mapval2;
    mapval2[0] = 0;
    mapval2[1] = 1;
    mapval2[2] = 3;
    assertThat(mapval, equalTo(mapval2));

    int x[] = {1,2,5,3,4};
    int y[] = {1,2,3,5,5};
    std::set<int> xx(std::begin(x), std::end(x));
    std::set<int> yy(std::begin(y), std::end(y));
    assertThat(xx, equalTo(yy));
}

TEST(Matcha, NullPointers) {
    int foo = 0;
    int *p = &foo;
    assertThat(p, is(null()));
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
