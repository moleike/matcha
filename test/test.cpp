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
 * Sample code for Matcha on Google Tests
 * All the tests in this sample code are made to deliberately fail
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
}

TEST(Matcha, CArraysContaining) {
    int w[] = {1,2,5,3,6};
    assertThat(w, is(not(containing(6))));
}

TEST(Matcha, CArraysEquals) {
    int array1[5] = {1,3,3,4,5};
    int array2[6] = {1,2,3,4,5,6};
    assertThat(array1, is(equalTo(array2)));
}

TEST(Matcha, CStringEquals) {
    assertThat("foo", is(equalTo("bar")));
}

TEST(Matcha, CStringContaining) {
    assertThat("hello, world", is(not(containing("hello"))));
}

TEST(Matcha, vectorsEquals) {
    std::vector<int> v;
    v.push_back(4);
    std::vector<int> w;
    w.push_back(4);
    assertThat(v, is(not(equalTo(w))));
}

TEST(Matcha, vectorsContaining) {
    std::vector<int> v;
    v.push_back(3);
    v.push_back(5);
    v.push_back(1);
    assertThat(v, is(containing(6)));
}

TEST(Matcha, mapsEquals) {
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

TEST(Matcha, mapsHasKey) {
    std::map<int,int> mapval;
    mapval[0] = 0;
    mapval[1] = 3;
    mapval[2] = 3;
    assertThat(mapval, hasKey(3));
}

TEST(Matcha, setsEquals) {
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

TEST(Matcha, StringStartWith) {
    assertThat("myStringOfNote", startsWith("you"));
}

TEST(Matcha, StringEndsWith) {
    assertThat("myStringOfNote", endsWith("Notes"));
}

TEST(Matcha, testAnyOf) {
    std::vector<int> v;
    v.push_back(4);
    v.push_back(5);
    assertThat(v, is(anyOf(containing(3), containing(6))));
}

TEST(Matcha, testAllOf) {
    std::vector<int> v;
    v.push_back(4);
    v.push_back(5);
    assertThat(v, allOf(containing(4), containing(6)));
}

TEST(Matcha, testStringAllof) {
    assertThat("myStringOfNote", is(allOf(startsWith("you"), endsWith("Note"))));
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
