#ifndef _MATCHA_MATCHERASSERT_GTEST_H_
#define _MATCHA_MATCHERASSERT_GTEST_H_

#include <iostream>
#include <sstream>
#include "matcher.hpp"
#include "matcherassert.hpp"

namespace matcha {

struct AssertPolicyGTest { 
    void failure(const std::ostringstream& msg) {
        std::cout << msg.str()<< std::endl;
    }
};

MatcherAssert<AssertPolicyGTest> assertThat;

} // namespace matcha

#endif // _MATCHA_MATCHERASSERT_H_
