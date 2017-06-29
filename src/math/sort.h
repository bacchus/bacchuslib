#pragma once

#include <iostream>
#include <iomanip>
#include <vector>

#include "mdefs.h"
#include "utils/logger.h"

namespace bacchus {

//====================================================================
template<typename T>
inline bool test_sort(const std::vector<T>& a) {
    for (int i = 1; i < (int)a.size(); ++i) {
        if (a[i] < a[i-1])
            return false;
    }
    return true;
}

//====================================================================
template<typename T>
inline void insert_sort(std::vector<T>& a) {
    for (int j = 1; j < (int)a.size(); ++j) {
        const T key = a[j];
        int i = j - 1;
        // bin-search-insert can be used here
        while (i >= 0 && a[i] > key) {
            a[i+1] = a[i];
            --i;
        }
        a[i+1] = key;
    }
}

//====================================================================
template<typename T>
inline void bubble_sort(std::vector<T>& a) {
    for (int j = 1; j < (int)a.size(); ++j) {
        for (int i = j-1; i >=0; --i) {
            if (a[i] > a[i+1])
                std::swap(a[i], a[i+1]);
        }
    }
}

//====================================================================
/// inplace realisation
/// |p| | |q| | |r|
/// |0|1|2|3|4|5| |
template<typename T>
inline void merge_sort(std::vector<T>& a, int l, int r) {

    // condition
    if (l + 1 < r) {

        // pivot
        int q = (l+r)/2;

        // divide
        merge_sort(a, l, q);
        merge_sort(a, q, r);

        // merge
        std::vector<T> res(r-l);
        int i = l; int j = q;
        T ai = a[i];
        T aj = a[j];
        for (int k = 0; k < r-l; ++k) {
            if (ai < aj) {
                res[k] = ai;
                ++i;
                ai = (i<q ? a[i] : a[r-1]);
            } else {
                res[k] = aj;
                ++j;
                aj = (j<r ? a[j] : a[q-1]);
            }
        }

        // inplace
        for (int k = 0; k < r-l; ++k) {
            a[l+k] = res[k];
        }

    }
}

//====================================================================
template<typename T>
inline void merge_sort(std::vector<T>& a) {
    merge_sort(a, 0, a.size());
}

//====================================================================
namespace heap {

inline int parent(int i) { return (i-1)/2; }
inline int letf(int i) { return 2*i+1; }
inline int right(int i) { return 2*i+2; }

template<typename T>
inline void max_heapify(std::vector<T>& a, int heap_size, int i) {
    const int l = letf(i);
    const int r = right(i);
    int largest = i;

    if (l<heap_size && a[l]>a[i])
        largest = l;
    if (r<heap_size && a[r]>a[largest])
        largest = r;

    if (largest != i) {
        std::swap(a[i], a[largest]);
        max_heapify(a, heap_size, largest);
    }
}

template<typename T>
inline void build_max_heap(std::vector<T>& a) {
    for (int i = a.size()/2 - 1; i >= 0; --i)
        max_heapify(a, a.size(), i);
}

template<typename T>
inline T pop_max(std::vector<T>& a) {
    assert(a.size());
    const T max = a[0];
    a[0] = a[a.size()-1];
    a.resize(a.size()-1);
    max_heapify(a, a.size(), 0);
    return max;
}

template<typename T>
inline void inc_key(std::vector<T>& a, int i, T key) {
    assert(key > a[i]);
    a[i] = key;
    while (i > 0 && a[parent(i)] < a[i]) {
        std::swap(a[i], a[parent(i)]);
        i = parent(i);
    }
}

template<typename T>
inline void insert_key(std::vector<T>& a, T key) {
    a.push_back(key-1);
    inc_key(a, a.size(), key);
}

} // namespace heap

template<typename T>
void heap_sort(std::vector<T>& a) {
    heap::build_max_heap(a);
    int heap_size = a.size();
    for (int i = a.size()-1; i >= 1; --i) {
        std::swap(a[0], a[i]);
        heap::max_heapify(a, --heap_size, 0);
    }
}

//====================================================================
template<typename T>
inline int choose_pivot(std::vector<T>&, int l, int r) {
    return l + rnd(r-l);
}

template<typename T>
inline int partition(std::vector<T>& a, int l, int r) {
    int pi = choose_pivot(a,l,r);
    std::swap(a[l], a[pi]);

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
    int p = partition(a,l,r);
    quick_sort(a,l,p);
    quick_sort(a,p+1,r);
}

template<typename T>
inline void quick_sort(std::vector<T>& a) {
    if (a.size() > 1) {
        rndmz();
        quick_sort(a,0,a.size());
    }
}

template<typename T>
inline T quick_select(std::vector<T> a, int l, int r, int i) {
    if ((r-l)<=1)
        return a[l];

    const int p = partition(a,l,r);
    const int k = p - l;
    if (i == k)
        return a[p];
    else if (i < k)
        return quick_select(a,l,p-1,i);
    else
        return quick_select(a,p+1,r,i-k);
}
//====================================================================
inline void count_sort(std::vector<int>& b, const std::vector<int>& a, int k) {
    std::vector<int> c(k,0);
    b.resize(a.size());
    for (int i = 0; i < (int)a.size(); ++i)
        ++c[a[i]];
    for (int i = 1; i < (int)c.size(); ++i)
        c[i] += c[i-1];
    for (int i = a.size()-1; i >= 0; --i) {
        b[c[a[i]]-1] = a[i];
        --c[a[i]];
    }
}

//inline void radix_sort(std::vector<int>& a, int d) {
//    for (int i = 0; i < d; ++i) {
//        count_sort(b,a by i bit,k);
//    }
//}

//====================================================================
/// 0.0<=a[i]<=1.0; equaly distributed
inline void bucket_sort(std::vector<float>& b, const std::vector<float>& a) {
    const int n = (int)a.size();
    std::vector< std::vector<float> > buk(n);
    b.reserve(n);

    for (int i = 0; i < n; ++i)
        buk[(int)(n*a[i])].push_back(a[i]);

    for (int i = n-1; i >= 0; --i)
        insert_sort(buk[i]);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < (int)buk[i].size(); ++j) {
            b.push_back(buk[i][j]);
        }
    }
}

