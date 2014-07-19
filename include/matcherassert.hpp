#ifndef _MATCHA_MATCHERASSERT_H_
#define _MATCHA_MATCHERASSERT_H_

#include <iostream>
#include <iterator>
#include <vector>
#include "matcher.hpp"
#include "prettyprint.hpp"

namespace matcha {

namespace MatcherAssert {

    template<typename T>
    void assertThat(const T& actual, Matcher<T> const& matcher) {
        if (!matcher.matches(actual)) {
            std::cout << "\nExpected: "
                      << matcher
                      << "\n\tbut got: "
                      << actual
                      << std::endl;
        }
    }

    // raw C-style arrays are wrapped in std::vector
    template<typename T, size_t N>
    void assertThat(T const (& actual)[N], Matcher<std::vector<T> > const& matcher) {
        std::vector<T> wrapper(std::begin(actual), std::end(actual));
        assertThat(wrapper, matcher);
    }

} // namespace MatcherAssert

} // namespace matcha

#endif // _MATCHA_MATCHERASSERT_H_

