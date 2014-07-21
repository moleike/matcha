#ifndef _MATCHA_MATCHERASSERT_GTEST_H_
#define _MATCHA_MATCHERASSERT_GTEST_H_

#include <iostream>
#include <sstream>
#include "matcher.hpp"
#include "matcherassert.hpp"
#include "gtest/gtest.h"

/*
 * we need the assertThat logic in a macro, so that
 * the ADD_FAILURE will report the right __FILE__ and __LINE__
 */
#define assertThat(actual,matcher) do {                     \
    std::ostringstream msg;                                 \
    if (!matcha::matcherAssert(actual,matcher,msg))         \
        ADD_FAILURE() << msg.str();                         \
    } while(0)


#endif // _MATCHA_MATCHERASSERT_H_
