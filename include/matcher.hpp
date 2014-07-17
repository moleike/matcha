#ifndef _MATCHA_MATCHER_H_
#define _MATCHA_MATCHER_H_

namespace matcha {

template<typename T>
class Matcher {
public:
    virtual bool matches(const T&value) const = 0;
    template <typename U = T>
    friend std::ostream& operator << (std::ostream& o, const Matcher<U>& m) {
        m.describe(o);
        return o; 
    }
    virtual void describe(std::ostream& o) const = 0;
};

} // namespace matcha

#endif // _MATCHA_MATCHER_H_
