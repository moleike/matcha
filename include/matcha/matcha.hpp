/* vim: set sw=4 ts=4 et : */
/* matcha.hpp: matcher objects for GoogleTest
 *
 * Copyright (C) 2014 Alexandre Moreno
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#ifndef _MATCHA_H_
#define _MATCHA_H_

#include <utility>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator> 
#include <functional>
#include <set>
#include <vector>
#include <string>
#include <tuple>
#include <cstring>
#include <type_traits>
#include "prettyprint.hpp"
#include "gtest/gtest.h"

/* we need the assertThat logic in a macro, so that
 * the ADD_FAILURE will report the right __FILE__ and __LINE__
 */
#define assertThat(actual,matcher)  \
    ASSERT_PRED_FORMAT2(assertResult, actual, matcher)

namespace matcha {

// SFINAE type trait to detect whether type T satisfies EqualityComparable.

template<typename T, typename = void>
struct is_equality_comparable : std::false_type
{ };

template<typename T>
struct is_equality_comparable<T,
    typename std::enable_if<
        true,
        decltype((std::declval<T>() == std::declval<T>()), (void)0)
        >::type
    > : std::true_type
{ };

// templated operator<< when type T is not std container (prettyprint)
// and user-defined insertion operator is not provided

template<typename T, typename TChar, typename TCharTraits>
typename std::enable_if<
    !::pretty_print::is_container<T>::value, 
    std::basic_ostream<TChar, TCharTraits>&
    >::type
operator<<(std::basic_ostream<TChar, TCharTraits> &os, const T &)
{
    const char s[] = "<unknown-type>";
    os.write(s, sizeof(s) - 1);
    return os;
}

struct DefaultOutputPolicy {
    typedef bool return_type;
protected:
    bool print(std::string const& expected, std::string const& actual, bool assertion) const {
        if (!assertion) {
            std::cout << "Expected: " << expected << "\n but got: " << actual << std::endl;
            return false; 
        }
        return true;
    }
};

struct GTestOutputPolicy {
    typedef ::testing::AssertionResult return_type;
protected:
    // Google Test implementation of matcher assertions
    ::testing::AssertionResult print(std::string const& expected, 
                                     std::string const& actual, 
                                     bool assertion) const {
        if (!assertion) {
            return ::testing::AssertionFailure() 
                   << "Expected: " << expected << "\n but got: " << actual;
        }
        return ::testing::AssertionSuccess();
    }
};

template<class MatcherPolicy,class ExpectedType, class OutputPolicy = GTestOutputPolicy>
class Matcher : private MatcherPolicy, private OutputPolicy {
public:
    typedef Matcher<MatcherPolicy,ExpectedType> type;
    Matcher(ExpectedType const& value = ExpectedType()) : expected_(value)
    { }

    template<class ActualType>
    bool matches(ActualType const& actual) const {
        return MatcherPolicy::matches(expected_, actual);
    }

    template<size_t N>
    bool matches(ExpectedType const (&actual)[N]) const {
        std::vector<ExpectedType> wrapper(std::begin(actual), std::end(actual));
        return MatcherPolicy::matches(expected_, wrapper);
    }

    template<class ActualType>
    friend auto assertResult(const char*, 
                             const char*,
                             ActualType const& actual,
                             type const& matcher) -> typename OutputPolicy::return_type {
        std::ostringstream sactual, smatcher;
        sactual << actual;
        smatcher << matcher;
        return matcher.print(smatcher.str(), sactual.str(), matcher.matches(actual));
    }

    template<size_t N>
    friend auto assertResult(const char*,
                             const char*,
                             ExpectedType const (&actual)[N],
                             type const& matcher) -> typename OutputPolicy::return_type {
        std::ostringstream sactual, smatcher;
        sactual << actual;
        smatcher << matcher;
        return matcher.print(smatcher.str(), sactual.str(), matcher.matches(actual));
    }

