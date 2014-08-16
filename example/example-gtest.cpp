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

struct S {
    int x;
    int y;
};

TEST(Matcha, testx) {
    S a = { 0,1 };
    S b = { 0,1 };
    assertThat(a, is(not(equalTo(b))));
}

TEST(Matcha, testIntegralValues) {
    int foo = 5;
    int bar = 5;
    assertThat(foo, is(not(equalTo(bar))));
}

TEST(Matcha, testCArraysContains) {
    int w[] = {1,2,5,3,6};
    assertThat(w, not(contains(6)));
}

TEST(Matcha, testCArraysEquals) {
    int array1[5] = {1,3,3,4,5};
    int array2[6] = {1,2,3,4,5,6};
    assertThat(array1, is(equalTo(array2)));
}

TEST(Matcha, testCStringEquals) {
    assertThat("foo", is(equalTo("bar")));
}

TEST(Matcha, testCStringContains) {
    assertThat("hello, world", not(contains("hello")));
}

TEST(Matcha, testVectorsEquals) {
    std::vector<int> v,w;
    v.push_back(4);
    w.push_back(4);
    assertThat(v, is(not(equalTo(w))));
}

TEST(Matcha, testVectorsContains) {
    std::vector<int> v;
    v.push_back(3);
    v.push_back(5);
    v.push_back(1);
    assertThat(v, contains(6));
}

TEST(Matcha, testMapsEquals) {
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

TEST(Matcha, testMapContains) {
    std::map<int,int> mapval;
    mapval[0] = 0;
    mapval[1] = 3;
    mapval[2] = 3;
    assertThat(mapval, contains(3,3));
}

TEST(Matcha, testMapsHasKey) {
    std::map<int,int> mapval;
    mapval[0] = 0;
    mapval[1] = 3;
    mapval[2] = 3;
    assertThat(mapval, hasKey(3));
}

TEST(Matcha, testSetsEquals) {
    int x[] = {1,2,5,3,4};
    int y[] = {1,2,3,5,5};
    std::set<int> xx(std::begin(x), std::end(x));
    std::set<int> yy(std::begin(y), std::end(y));
    assertThat(xx, equalTo(yy));
}

TEST(Matcha, testNullPointers) {
    int foo = 0;
    int *p = &foo;
    assertThat(p, is(null()));
}

TEST(Matcha, testStringStartWith) {
    assertThat("myStringOfNote", startsWith("you"));
}

TEST(Matcha, testStringEndsWith) {
    assertThat("myStringOfNote", endsWith("Notes"));
}

TEST(Matcha, testAnyOf) {
    std::vector<int> v;
    v.push_back(4);
    v.push_back(5);
    assertThat(v, is(anyOf(contains(3), contains(6))));
}

TEST(Matcha, testAllOf) {
    std::vector<int> v;
    v.push_back(4);
    v.push_back(5);
    assertThat(v, allOf(contains(4), contains(6)));
}

TEST(Matcha, testStringAllof) {
    assertThat("myStringOfNote", is(allOf(startsWith("my"), endsWith("Note"))));
}

TEST(Matcha, testStringEveryItem) {
    std::vector<std::string> vs;
    vs.push_back("192.168.0.1");
    vs.push_back("192.168.2.1");
    vs.push_back("192.168.0.5");
    assertThat(vs, everyItem(startsWith("192.168")));
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
