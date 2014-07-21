#ifndef _MATCHA_ISEQUAL_H_
#define _MATCHA_ISEQUAL_H_

#include <iostream>
#include <string>
#include <iterator>
#include <functional>
#include <vector>
#include <set>
#include <map>
#include <type_traits>
#include "prettyprint.hpp"
#include "matcher.hpp"

namespace matcha {

template<typename T>
class IsEqual : public Matcher<T> {
public:
    IsEqual(T const& equalArg) : expectedValue(equalArg) { }

    virtual bool matches(const T& actualValue) const
    { 
        return areEqual(actualValue, expectedValue);
    }
    virtual void describe(std::ostream& o) const
    {
        o << expectedValue;
    }
private:
    template<class U = T>
    bool areEqual(U const& actual, U const& expected,
                  typename std::enable_if<
                  !pretty_print::is_container<U>::value >::type* = 0) const
    {
        return actual == expected;
    }

    template<class U = T>
    bool areEqual(U const&actual, U const&expected,
                  typename std::enable_if<
                  pretty_print::has_const_iterator<U>::value>::type* = 0) const
    {
        using namespace std;
        return actual.size() == expected.size() 
            && mismatch(begin(actual), end(actual), begin(expected)).first == end(actual);
    }

    template<class U = T>
    bool areEqual(std::set<U> const&actual, std::set<U> const&expected)
    {
        using namespace std;
        if (actual.size() != expected.size())
            return false;
        set<U> result;
        set_difference(actual.begin(), actual.end(), expected.begin(), expected.end(), 
            inserter(result, begin(result)));
        return result.empty();
    }
private:
    T expectedValue;
};

template<typename T>
IsEqual<T> equalTo(T const& operand) {
    return IsEqual<T>(operand);
}

template<typename T, size_t N>
IsEqual<std::vector<T> > equalTo(T const (&operand)[N]) {
    std::vector<T> wrapper(std::begin(operand), std::end(operand));
    return IsEqual<std::vector<T> >(wrapper);
}

template<size_t N>
IsEqual<std::string> equalTo(char const (&operand)[N]) {
    std::string wrapper(std::begin(operand), std::end(operand));
    return IsEqual<std::string>(wrapper);
}

} // namespace matcha

//    template<class U = T>
//    bool areEqual(U const&actual, U const&expected,
//                  typename std::enable_if<
//                  has_const_iterator<U>::value>::type* = 0)
//    {
//        std::cout << "associative" << std::endl; 
//        using namespace std;
//        if (actual.size() != expected.size())
//            return false;
//
//        cout << actual << endl;
//        cout << expected << endl;
//
//        set<typename U::value_type> s1(begin(actual), end(actual));
//        set<typename U::value_type> s2(begin(expected), end(expected));
//        U result;
//
//        set_difference(s1.begin(), s1.end(), s2.begin(), s2.end(), inserter(result, begin(result)));
//        return result.empty();
//    }

#endif // _MATCHA_ISEQUAL_H_