    friend std::ostream& operator<<(std::ostream& o, type const& matcher) {
        matcher.describe(o);
        return o; 
    }
private:
    void describe(std::ostream& o) const {
        MatcherPolicy::describe(o, expected_);
    }
    ExpectedType expected_;
};

template<class MatcherPolicy, class ExpectedType, class OutputPolicy>
class Matcher<MatcherPolicy,ExpectedType*,OutputPolicy> : private MatcherPolicy, private OutputPolicy {
public:
    typedef Matcher<MatcherPolicy,ExpectedType*> type;
    Matcher(ExpectedType const* pvalue) : expected_(pvalue) { }

    template<class ActualType>
    bool matches(ActualType const* actual) const {
        return MatcherPolicy::matches(expected_, actual);
    }

    template<class ActualType>
    friend auto assertResult(const char*,
                             const char*,
                             ActualType const* actual, 
                             type const& matcher) -> typename OutputPolicy::return_type {
        std::ostringstream sactual, smatcher;
        sactual << actual;
        smatcher << matcher;
        return matcher.print(smatcher.str(), sactual.str(), matcher.matches(actual));
    }

    friend std::ostream& operator<<(std::ostream& o, type const& matcher) {
        matcher.describe(o);
        return o; 
    }
private:
    void describe(std::ostream& o) const {
        MatcherPolicy::describe(o, expected_);
    }
    ExpectedType const* expected_;
};

// raw C-style arrays are wrapped in std::vector
template<class MatcherPolicy, class ExpectedType, size_t N, class OutputPolicy>
class Matcher<MatcherPolicy,ExpectedType[N],OutputPolicy> : private MatcherPolicy, private OutputPolicy {
public:
    typedef Matcher<MatcherPolicy,ExpectedType[N]> type;
    Matcher(ExpectedType const (&value)[N]) 
        : expected_(std::begin(value), std::end(value)) { }

    template<size_t M>
    bool matches(ExpectedType const (&actual)[M]) const {
        std::vector<ExpectedType> wrapper(std::begin(actual), std::end(actual));
        return MatcherPolicy::matches(expected_, wrapper);
    }

    template<size_t M>
    friend auto assertResult(const char*,
                             const char*,
                             ExpectedType const (&actual)[M], 
                             type const& matcher) -> typename OutputPolicy::return_type {
        std::ostringstream sactual, smatcher;
        sactual << actual;
        smatcher << matcher;
        return matcher.print(smatcher.str(), sactual.str(), matcher.matches(actual));
    }

    friend std::ostream& operator<<(std::ostream& o, type const& matcher) {
        matcher.describe(o);
        return o; 
    }
private:
    void describe(std::ostream& o) const {
        MatcherPolicy::describe(o, expected_);
    }
    std::vector<ExpectedType> expected_;
};


// null-terminated strings are converted to std::string
template<class MatcherPolicy, size_t N, class OutputPolicy>
class Matcher<MatcherPolicy,char[N],OutputPolicy> : private MatcherPolicy, private OutputPolicy {
public:
    typedef Matcher<MatcherPolicy,char[N]> type;
    Matcher(char const (&value)[N]) 
        : expected_(value) { }

    template<size_t M>
    bool matches(char const (&actual)[M]) const {
        using namespace std;
        return MatcherPolicy::matches(expected_, string(actual));
    }

    template<size_t M>
    friend auto assertResult(const char*,
                             const char*,
                             char const (&actual)[M],
                             type const& matcher) -> typename OutputPolicy::return_type {
        std::ostringstream sactual, smatcher;
        sactual << actual;
        smatcher << matcher;
        return matcher.print(smatcher.str(), sactual.str(), matcher.matches(actual));
    }

    friend std::ostream& operator<<(std::ostream& o, type const& matcher) {
        matcher.describe(o);
        return o; 
    }
private:
    void describe(std::ostream& o) const {
        MatcherPolicy::describe(o, expected_);
    }
    std::string expected_;
};

struct Is {
protected:
    template<class Policy, class ExpType, class ActualType>
    bool matches(Matcher<Policy,ExpType> const& expected, ActualType const& actual) const {
        return expected.matches(actual);
    }

