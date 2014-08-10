matcha
======

Matcha (抹茶) is a C++ port of [Java Hamcrest](http://hamcrest.org/JavaHamcrest/) framework of matcher objects.

Matcha provides an implementation of most of the core matchers from Hamcrest and currently integrates with [Google C++ Testing Framework](https://code.google.com/p/googletest/). Google Test assertions can co-exist with Matcha's.

From the original project:
> Provides a library of matcher objects (also known as constraints or predicates) allowing 'match' rules to be defined declaratively, to be used in other frameworks. Typical scenarios include testing frameworks, mocking libraries and UI validation rules.


Matcha should be easily extensible with custom matchers since it uses policy-based classes for the matchers.

Examples
========
```cpp
#include "gtest/gtest.h"
#inlcude \"matcha/matcha.hpp"

TEST(Matcha, testMyString) {
    assertThat("myStringOfNote", is(anyOf(startsWith("you"), endsWith("Note"))));
}
```

License
=======
Apache License 2.0
