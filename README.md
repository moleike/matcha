matcha
======

Matcha (抹茶) is a C++ port of [Java Hamcrest](http://hamcrest.org/JavaHamcrest/) framework of matcher objects.

From the original project:
> Provides a library of matcher objects (also known as constraints or predicates) allowing 'match' rules to be defined declaratively, to be used in other frameworks. Typical scenarios include testing frameworks, mocking libraries and UI validation rules.

Matcha provides an implementation of most of the core matchers from Hamcrest and currently integrates with [Google C++ Testing Framework](https://code.google.com/p/googletest/). Google Test assertions can co-exist with Matcha's.

Besides unit testing and mocking frameworks, there are many interesting use cases of matcher objects, see http://code.google.com/p/hamcrest/wiki/UsesOfHamcrest for some examples.

Matcha follows a [policy-based](http://en.wikipedia.org/wiki/Policy-based_design) class design, where both matchers and result actions are policies, with the aim of simplifying the writing of custom matchers.

Dependencies
============
* C++11 support; Clang 3.3 or gcc 4.8, and above
* Google Test: downloaded and built as an external project with CMake
* CMake


Build
=====
You can get started with the test examples.

```sh
mkdir build
cd build
cmake ..
make
./test/example_test
```

Examples
========
```
TEST(Matcha, testMyArray) {
    int w[] = {1,2,5,3,6};
    assertThat(w, not(contains(6)));
}
```
```
TEST(Matcha, testStringEveryItem) {
    std::vector<std::string> vs;
    vs.push_back("192.168.0.1");
    vs.push_back("192.168.2.1");
    assertThat(vs, everyItem(startsWith("192.168.")));
}
```
```
TEST(Matcha, testMyString) {
    assertThat("myStringOfNote", is(anyOf(startsWith("you"), endsWith("Note"))));
}
```
```
TEST(Matcha, testMyVector) {
    std::vector<int> v, w;
    v.push_back(4);
    w.push_back(5);
    assertThat(v, is(not(equalTo(w))));
}
```
```
TEST(Matcha, testMapContains) {
    std::map<int,int> u;
    u[3] = 4;
    assertThat(u, contains(3,4));
}
```

Writing Custom Matchers
=======================

Let's write our own matcher for testing if a double value has the value NaN (not a number). This is the test we want to write:
```
TEST(NotANumber, testSquareRootOfMinusOneIsNotANumber) {
    assertThat(std::sqrt(-1), is(notANumber()));
}
```
In matcha, we write policy classes to encapsulate the matcher behaviour, which comprises two methods:
```
class IsNotANumber {
protected:
    bool matches(double actual) const {
        return std::isnan(actual);
    }
    // describe what you expected
    void describe(std::ostream& o) const {
       o << "not a number (NaN);
    }
};
```
Matcher is the host class taking as type parameter the policy class:
```
Matcher<IsNotANumber> notANumber() {
    return Matcher<IsNotANumber>();
}
```
There are two other type paramteres: the expected value type, for matchers that have an argument (defaults to void) and the output policy, which defautls to Google Test.

If the test fails:
```
[ RUN      ] TestNaN.SomeValue
/Users/alex/Playground/matcha/test/example-gtest.cpp:47: Failure
Expected: is not a number (NaN)
 but got: 1.0
[  FAILED  ] TestNaN.SomeValue (0 ms)
```
Notes
=====
Currently works well with primitive types and std containers. User-defined types should provide:
- *operator==* for equality comparison unless are plain-old data types
- *operator<<* for insertion into an output source, printing "\<unkown-type\>" otherwise.

License
=======
Apache License 2.0
