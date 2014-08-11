matcha
======

Matcha (抹茶) is a C++ port of [Java Hamcrest](http://hamcrest.org/JavaHamcrest/) framework of matcher objects.

From the original project:
> Provides a library of matcher objects (also known as constraints or predicates) allowing 'match' rules to be defined declaratively, to be used in other frameworks. Typical scenarios include testing frameworks, mocking libraries and UI validation rules.

Matcha provides an implementation of most of the core matchers from Hamcrest and currently integrates with [Google C++ Testing Framework](https://code.google.com/p/googletest/). Google Test assertions can co-exist with Matcha's. It can be easily extended with custom matchers since it uses [policy-based](http://en.wikipedia.org/wiki/Policy-based_design) classes for the matchers.

Examples
========
```cpp
TEST(Matcha, testMyArray) {
    int w[] = {1,2,5,3,6};
    assertThat(w, not(contains(6)));
}

TEST(Matcha, testMyString) {
    assertThat("myStringOfNote", is(anyOf(startsWith("you"), endsWith("Note"))));
}

TEST(Matcha, testMyVector) {
    std::vector<int> v, w;
    v.push_back(4);
    w.push_back(5);
    assertThat(v, is(not(equalTo(w))));
}

TEST(Matcha, testMapContains) {
    std::map<int,int> u;
    u[3] = 4;
    assertThat(u, contains(3,4));
}
```

Notes
=====
Currently works well with primitive types and std containers. User-defined types should provide:
- *operator==* for equality comparison unless are plain-old data types
- *operator<<* for insertion into an output source, printing "\<unkown-type\>" otherwise.

License
=======
Apache License 2.0
