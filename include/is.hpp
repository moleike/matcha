#ifndef _MATCHA_IS_H_
#define _MATCHA_IS_H_

#include <memory>
#include "matcher.hpp"

namespace matcha {

/**
 * Decorates another Matcher, retaining the behaviour but allowing tests
 * to be slightly more expressive.
 *
 * For example:  assertThat(cheese, equalTo(smelly))
 *          vs.  assertThat(cheese, is(equalTo(smelly)))
 */

template<typename T>
class Is : public Matcher<T> {
public:
    Is(const Matcher<T>& matcher) : matcher_(matcher) { }
    virtual bool matches(const T& arg) const {
        return matcher_.matches(arg); 
    }
    virtual void describe(std::ostream& o) const
    {
        o << "is ";
        matcher_.describe(o);
    }

private:
    Matcher<T> const& matcher_;
};

template<typename T>
Is<T> is(Matcher<T> const& operand) {
    return Is<T>(operand);
}

} // namespace matcha

#endif // _MATCHA_IS_H_
