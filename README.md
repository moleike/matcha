matcha
======

Matcha (抹茶) is a C++ port of [Java Hamcrest](http://hamcrest.org/JavaHamcrest/) framework of matcher objects. 

Matcha follows a [policy-based](http://en.wikipedia.org/wiki/Policy-based_design) class design, where both matchers and result actions are policies, with the aim of simplifying the writing of custom matchers.

Examples
--------
```
assertThat("foo1", is(allOf(not(emptyString()), matches("[a-z]+"))));
```
This assertion fails, and we get this message:
```
Expected: is all of not an empty string and a string matching the pattern [a-z]+.
 but got: foo1
``` 

Examples are located in test directory.

Integration
-----------
Currently supports Boost and Google C++ Testing frameworks. By default it prints to stdout, and can also throw an exception when an assertion fails.

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
