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
#include <array>
#include <cstring>
#include <cctype>
#include <type_traits>
#include <regex>
#include "prettyprint.hpp"

#if defined(MATCHA_GTEST)
#include "gtest/gtest.h"

/* we need the assertThat logic in a macro, so that
 * the ADD_FAILURE will report the right __FILE__ and __LINE__
 */
#define assertThat(actual,matcher)  \
    ASSERT_PRED_FORMAT2(assertResult, actual, matcher)

#elif defined(MATCHA_BOOSTTEST)
#include <boost/test/included/unit_test.hpp>

/* we override default behaviour (stringying the assertion) and
 * provide an empty string here, and the matcher description in the result
 */
#define assertThat(actual,matcher)  \
  BOOST_CHECK_MESSAGE               \
    (assertResult<boost::test_tools::predicate_result>(actual, matcher), "")

#else

#define assertThat(actual, matcher) \
    assertResult<bool>(actual, matcher)

#endif

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

// SFINAE type trait to detect whether type T satisfies LessThanComparable.

template<typename T, typename = void>
struct is_lessthan_comparable : std::false_type
{ };

template<typename T>
struct is_lessthan_comparable<T,
    typename std::enable_if<
        true,
        decltype((std::declval<T>() < std::declval<T>()), (void)0)
        >::type
    > : std::true_type
{ };

// SFINAE type trait to detect whether a class is a specialization of Template

template <template <typename...> class Template, typename T>
struct is_specialization_of : std::false_type
{ };

template <template <typename...> class Template, typename... Args>
struct is_specialization_of<Template, Template<Args...>> : std::true_type
{ };

// SFINAE type trait to detect whether T and all of the Rest name the same type.

template<typename T, typename... Rest>
struct is_same : std::false_type
{ };

template<typename T, typename First>
struct is_same<T, First> : std::is_same<T, First>
{ };

template<typename T, typename First, typename... Rest>
struct is_same<T, First, Rest...>
    : std::integral_constant<
        bool,
        std::is_same<T, First>::value && is_same<T, Rest...>::value
      >
{ };

// templated operator<< when type T is not std container (using prettyprint)
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

/*
 * character traits to provide case-insensitive comparison
 * http://www.gotw.ca/gotw/029.htm
 */
struct ci_char_traits : public std::char_traits<char> {
    static bool eq(char c1, char c2) {
         return std::toupper(c1) == std::toupper(c2);
     }
    static bool lt(char c1, char c2) {
         return std::toupper(c1) <  std::toupper(c2);
    }
    static int compare(const char* s1, const char* s2, size_t n) {
        while (n-- != 0) {
            if (std::toupper(*s1) < std::toupper(*s2))
                return -1;
            if (std::toupper(*s1) > std::toupper(*s2))
                return 1;
            ++s1; ++s2;
        }
        return 0;
    }
    static const char* find(const char* s, int n, char a) {
        auto const ua (std::toupper(a));
        while (n-- != 0) {
            if (std::toupper(*s) == ua)
                return s;
            s++;
        }
        return nullptr;
    }
};

// case-insensitive string class
typedef std::basic_string<char, ci_char_traits> ci_string;

std::ostream& operator<<(std::ostream& os, const ci_string& str)
{
    return os.write(str.data(), str.size());
}

template <typename T>
std::string to_string(T const& val)
{
    std::ostringstream out;
    out << val;
    return out.str();
}

template<typename T>
struct output_traits;

template<>
struct output_traits<bool>
{
    typedef bool result_type;
    static constexpr bool success = true;
    static constexpr bool failure = false;

    static std::ostream & ostream(bool & result) {
        return std::cout;
    }
};

#if defined(MATCHA_GTEST)

template<>
struct output_traits<::testing::AssertionResult>
{
    typedef ::testing::AssertionResult result_type;

    static ::testing::AssertionResult success() {
        return ::testing::AssertionSuccess();
    }

