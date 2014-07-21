#ifndef _MATCHA_MATCHERASSERT_H_
#define _MATCHA_MATCHERASSERT_H_

#include <sstream>
#include <vector>
#include <iterator>
#include "matcher.hpp"
#include "prettyprint.hpp"

namespace matcha {

template<typename T>
bool matcherAssert(const T& actual, Matcher<T> const& matcher, std::ostream& msg) {
    if (!matcher.matches(actual)) {
        std::ostringstream os;
        os << "Expected: "
           << matcher
           << "\n but got: "
           << actual;
        msg << os.str();
        return false;
    }
    return true;
}

// raw C-style arrays are wrapped in std::vector
template<typename T , size_t N>
bool matcherAssert(T const (& actual)[N], Matcher<std::vector<T> > const& matcher, std::ostream& msg) {
    std::vector<T> wrapper(std::begin(actual), std::end(actual));
    return matcherAssert(wrapper, matcher, msg);
}

} // namespace matcha


#endif // _MATCHA_MATCHERASSERT_H_

