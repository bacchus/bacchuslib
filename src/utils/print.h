#pragma once

#include <iostream>
#include <vector>
#include <iomanip>

namespace bacchus {

template<class T>
inline std::ostream& operator <<(std::ostream& os, const std::vector<T>& vec) {
    for (auto val: vec) {
        os << std::setw(6)
           << std::setprecision(3)
           << std::setiosflags(std::ios_base::showpoint)
           << std::setiosflags(std::ios_base::fixed)
           << std::setiosflags(std::ios_base::skipws)
           << val << " ";
    }
    return os << std::endl;
}

} // namespace bacchus
