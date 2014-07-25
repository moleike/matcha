#ifndef _MATCHA_MATCHER_H_
#define _MATCHA_MATCHER_H_

#include <iostream>

namespace matcha {

template<typename T>
class Matcher {
public:
    virtual bool matches(const T&value) const = 0;
    virtual void describe(std::ostream& o) const = 0;
    friend std::ostream& operator << (std::ostream& o, const Matcher<T>& m) {
        m.describe(o);
        return o; 
    }
};

template<typename T>
class Matcher<T*> {
public:
    virtual bool matches(const T*value) const = 0;
    virtual void describe(std::ostream& o) const = 0;
    friend std::ostream& operator << (std::ostream& o, const Matcher<T*>& m) {
        m.describe(o);
        return o; 
    }
};

class AnyMatcher : public Matcher<void*> {
public:
    virtual bool matches(const void*value) const = 0;
    virtual void describe(std::ostream& o) const = 0;
};

} // namespace matcha

#endif // _MATCHA_MATCHER_H_