    static ::testing::AssertionResult failure() {
        return ::testing::AssertionFailure();
    }

    static ::testing::AssertionResult & 
    ostream(::testing::AssertionResult &result) {
        return result;
    }
};

template<class T, class Matcher, class Return>
::testing::AssertionResult
assertResult(const char *, const char *, T const& actual, Matcher const& matcher) {
    return assertResult<::testing::AssertionResult>(actual, matcher);
}

#elif defined(MATCHA_BOOSTTEST)

template<>
struct output_traits<boost::test_tools::predicate_result>
{
    typedef boost::test_tools::predicate_result result_type;

    static bool success() { 
        return true; 
    }

    static boost::test_tools::predicate_result failure() { 
        return false; 
    }

    static decltype(std::declval<boost::test_tools::predicate_result>().message()) & 
    ostream(boost::test_tools::predicate_result & result) {
        return result.message();
    }
};

#endif

template<class Result, class T, class Matcher>
typename output_traits<Result>::result_type
assertResult(T const& actual, Matcher const& matcher) {
    Result result = output_traits<Result>::failure();

    if (matcher.matches(actual))
        return output_traits<Result>::success();

    output_traits<Result>::ostream(result)    << '\n'
        << "Expected: " << to_string(matcher) << '\n'
        << "but got : " << to_string(actual)  << '\n'; 

    return result;
}

template<class MatcherPolicy, class ExpectedType = void>
class Matcher : public MatcherPolicy {
public:
    Matcher(ExpectedType const& value = ExpectedType()) : expected_(value)
    { }

    template<class ActualType>
    bool matches(ActualType const& actual) const {
        return MatcherPolicy::matches(expected_, actual);
    }

    template<size_t M>
    bool matches(char const (&actual)[M]) const {
        return MatcherPolicy::matches(expected_, std::string(actual));
    }

    friend std::ostream& operator<<(std::ostream& o, Matcher const& matcher) {
        matcher.describe(o, matcher.expected_);
        return o;
    }
private:
    ExpectedType expected_;
};

// there is no expected value, for matcher not taking input parameters
template<class MatcherPolicy>
class Matcher<MatcherPolicy,void> : public MatcherPolicy {
public:
    template<class ActualType>
    bool matches(ActualType const& actual) const {
        return MatcherPolicy::matches(actual);
    }

    friend std::ostream& operator<<(std::ostream& o, Matcher const& matcher) {
        matcher.describe(o);
        return o;
    }
};

// C-style arrays and strings
template<class MatcherPolicy, class ExpectedType, size_t N>
class Matcher<MatcherPolicy,ExpectedType[N]> : public MatcherPolicy
{
public:
    Matcher(ExpectedType const (&value)[N]) : expected_(value)
    { }

    template<size_t M>
    bool matches(ExpectedType const (&actual)[M]) const {
        std::vector<ExpectedType> actual_vec(actual, actual + M), expected_vec(expected_, expected_ + N);
        return MatcherPolicy::matches(expected_vec, actual_vec);
    }
 
    bool matches(std::string const& actual) const {
        return MatcherPolicy::matches(std::string(expected_), actual);
    }

    friend std::ostream& operator<<(std::ostream& o, Matcher const& matcher) {
        matcher.describe(o, matcher.expected_);
        return o;
    }
private:
    ExpectedType const (&expected_)[N];
};

// this is just sugar to make it easy to create a matcher
// still preferred to use a free function though

template<class MatcherPolicy>
struct MatcherGenerator {

    template<typename... T>
    Matcher<MatcherPolicy,T...> operator()(T const& ...value) const {
        return Matcher<MatcherPolicy,T...>(value...); 
    }

    template<typename T, size_t N>
    Matcher<MatcherPolicy,T[N]> operator()(T const (&value)[N]) const {
        return Matcher<MatcherPolicy,T[N]>(value);
    }
};

