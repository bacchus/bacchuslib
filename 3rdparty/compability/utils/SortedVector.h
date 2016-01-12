#ifndef SORTEDVECTOR_H
#define SORTEDVECTOR_H

#include <vector>
#include <algorithm>

template <class Val>
class SortedVector {
public:
    inline void remove(const Val& val) {
        typename std::vector<Val>::iterator it = std::find(mVec.begin(), mVec.end(), val);
        if (it != mVec.end())
            mVec.erase(it);
    }

    inline void add(const Val& val) {
        mVec.push_back(val);
    }

    inline std::size_t size() {
        return mVec.size();
    }

    inline void clear() {
        mVec.clear();
    }

    inline Val operator[](int i) {
        return mVec[i];
    }

private:
    std::vector<Val> mVec;
};

#endif // SORTEDVECTOR_H