    template<class Policy, class ExpType>
    void describe(std::ostream& o, Matcher<Policy,ExpType> const& expected) const {
        o << "is " << expected;  
    }
};

template<class Policy, class ExpType>
Matcher<Is,Matcher<Policy,ExpType>> is(Matcher<Policy,ExpType> const& value) {
    return Matcher<Is,Matcher<Policy,ExpType>>(value);
}

struct IsNot {
protected:
    template<class Policy, class ExpType, class ActualType>
    bool matches(Matcher<Policy,ExpType> const& expected, ActualType const& actual) const {
        return !expected.matches(actual);
    }

    template<class Policy, class ExpType>
    void describe(std::ostream& o, Matcher<Policy,ExpType> const& expected) const {
        o << "not " << expected;  
    }
};

template<class Policy, class T>
Matcher<IsNot,Matcher<Policy,T>> operator!(Matcher<Policy,T> const& value) {
    return Matcher<IsNot,Matcher<Policy,T>>(value);
}

struct IsNull {
protected:
    template<typename T>
    bool matches(std::nullptr_t expected, T const* actual) const {
        return actual == expected;
    }

    template<typename T>
    void describe(std::ostream& o, T const& expected) const {
        o << "null pointer";  
    }
};

Matcher<IsNull,std::nullptr_t> null() {
    return Matcher<IsNull,std::nullptr_t>(nullptr);
}

struct IsEqual {
protected:
    template<typename T>
    bool matches(T const& expected, T const& actual,
                 typename std::enable_if<
                    is_equality_comparable<T>::value
                    >::type* = 0) const
    {
        return expected == actual;
    }

    template<typename T>
    bool matches(T const& expected, T const& actual,
                 typename std::enable_if<
                    !is_equality_comparable<T>::value
                    && std::is_pod<T>::value
                    >::type* = 0) const
    {
        return !std::memcmp(&expected, &actual, sizeof expected);
    }

    template<typename T>
    void describe(std::ostream& o, T const& expected) const {
       o << expected;  
    }
};

template<>
void IsEqual::describe(std::ostream& o, std::string const& expected) const {
   o << "\"" << expected << "\"";  
}

template<typename T>
Matcher<IsEqual,T> equalTo(T const& value) {
    return Matcher<IsEqual,T>(value);
}

template<typename T, size_t N>
Matcher<IsEqual,T[N]> equalTo(T const (&value)[N]) {
    return Matcher<IsEqual,T[N]>(value);
}

struct IsContaining {
protected:
    template<typename C, typename T,
         typename std::enable_if<std::is_same<typename C::value_type,T>::value>::type* = nullptr>
    bool matches(T const& item, C const& cont) const {
        return std::end(cont) != std::find(std::begin(cont), std::end(cont), item);
    }

    template<typename C = std::string, typename T = std::string>
    bool matches(std::string const& substr, std::string const& actual) const {
        return std::string::npos != actual.find(substr);
    }

    // overload for checking whether container values match a predicate specified by a Matcher
    template<typename C, typename T, typename Policy,
         typename std::enable_if<std::is_same<typename C::value_type,T>::value>::type* = nullptr>
    bool matches(Matcher<Policy,T> const& itemMatcher, C const& cont) const {
        using namespace std::placeholders;
        auto pred = std::bind(&Matcher<Policy,T>::template matches<T>, &itemMatcher, _1);
        return std::all_of(std::begin(cont), std::end(cont), pred);
    }

    template<typename T>
    void describe(std::ostream& o, T const& expected) const {
       o << "contains " << expected;  
    }
};

template<>
void IsContaining::describe(std::ostream& o, std::string const& expected) const {
   o << "contains " << "\"" << expected << "\"";  
}

template<typename T>
Matcher<IsContaining,T> contains(T const& value) {
    return Matcher<IsContaining,T>(value);
}

template<typename T, size_t N>
Matcher<IsContaining,T[N]> contains(T const (&value)[N]) {
    return Matcher<IsContaining,T[N]>(value);
}

template<class Key, class T>
Matcher<IsContaining,std::pair<const Key,T>> contains(Key const& key, T const& value) {
    return Matcher<IsContaining,std::pair<const Key,T>>(std::pair<const Key,T>(key, value));
}

template<typename T, typename Policy>
Matcher<IsContaining,Matcher<Policy,T>> everyItem(Matcher<Policy,T> const& itemMatcher) {
    return Matcher<IsContaining,Matcher<Policy,T>>(itemMatcher);
}

struct IsContainingKey {
protected:
    template<typename C, typename T,
         typename std::enable_if<std::is_same<typename C::key_type,T>::value>::type* = nullptr>
    bool matches(T const& key, C const& cont) const {
        for (auto const& val : cont) {
            if (val.first == key)
                return true;
        }
        return false;
    }

