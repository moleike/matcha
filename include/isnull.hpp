#ifndef _MATCHA_ISN_H_
#define _MATCHA_ISN_H_

#include <utility>
#include "matcher.hpp"

namespace matcha {

class IsNull : public AnyMatcher {
public:
    virtual bool matches(const void*value) const { return value == nullptr; }
    virtual void describe(std::ostream& o) const { o << "null pointer"; }
};

IsNull null() {
    return IsNull();
}

} // namespace matcha

#endif // _MATCHA_IS_H_

