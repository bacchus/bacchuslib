#pragma once

#include <iostream>
#include <iomanip>
#include <vector>

#include "mdefs.h"
#include "utils/logger.h"

namespace bacchus {

template<typename T>
void combsort(std::vector<T>& a) {
    int n = 0;
    float gap = a.size();
    bool ok = true;
    while (ok) {
        gap = std::floor(gap/1.24733f);
        if (gap < 1.f) gap = 1.f;
        ok = (gap!=1.f);
        for (int i = 0; i < a.size()-(int)gap; ++i) {
            ++n;
            int j = i + (int)gap;
            if (a[i] > a[j]) {
                std::swap(a[i], a[j]);
                ok = true;
            }
        }
    }
    std::cout<<"combsort complexity: "<<n<<" ("<<a.size()<<")"<<std::endl;
}

//template<typename T>
//inline int choose_pivot(T*, int l, int) {
//    return l;
//}

template<typename T>
inline int partition(std::vector<T>& a, int l, int r) {
//    int pi = choose_pivot(a,l,r);
//    std::swap(a[pi], a[l]);
    T p = a[l];
    int i = l+1;
    for (int j = l+1; j < r; ++j) {
        if (a[j] < p) {
            std::swap(a[j], a[i]);
            ++i;
        }
    }
    std::swap(a[l], a[i-1]);
    return i-1;
}

template<typename T>
inline void quick_sort(std::vector<T>& a, int l, int r) {
    if ((r-l)<=1)
        return;
    int i = partition(a,l,r);
    quick_sort(a,l,i);
    quick_sort(a,i+1,r);
}

template<typename T>
inline void quick_sort(T* a, int n) {
    if (n<=1)
        return;
    quick_sort(a,0,n);
}

template<typename T>
inline T* merge_sort(const T* a, int l, int r) {
    T* res = new T[r-l];
    if ((r-l)==1) {
        res[0] = a;
        return res;
    }
    T* left = merge_sort(a, l, (l+r)/2);
    T* right = merge_sort(a, 1+(l+r)/2, r);
    int i = 0; int j = 0;
    for (int k = 0; k < r-l; ++k) {
        if (left[i] < right[j]) {
            res[k] = left[i];
            ++i;
        } else {
            res[k] = right[j];
            ++j;
        }
    }
    delete[] left;
    delete[] right;
    return res;
}

template<typename T>
inline T find_median(T* a, int n) {
    int k = n/2;
    for (int i = 0; i < k+1; ++i) {
        int min_i = i;
        T min_v = a[i];
        for (int j = i+1; j < n; ++j) {
            if (a[j] < min_v) {
                min_i = j;
                min_v = a[j];
            }
        }
        a[min_i] = a[i];
        a[i] = min_v;
        //std::cout<<i<<": "<<a[i]<<std::endl;
    }
    return a[k];
}

inline uint bcc_rev_bits(uint x, uint n) {
    uint res = 0;
    for (uint i = 0; i < n; ++i) {
        res = (res<<1) | (x&1);
        x >>= 1;
    }
    return res;
}

//template <typename T>
//std::vector<int> argsort(const std::vector<T>& v) {
//    std::vector<int> idx(v.size());
//    std::iota(idx.begin(), idx.end(), 0);
//    std::sort(idx.begin(), idx.end(), [&v](int i1, int i2) { return v[i1] < v[i2]; });
//    return idx;
//}

} // namespace bacchus
