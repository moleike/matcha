#ifndef _MATCHA_MATCHERASSERT_H_
#define _MATCHA_MATCHERASSERT_H_

#include <sstream>
#include <vector>
#include <iterator>
#include "matcher.hpp"
#include "prettyprint.hpp"

namespace matcha {

template<typename AssertPolicy>
class MatcherAssert : private AssertPolicy {

public:
    template<typename T>
    void operator()(const T& actual, Matcher<T> const& matcher) {
        if (!matcher.matches(actual)) {
            std::ostringstream os;
            os << "\nExpected: "
               << matcher
               << "\n but got: "
               << actual;
            AssertPolicy::failure(os);
        }
    }

    // raw C-style arrays are wrapped in std::vector
    template<typename T , size_t N>
    void operator()(T const (& actual)[N], Matcher<std::vector<T> > const& matcher) {
        std::vector<T> wrapper(std::begin(actual), std::end(actual));
        operator()(wrapper, matcher);
    }
};

} // namespace matcha


#endif // _MATCHA_MATCHERASSERT_H_

