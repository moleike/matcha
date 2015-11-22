matcha
======

<b>matcha</b> (抹茶) is a C++11 port of [Java Hamcrest](http://hamcrest.org/JavaHamcrest/) framework of matcher objects. Currently supports Boost and Google C++ Testing frameworks. By default it prints to stdout, and can also throw an exception when an assertion fails.

Matcha follows a [policy-based](http://en.wikipedia.org/wiki/Policy-based_design) class design, where both matchers and result actions are policies, with the aim of simplifying the writing of custom matchers.

Examples
--------
```cpp
TEST(Matcha, testExample) {
    std::array<std::string,3> tautogram = { "veni", "vidi", "vici" };
    assertThat(tautogram, not(everyItem(endsWith("i"))));
}
```
This assertion fails, and we get this message:
```
Expected: not every item ends with "i"
but got : [veni, vidi, vici]
``` 

More examples [here](examples).

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

Let's say you need to test if a word is a palindrome. This is the test we want to write:
```cpp
TEST(Example, palindrome) {
    assertThat("kayak", is(palindrome()));
}
```
In matcha, we write policy classes to encapsulate the matcher behaviour, which comprises two methods:
```cpp
struct IsPalindrome 
{
    bool matches(const std::string& s) const 
		{
			return std::equal(s.begin(), s.begin() + s.size()/2, s.rbegin());
		}

    void describe(std::ostream& o) const 
		{ 
			o << "a palindromic word";
		}
};
```
Create a matcher with the desired name:
```cpp
auto palindrome = make_matcher<IsPalindrome>();
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