    template<typename T>
    void describe(std::ostream& o, T const& expected) const {
       o << "has key " << expected;  
    }
};

template<typename T>
Matcher<IsContainingKey,T> hasKey(T const& key) {
    return Matcher<IsContainingKey,T>(key);
}

struct StringStartsWith {
protected:
    bool matches(std::string const& substr, std::string const& actual) const {
        return !actual.compare(0, substr.size(), substr);
    }
    
    void describe(std::ostream& o, std::string const& expected) const {
       o << "starts with " << "\"" << expected << "\"";  
    }
};

Matcher<StringStartsWith,std::string> startsWith(std::string const& val) {
    return Matcher<StringStartsWith,std::string>(val);
}

struct StringEndsWith {
protected:
    bool matches(std::string const& substr, std::string const& actual) const {
        return !actual.compare(actual.size() - substr.size(), substr.size(), substr);
    }
    
    void describe(std::ostream& o, std::string const& expected) const {
       o << "ends with " << "\"" << expected << "\"";  
    }
};

Matcher<StringEndsWith,std::string> endsWith(std::string const& val) {
    return Matcher<StringEndsWith,std::string>(val);
}

struct AnyOf {
protected:
    template<class PolicyA, class PolicyB, class TA, class TB, class ActualType>
    bool matches(std::tuple<Matcher<PolicyA,TA>,Matcher<PolicyB,TB>> const& matchers, ActualType const& actual) const {
        return std::get<0>(matchers).matches(actual) || std::get<1>(matchers).matches(actual);
    }

    template<class PolicyA, class PolicyB, class TA, class TB>
    void describe(std::ostream& o, std::tuple<Matcher<PolicyA,TA>,Matcher<PolicyB,TB>> const& matchers) const {
        o << "any of " << std::get<0>(matchers) << " or " <<  std::get<1>(matchers);  
    }
};

template<class PolicyA, class PolicyB, class TA, class TB>
Matcher<AnyOf,std::tuple<Matcher<PolicyA,TA>,Matcher<PolicyB,TB>>> 
anyOf(Matcher<PolicyA,TA> const& matcherA, Matcher<PolicyB,TB> const& matcherB) {
    return Matcher<AnyOf,std::tuple<Matcher<PolicyA,TA>,Matcher<PolicyB,TB>>>(std::make_tuple(matcherA, matcherB));
}

struct AllOf {
protected:
    template<class PolicyA, class PolicyB, class TA, class TB, class ActualType>
    bool matches(std::tuple<Matcher<PolicyA,TA>,Matcher<PolicyB,TB>> const& matchers, ActualType const& actual) const {
        return std::get<0>(matchers).matches(actual) && std::get<1>(matchers).matches(actual);
    }

    template<class PolicyA, class PolicyB, class TA, class TB>
    void describe(std::ostream& o, std::tuple<Matcher<PolicyA,TA>,Matcher<PolicyB,TB>> const& matchers) const {
        o << "all of " << std::get<0>(matchers) << " and " <<  std::get<1>(matchers);  
    }
};

template<class PolicyA, class PolicyB, class TA, class TB>
Matcher<AllOf,std::tuple<Matcher<PolicyA,TA>,Matcher<PolicyB,TB>>> 
allOf(Matcher<PolicyA,TA> const& matcherA, Matcher<PolicyB,TB> const& matcherB) {
    return Matcher<AllOf,std::tuple<Matcher<PolicyA,TA>,Matcher<PolicyB,TB>>>(std::make_tuple(matcherA, matcherB));
}

} // namespace matcha

#endif // _MATCHA_H_
