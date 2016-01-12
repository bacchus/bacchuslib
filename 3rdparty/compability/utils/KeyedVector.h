#ifndef KEYEDVECTOR_H
#define KEYEDVECTOR_H

#include <map>

template <class Key, class Val>
class KeyedVector {
public:
    inline int indexOfKey(const Key& key) {
        int index = 0;
        typename std::map<Key, Val>::iterator it = mMap.find(key);
        if (it != mMap.end()) {
            for (typename std::map<Key, Val>::iterator test = mMap.begin(); test != it; ++test) {
                ++index;
            }
        } else {
            index = -1;
        }
        return index;
    }

    inline void add(const Key& key, const Val& val) {
        mMap[key]= val;
    }

    inline std::size_t size() {
        return mMap.size();
    }

    inline void clear() {
        mMap.clear();
    }

    inline Val valueAt(int index) {
        typename std::map<Key, Val>::iterator it = mMap.begin();
        for (int i=0; i<index; ++i) {
            ++it;
        }
        return it->second;
    }

private:
    std::map<Key, Val> mMap;
};

#endif // KEYEDVECTOR_H