template<class MatcherPolicy>
MatcherGenerator<MatcherPolicy> make_matcher() {
    return MatcherGenerator<MatcherPolicy>();
}




// SFINAE type trait to detect whether one or more classes are matchers

template<typename... Ts>
struct is_matcher : std::false_type 
{ };

template<typename T>
struct is_matcher<T> : is_specialization_of<Matcher,T> 
{ };

template<typename First, typename... Rest>
struct is_matcher<First, Rest...>
    : std::integral_constant<
        bool,
        is_matcher<First>::value && is_matcher<Rest...>::value
      >
{ };

struct Is {
protected:
    template<typename MatcherType, typename ActualType>
    bool matches(MatcherType const& expected, ActualType const& actual) const {
        static_assert(is_matcher<MatcherType>::value, "IsNot matcher requires a Matcher parameter");
        return expected.matches(actual);
    }

    template<typename MatcherType>
    void describe(std::ostream& o, MatcherType const& expected) const {
        o << "is " << expected;
    }
};

auto is = make_matcher<Is>();

struct IsNot_ {
protected:
    template<typename MatcherType, typename ActualType>
    bool matches(MatcherType const& expected, ActualType const& actual) const {
        return !expected.matches(actual);
    }

    template<typename MatcherType>
    void describe(std::ostream& o, MatcherType const& expected) const {
        o << "not " << expected;
    }
};

template<class T>
using IsNot = Matcher<IsNot_,T>;


template<class T>
constexpr IsNot<T> operator!(T const& value) {
    static_assert(is_matcher<T>::value, "IsNot matcher requires a Matcher parameter");
    return IsNot<T>(value);
}

struct IsNull {
protected:
    template<typename T>
    bool matches(T const* actual) const {
        return actual == nullptr;
    }

    void describe(std::ostream& o) const {
        o << "null pointer";
    }
};

auto null = make_matcher<IsNull>();


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

auto equalTo = make_matcher<IsEqual>();

struct IsContaining_ {
protected:
    template<typename C, typename T,
         typename std::enable_if<std::is_same<typename C::value_type,T>::value>::type* = nullptr>
    bool matches(T const& item, C const& cont) const {
        return std::end(cont) != std::find(std::begin(cont), std::end(cont), item);
    }

    template<typename T, size_t N>
    bool matches(T const& item, T const (&array)[N]) const {
        return std::end(array) != std::find(std::begin(array), std::end(array), item);
    }

    template<typename C = std::string, typename T = std::string>
    bool matches(std::string const& substr, std::string const& actual) const {
        return std::string::npos != actual.find(substr);
    }

    // overload for checking whether container values match a predicate specified by a Matcher
    template<typename C, typename T, typename Policy,
         typename std::enable_if<std::is_same<typename C::value_type,T>::value>::type* = nullptr>
    bool matches(Matcher<Policy,T> const& itemMatcher, C const& cont) const {
        auto pred = std::bind(&Matcher<Policy,T>::template matches<T>, &itemMatcher, std::placeholders::_1);
        return std::all_of(std::begin(cont), std::end(cont), pred);
    }

    template<typename T>
    void describe(std::ostream& o, T const& expected) const {
       o << "contains " << expected;
    }

    template<typename T, typename Policy>
    void describe(std::ostream& o, Matcher<Policy,T> const& expected) const {
       o << "every item " << expected;
    }
};

template<>
void IsContaining_::describe(std::ostream& o, std::string const& expected) const {
   o << "contains " << "\"" << expected << "\"";
}

template<class T>
using IsContaining = Matcher<IsContaining_,T>;

template<typename T>
constexpr IsContaining<T> contains(T const& value) {
    return IsContaining<T>(value);
}

template<typename T, size_t N>
constexpr IsContaining<T[N]> contains(T const (&value)[N]) {
    return IsContaining<T[N]>(value);
}

