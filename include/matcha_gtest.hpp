#ifndef _MATCHA_MATCHERASSERT_GTEST_H_
#define _MATCHA_MATCHERASSERT_GTEST_H_

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <iterator>
#include <utility>
#include "matcher.hpp"
#include "prettyprint.hpp"
#include "gtest/gtest.h"

/* 
 * Google Test implementation of matcher assertions
*/

namespace matcha {
namespace gtest {

template<typename T>
::testing::AssertionResult matcherAssert(const char*,
                                         const char*,
                                         const T& actual, 
                                         Matcher<T> const& matcher) {
    if (!matcher.matches(actual)) {
        std::ostringstream os;
        os << "Expected: "
           << matcher
           << "\n but got: "
           << actual;
        return  ::testing::AssertionFailure() 
            << os.str();
    }
    return ::testing::AssertionSuccess();
}

// raw C-style arrays are wrapped in std::vector
template<typename T , size_t N>
::testing::AssertionResult matcherAssert(const char*,
                                         const char*,
                                         T const (& actual)[N], 
                                         Matcher<std::vector<T> > const& matcher) {
    std::vector<T> wrapper(std::begin(actual), std::end(actual));
    return matcherAssert(0, 0, wrapper, matcher);
}

// C style strings are converted to std::string
template<size_t N>
::testing::AssertionResult matcherAssert(const char*,
                                         const char*,
                                         const char (& actual)[N], 
                                         Matcher<std::string> const& matcher) {
    std::string wrapper(std::begin(actual), std::end(actual));
    return matcherAssert(0, 0, wrapper, matcher);
}

// pointer matchers
template<typename T, typename U,
         typename std::enable_if<std::is_convertible<T*,U*>::value>::type* = nullptr>
::testing::AssertionResult matcherAssert(const char*,
                                         const char*,
                                         const T* actual, 
                                         Matcher<U*> const& matcher) {
    if (!matcher.matches(actual)) {
        return  ::testing::AssertionFailure() 
            << "Expected: "
            << matcher
            << "\n but got: "
            << actual;
    }
    return ::testing::AssertionSuccess();
}

} // namespace gtest
} // namespace matcha

/*
 * we need the assertThat logic in a macro, so that
 * the ADD_FAILURE will report the right __FILE__ and __LINE__
 */
#define assertThat(actual,matcher)  \
    ASSERT_PRED_FORMAT2(gtest::matcherAssert, actual, matcher)

#endif // _MATCHA_MATCHERASSERT_H_
