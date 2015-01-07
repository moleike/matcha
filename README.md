matcha
======

Matcha (抹茶) is a C++ port of [Java Hamcrest](http://hamcrest.org/JavaHamcrest/) framework of matcher objects.

From the original project:
> Provides a library of matcher objects (also known as constraints or predicates) allowing 'match' rules to be defined declaratively, to be used in other frameworks. Typical scenarios include testing frameworks, mocking libraries and UI validation rules.

Currently provides an implementation of most of the core matchers from Hamcrest and currently integrates with [Google C++ Testing Framework](https://code.google.com/p/googletest/).

Matcha follows a [policy-based](http://en.wikipedia.org/wiki/Policy-based_design) class design, where both matchers and result actions are policies, with the aim of simplifying the writing of custom matchers.

Dependencies
------------
* C++11 support; Clang 3.3 or gcc 4.8, and above
* Google Test: downloaded and built as an external project with CMake
* CMake


Build
-----
You can get started with the test examples.

```sh
mkdir build
cd build
cmake ..
make
./test/example_test
```

Examples
--------
Examples are located in test directory.

Writing Custom Matchers
-----------------------

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
There are two other type paramteres: the expected value type, for matchers that have an argument (defaults to void) and the output policy, which defaults to stdout.

For example, the result of the failure using GTest:
```
[ RUN      ] TestNaN.SomeValue
/Users/alex/Playground/matcha/test/example-gtest.cpp:47: Failure
Expected: is not a number (NaN)
 but got: 1.0
[  FAILED  ] TestNaN.SomeValue (0 ms)
```
Notes
-----
Currently works well with primitive types and std containers. User-defined types should provide:
- *operator==* for equality comparison unless are plain-old data types
- *operator<<* for insertion into an output source, printing "\<unkown-type\>" otherwise.

Other Uses
----------
Besides unit testing and mocking frameworks, there are many interesting use cases of matcher objects, see http://code.google.com/p/hamcrest/wiki/UsesOfHamcrest for some examples.


License
-------
Apache License 2.0