template<class Key, class T>
constexpr IsContaining<std::pair<const Key,T>> contains(Key const& key, T const& value) {
    return IsContaining<std::pair<const Key,T>>(std::pair<const Key,T>(key, value));
}

template<typename T, typename Policy>
constexpr IsContaining<Matcher<Policy,T>> everyItem(Matcher<Policy,T> const& itemMatcher) {
    return IsContaining<Matcher<Policy,T>>(itemMatcher);
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

auto hasKey = make_matcher<IsContainingKey>();

struct IsIn_ {
protected:
    template<typename C, typename T,
         typename std::enable_if<std::is_same<typename C::value_type,T>::value>::type* = nullptr>
    bool matches(C const& cont, T const& item) const {
        return std::end(cont) != std::find(std::begin(cont), std::end(cont), item);
    }

    template<typename T, size_t N>
    bool matches(T const (&array)[N], T const& item) const {
        return std::end(array) != std::find(std::begin(array), std::end(array), item);
    }

    template<typename C>
    void describe(std::ostream& o, C const& expected) const {
       o << "one of " << expected;
    }

};

template<typename C>
using IsIn = Matcher<IsIn_,C>;

template<typename C>
constexpr IsIn<C> in(C const& cont) {
    return IsIn<C>(cont);
}

template<typename T, typename... Args>
IsIn<std::vector<T>> oneOf(T const& first, Args const& ... args) {
    std::vector<T> cont{first, args...};
    return IsIn<std::vector<T>>(cont);
}

template<size_t M, size_t... N>
IsIn<std::vector<std::string>> oneOf(const char (&first)[M], const char (&...args)[N]) {
    std::vector<std::string> cont{first, args...};
    return IsIn<std::vector<std::string>>(cont);
}

struct IsEmpty_ {
protected:
    template<typename C>
    bool matches(C const& actual) const {
        static_assert(pretty_print::is_container<C>::value, "empty matcher is for std containers");
        return actual.empty();
    }

    void describe(std::ostream& o) const {
        o << "an empty container";
    }
};

using IsEmpty = Matcher<IsEmpty_>;

constexpr IsEmpty empty() {
    return IsEmpty();
}

struct IsEmptyString_ {
protected:
    bool matches(std::string const& actual) const {
        return actual.empty();
    }

    void describe(std::ostream& o) const {
        o << "an empty string";
    }
};

using IsEmptyString = Matcher<IsEmptyString_>;

constexpr IsEmptyString emptyString() {
    return IsEmptyString();
}

struct IsEqualIgnoringCase_ {
protected:
    bool matches(std::string const& expected, std::string const& actual) const {
        ci_string ci_exp, ci_act;

        ci_exp.assign(expected.begin(), expected.end());
        ci_act.assign(actual.begin(), actual.end());
        return ci_exp == ci_act;
    }

    void describe(std::ostream& o, std::string const& expected) const {
       o << "Equal to " << "\"" << expected << "\"" << " ignoring case";
    }
};

using IsEqualIgnoringCase = Matcher<IsEqualIgnoringCase_,std::string>;

IsEqualIgnoringCase equalToIgnoringCase(std::string const& val) {
    return IsEqualIgnoringCase(val);
}

struct IsEqualIgnoringWhiteSpace_ {
protected:
    bool matches(std::string const& expected, std::string const& actual) const {
        std::string exp(expected), act(actual);

        exp.erase(std::remove_if(exp.begin(),
                                 exp.end(),
                                 [](char x){return std::isspace(x);}),
                  exp.end());
        act.erase(std::remove_if(act.begin(),
                                 act.end(),
                                 [](char x){return std::isspace(x);}),
                  act.end());
        return exp == act;
    }

    void describe(std::ostream& o, std::string const& expected) const {
       o << "Equal to " << "\"" << expected << "\"" << " ignoring white space";
    }
};

using IsEqualIgnoringWhiteSpace = Matcher<IsEqualIgnoringWhiteSpace_,std::string>;

IsEqualIgnoringWhiteSpace equalToIgnoringWhiteSpace(std::string const& val) {
    return IsEqualIgnoringWhiteSpace(val);
}

struct StringStartsWith_ {
protected:
    bool matches(std::string const& substr, std::string const& actual) const {
        return !actual.compare(0, substr.size(), substr);
    }

    void describe(std::ostream& o, std::string const& expected) const {
       o << "starts with " << "\"" << expected << "\"";
    }
};

using StringStartsWith = Matcher<StringStartsWith_,std::string>;

StringStartsWith startsWith(std::string const& val) {
    return StringStartsWith(val);
}

struct StringEndsWith_ {
protected:
    bool matches(std::string const& substr, std::string const& actual) const {
        return !actual.compare(actual.size() - substr.size(), substr.size(), substr);
    }

    void describe(std::ostream& o, std::string const& expected) const {
       o << "ends with " << "\"" << expected << "\"";
    }
};

using StringEndsWith = Matcher<StringEndsWith_,std::string>;

StringEndsWith endsWith(std::string const& val) {
    return StringEndsWith(val);
}

struct AnyOf_ {
protected:
    template<class ActualType, std::size_t I = 0, typename... Tp>
    typename std::enable_if<I == sizeof...(Tp), bool>::type
    matches(std::tuple<Tp...> const& t, ActualType const& actual) const {
        return false;
    }

    template<class ActualType, std::size_t I = 0, typename... Tp>
    typename std::enable_if<I < sizeof...(Tp), bool>::type
    matches(std::tuple<Tp...> const& t, ActualType const& actual) const {
        return std::get<I>(t).matches(actual) || matches<ActualType, I + 1, Tp...>(t, actual);
    }

    template<typename... Tp>
    void describe(std::ostream& o, std::tuple<Tp...> const& t) const {
        o << "any of ";
        printall(o, t);
    }

private:
    template<std::size_t I = 0, typename... Tp>
    typename std::enable_if<I == sizeof...(Tp) - 1, void>::type
    printall(std::ostream& o, std::tuple<Tp...> const& t) const {
        o << std::get<I>(t) << ".";
    }

    template<std::size_t I = 0, typename... Tp>
    typename std::enable_if<I < sizeof...(Tp) - 1, void>::type
    printall(std::ostream& o, std::tuple<Tp...> const& t) const {
        o << std::get<I>(t) << " or ";
        printall<I + 1, Tp...>(o, t);
    }
};

template<typename T>
using AnyOf = Matcher<AnyOf_,T>;

template<typename First, typename... Args>
constexpr AnyOf<std::tuple<First,Args...>> anyOf(First first, Args... args)
{
    static_assert(is_matcher<First, Args...>::value, "anyOf requires Matcher parameters");
    return AnyOf<std::tuple<First,Args...>>(std::make_tuple(first, args...));
}

struct AllOf_ {
protected:
    template<class ActualType, std::size_t I = 0, typename... Tp>
    typename std::enable_if<I == sizeof...(Tp), bool>::type
    matches(std::tuple<Tp...> const& t, ActualType const& actual) const {
        return true;
    }

    template<class ActualType, std::size_t I = 0, typename... Tp>
    typename std::enable_if<I < sizeof...(Tp), bool>::type
    matches(std::tuple<Tp...> const& t, ActualType const& actual) const {
        return std::get<I>(t).matches(actual) && matches<ActualType, I + 1, Tp...>(t, actual);
    }

    template<typename... Tp>
    void describe(std::ostream& o, std::tuple<Tp...> const& t) const {
        o << "all of ";
        printall(o, t);
    }

private:
    template<std::size_t I = 0, typename... Tp>
    typename std::enable_if<I == sizeof...(Tp) - 1, void>::type
    printall(std::ostream& o, std::tuple<Tp...> const& t) const {
        o << std::get<I>(t) << ".";
    }

    template<std::size_t I = 0, typename... Tp>
    typename std::enable_if<I < sizeof...(Tp) - 1, void>::type
    printall(std::ostream& o, std::tuple<Tp...> const& t) const {
        o << std::get<I>(t) << " and ";
        printall<I + 1, Tp...>(o, t);
    }
};

template<typename T>
using AllOf = Matcher<AllOf_,T>;

template<typename First, typename... Args>
constexpr AllOf<std::tuple<First,Args...>> allOf(First first, Args... args)
{
    static_assert(is_matcher<First, Args...>::value, "allOf requires Matcher parameters");
    return AllOf<std::tuple<First,Args...>>(std::make_tuple(first, args...));
}

struct IsCloseTo_ {
    template<typename T>
    bool matches (std::pair<T,T> const& expected, T const& actual) const {
        T value = expected.first;
        T delta = expected.second;

        return std::fabs(actual - value) <= delta;
    }

    template<typename T>
    void describe(std::ostream& o, std::pair<T,T> const& expected) const {
       o << "a numeric value within +/-" << expected.second
         << " of " << expected.first;
    }
};

template<typename T>
using IsCloseTo = Matcher<IsCloseTo_,T>;

template<typename T>
constexpr IsCloseTo<std::pair<T,T>> closeTo(T const& operand, T const& error) {
    static_assert(std::is_floating_point<T>::value, 
                  "closeTo parameters need be floating-point type");
    return IsCloseTo<std::pair<T,T>>(std::make_pair(operand, error));
}


struct MatchesPattern_ {
    bool matches(std::string const& reg, std::string const& actual) const {
        return std::regex_match(actual, std::regex(reg));
    }

    void describe(std::ostream& o, std::string const& expected) const {
       o << "a string matching the pattern " << expected;
    }
};

using MatchesPattern = Matcher<MatchesPattern_,std::string>;

MatchesPattern matchesPattern(std::string const& reg_exp) {
    return MatchesPattern(reg_exp);
}

MatchesPattern matches(std::string const& reg_exp) {
    return MatchesPattern(reg_exp);
}

template<typename F>
struct OrderingComparison {
protected:
    template<typename T>
    bool matches(T const& expected, T const& actual) const {
        F op = F();
        return op(actual, expected);
    }
};

template<typename T>
struct LessThan : OrderingComparison<std::less<T>> {
protected:
    void describe(std::ostream& o, T const& expected) const {
        o << "less than " << expected;
    }
};

template<typename T>
Matcher<LessThan<T>,T> lessThan(T const& value) {
    return Matcher<LessThan<T>,T>(value);
}

template<typename T>
struct GreaterThan : OrderingComparison<std::greater<T>> {
protected:
    void describe(std::ostream& o, T const& expected) const {
        o << "greater than " << expected;
    }
};

template<typename T>
Matcher<GreaterThan<T>,T> greaterThan(T const& value) {
    return Matcher<GreaterThan<T>,T>(value);
}

template<typename T>
struct GreaterThanOrEqual : OrderingComparison<std::greater_equal<T>> {
protected:
    void describe(std::ostream& o, T const& expected) const {
        o << "greater than or equal to " << expected;
    }
};

template<typename T>
Matcher<GreaterThanOrEqual<T>,T> greaterThanOrEqualTo(T const& value) {
    return Matcher<GreaterThanOrEqual<T>,T>(value);
}

template<typename T>
struct LessThanOrEqual : OrderingComparison<std::less_equal<T>> {
protected:
    void describe(std::ostream& o, T const& expected) const {
        o << "less than or equal to " << expected;
    }
};

template<typename T>
Matcher<LessThanOrEqual<T>,T> lessThanOrEqualTo(T const& value) {
    return Matcher<LessThanOrEqual<T>,T>(value);
}

} // namespace matcha

#endif // _MATCHA_H_
