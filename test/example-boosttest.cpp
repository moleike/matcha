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

#define BOOST_TEST_MODULE example-boosttest
#define MATCHA_BOOSTTEST
#include "matcha/matcha.hpp"

using namespace matcha;

BOOST_AUTO_TEST_CASE(testIntegralValues) {
    int foo = 5;
    int bar = 5;
    assertThat(foo, is(not(equalTo(bar))));
}

BOOST_AUTO_TEST_CASE(testCArraysContains) {
    int w[] = {1,2,5,3,6};
    assertThat(w, not(contains(6)));
}

BOOST_AUTO_TEST_CASE(testCArraysEquals) {
    int array1[5] = {1,3,3,4,5};
    int array2[6] = {1,2,3,4,5,6};
    assertThat(array1, is(equalTo(array2)));
}

BOOST_AUTO_TEST_CASE(testCStringEquals) {
    assertThat("foo", is(equalTo("bar")));
}

BOOST_AUTO_TEST_CASE(testCStringContains) {
    assertThat("hello, world", not(contains("hello")));
}

BOOST_AUTO_TEST_CASE(testVectorsEquals) {
    std::vector<int> v,w;
    v.push_back(4);
    w.push_back(4);
    assertThat(v, is(not(equalTo(w))));
}

BOOST_AUTO_TEST_CASE(testVectorsContains) {
    std::vector<int> v;
    v.push_back(3);
    v.push_back(5);
    v.push_back(1);
    assertThat(v, contains(6));
}

BOOST_AUTO_TEST_CASE(testMapsEquals) {
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

BOOST_AUTO_TEST_CASE(testMapContains) {
    std::map<int,int> mapval;
    mapval[0] = 0;
    mapval[1] = 3;
    mapval[2] = 3;
    assertThat(mapval, contains(3,3));
}

BOOST_AUTO_TEST_CASE(testMapsHasKey) {
    std::map<int,int> mapval;
    mapval[0] = 0;
    mapval[1] = 3;
    mapval[2] = 3;
    assertThat(mapval, hasKey(3));
}

BOOST_AUTO_TEST_CASE(testSetsEquals) {
    int x[] = {1,2,5,3,4};
    int y[] = {1,2,3,5,5};
    std::set<int> xx(std::begin(x), std::end(x));
    std::set<int> yy(std::begin(y), std::end(y));
    assertThat(xx, equalTo(yy));
}

BOOST_AUTO_TEST_CASE(testNullPointers) {
    int foo = 0;
    int *p = &foo;
    assertThat(p, is(null()));
}

BOOST_AUTO_TEST_CASE(testStringStartWith) {
    assertThat("myStringOfNote", startsWith("you"));
}

BOOST_AUTO_TEST_CASE(testStringEndsWith) {
    assertThat("myStringOfNote", endsWith("Notes"));
}

BOOST_AUTO_TEST_CASE(testAnyOf) {
    std::vector<int> v;
    v.push_back(4);
    v.push_back(5);
    assertThat(v, is(anyOf(contains(2), contains(3), contains(6))));
}

BOOST_AUTO_TEST_CASE(testAllOf) {
    std::vector<int> v;
    v.push_back(4);
    v.push_back(5);
    assertThat(v, is(allOf(contains(4), contains(5), contains(6))));
}

BOOST_AUTO_TEST_CASE(testStringAllof) {
    assertThat("myStringOfNote", is(allOf(startsWith("my"), endsWith("Notes"))));
}

BOOST_AUTO_TEST_CASE(testStringEveryItem) {
    std::vector<std::string> vs;
    vs.push_back("192.168.0.1");
    vs.push_back("192.168.0.2");
    vs.push_back("192.168.0.4");
    assertThat(vs, everyItem(startsWith("192.168")));
    assertThat(vs, everyItem(matches("^192\\.168\\.0\\.[1-3]$")));
}


BOOST_AUTO_TEST_CASE(testStringIgnoreCase) {
    assertThat("foo", is(equalToIgnoringCase("Foo")));
}

BOOST_AUTO_TEST_CASE(testStringIgnoreWhiteSpace) {
    assertThat("   my\tfoo  bar ", is(equalToIgnoringWhiteSpace(" my  foo bar")));
}

BOOST_AUTO_TEST_CASE(testRegex) {
    assertThat("12345a", matchesPattern("[0-9]+"));
}

BOOST_AUTO_TEST_CASE(testCloseTo) {
    assertThat(0.98, is(closeTo(1.0, 0.03)));
    assertThat(0.98f, is(closeTo(1.0f, 0.03f)));
}

BOOST_AUTO_TEST_CASE(testx) {
    struct S { int x; int y; } a = { 0,1 }, b = { 0,2 };
    assertThat(a, is(not(equalTo(b))));
}

typedef std::vector<std::string> stringlist;
BOOST_AUTO_TEST_CASE(testStringInArray) {
    assertThat("baz", is(in(stringlist{"bar","foo"})));
}

BOOST_AUTO_TEST_CASE(testIntIsOneOf) {
    int i = 1, j =2, k =3;
    assertThat(i, is(oneOf(j,k)));
    assertThat(4, is(oneOf(1,2,3)));
}

BOOST_AUTO_TEST_CASE(testStringIsOneOf) {
    assertThat("baz", is(oneOf("bar", "foo", "qux", "quux")));
}

