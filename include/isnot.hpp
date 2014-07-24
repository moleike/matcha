#ifndef _MATCHA_ISNOT_H_
#define _MATCHA_ISNOT_H_

#include "matcher.hpp"

namespace matcha {

/**
 * Calculates the logical negation of a matcher.
 */

template<typename T>
class IsNot : public Matcher<T> {
public:
    IsNot(const Matcher<T>& matcher) : matcher_(matcher) { }
    bool matches(const T& arg) const {
        return !matcher_.matches(arg); 
    }
    virtual void describe(std::ostream& o) const
    {
        o << "not ";
        matcher_.describe(o);
    }
private:
    Matcher<T> const& matcher_;
};

template<typename T>
class IsNot<T*> : public Matcher<T*> {
public:
    IsNot(const Matcher<T*>& matcher) : matcher_(matcher) { }
    bool matches(const T* arg) const {
        return !matcher_.matches(arg); 
    }
    virtual void describe(std::ostream& o) const
    {
        o << "not ";
        matcher_.describe(o);
    }
private:
    Matcher<T*> const& matcher_;
};

/**
 * Because 'not' is a C++ keyword and an alias for the ! operator
 * we need to define the 'not' free function as the operator!
 */
template<typename T>
IsNot<T> operator!(Matcher<T> const& operand) {
    return IsNot<T>(operand);
}

template<typename T>
IsNot<T*> operator!(Matcher<T*> const& operand) {
    return IsNot<T*>(operand);
}

} // namespace matcha

#endif // _MATCHA_ISNOT_H_

