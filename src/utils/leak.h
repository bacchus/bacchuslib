#pragma once

#include <sstream>

namespace bacchus {

class Leak {
    static int n;

    Leak(Leak&&);
    Leak& operator =(const Leak&);
    Leak& operator =(Leak&&);
public:
    Leak() { ++n; }
    Leak(const Leak&) { ++n; }
    ~Leak() { --n; }
    static int num() { return n; }
    static void zero() { n = 0; }
};

inline std::ostream& operator<<(std::ostream& os, const Leak&) { return os; }

} // namespace bacchus