//====================================================================
template<typename T>
inline void combsort(std::vector<T>& a) {
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

//====================================================================
template<typename T>
inline void find_minmax(T& min_val, T& max_val, const std::vector<T>& a) {
    const int n = a.size();
    const int ev = 1 - n%2;

    min_val = std::min(a[n-1], a[n-1-ev]);
    max_val = std::max(a[n-1], a[n-1-ev]);

    for (int i = 0; i < (n-1)/2; ++i) {
        const int j = a[2*i] < a[2*i+1] ? 0 : 1;

        const T& min_ai = a[2*i+j];
        const T& max_ai = a[2*i+1-j];

        max_val = max_ai > max_val ? max_ai : max_val;
        min_val = min_ai < min_val ? min_ai : min_val;
    }
}

//====================================================================
/// BCC: not tested
namespace qs { // deterministic quick select

template<typename T>
inline void qs_insert_sort(std::vector<T>& a, int l, int r) {
    for (int j = l+1; j < r; ++j) {
        const T key = a[j];
        int i = j - 1;
        while (i >= l && a[i] > key) {
            a[i+1] = a[i];
            --i;
        }
        a[i+1] = key;
    }
}

template<typename T>
inline T qs_median_of_median(std::vector<T>& a) {
    const int n = (int)a.size();

    // if 1 element return it
    if (n==1)
        return a[0];

    std::vector<T> medians;
    medians.reserve(n/5+1);

    // divide for 5 elements in group
    for (int i = 0; i < n/5; ++i) {
        qs_insert_sort(a, 5*i, 5*i+5);
        medians.push_back(a[5*i+2]);
    }

    // last n mod 5 elements
    const int ln = n%5;
    if (ln) {
        qs_insert_sort(a, n - ln, n);
        medians.push_back(a[n - ln/2 - 1]);// lower median
    }

    return qs_median_of_median(medians);
}

template<typename T>
inline int qs_partition(std::vector<T>& a, int l, int r, const T& p) {
    int i = l;
    for (int j = l; j < r; ++j) {
        if (a[j] < p) {
            std::swap(a[j], a[i]);
            ++i;
        }
    }

    return i-1;
}

template<typename T>
inline T qs_quick_select(std::vector<T>& a, int l, int r, int i) {
    if ((r-l)<=1)
        return a[l];

    T m = qs_median_of_median(a);
    const int p = qs_partition(a,l,r,m);
    const int k = p - l;
    if (i == k)
        return a[p];
    else if (i < k)
        return qs_quick_select(a,l,p-1,i);
    else
        return qs_quick_select(a,p+1,r,i-k);
}

}


//====================================================================
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
