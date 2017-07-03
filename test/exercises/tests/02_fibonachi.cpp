#include "setting.h"

#if TEST_02_FIBONA

typedef unsigned long long ull;
const ull FIB_MOD = 1000000UL;
//====================================================================
ull fib_simple_rec(int n) {
    if (n < 3)
        return 1;

    return (fib_simple_rec(n-1) + fib_simple_rec(n-2)) % FIB_MOD;
}

//====================================================================
ull fib_rem_rec_aux(int n, std::vector<ull>& cache) {
    if (n < 3)
        return 1;

    if (cache[n] == 0) {
        cache[n] = fib_rem_rec_aux(n-1, cache) + fib_rem_rec_aux(n-2, cache);
        cache[n] %= FIB_MOD;
    }

    return cache[n];
}

ull fib_rem_rec(int n) {
    std::vector<ull> cache(n+1, 0);
    cache[1] = 1; cache[2] = 1;
    return fib_rem_rec_aux(n, cache);
}

//====================================================================
ull fib_upres(int n) {
    std::vector<ull> cache(n+1, 0);
    cache[1] = 1; cache[2] = 1;

    for (int i = 3; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2];
        cache[i] %= FIB_MOD;
    }

    return cache[n];
}

//ull fib_upres_sum(int n) {
//    std::vector<ull> cache(n+1, 0);
//    cache[1] = 1; cache[2] = 1;

//    for (int i = 3; i <= n; ++i) {
//        cache[i] = cache[i-1] + cache[i-2];
//        cache[i] %= div;
//    }

//    ull f = 0;
//    for (int i = 1; i < n; ++i) {
//        f += cache[i];
//        f %= div;
//    }

//    return f;
//}

//====================================================================
ull fib_upres_simpler(ull n) {

    if (n < 3)
        return 1;

    ull cache;
    ull cache1 = 1;
    ull cache2 = 1;

    for (ull i = 3; i <= n; ++i) {
        cache = cache1 + cache2;
        cache %= FIB_MOD;

        cache1 = cache2;
        cache2 = cache;
    }

    return cache;
}

//====================================================================
ull fib_mul_mod_m(ull a, ull b, ull m) {
   ull d = 0;
   ull mp2 = m >> 1;
   int i;
   if (a >= m) a %= m;
   if (b >= m) b %= m;
   for (i = 0; i < 64; ++i) {
       d = (d > mp2) ? (d << 1) - m : d << 1;
       if (a & 0x8000000000000000ULL)
           d += b;
       if (d > m) d -= m;
       a <<= 1;
   }
   return d;
}

//ull fib_mul_mod(ull a, ull b) {
//    return fib_mul_mod_m(a,b,FIB_MOD);
//}

// a0 a1  \/  b0 b1  _  a0*b0 + a1*b2   a0*b1 + a1*b3
// a2 a3  /\  b2 b3  -  a2*b0 + a3*b2   a2*b1 + a3*b3
void fib_mul(ull* c, const ull* a, const ull* b) {
    c[0] = (a[0]*b[0] + a[1]*b[2]) % FIB_MOD;
    c[1] = (a[0]*b[1] + a[1]*b[3]) % FIB_MOD;
    c[2] = (a[2]*b[0] + a[3]*b[2]) % FIB_MOD;
    c[3] = (a[2]*b[1] + a[3]*b[3]) % FIB_MOD;
}

//void fib_mul(ull* c, const ull* a, const ull* b) {
//    c[0] = (fib_mul_mod(a[0],b[0]) + fib_mul_mod(a[1],b[2])) % FIB_MOD;
//    c[1] = (fib_mul_mod(a[0],b[1]) + fib_mul_mod(a[1],b[3])) % FIB_MOD;
//    c[2] = (fib_mul_mod(a[2],b[0]) + fib_mul_mod(a[3],b[2])) % FIB_MOD;
//    c[3] = (fib_mul_mod(a[2],b[1]) + fib_mul_mod(a[3],b[3])) % FIB_MOD;
//}

static int fib_pow_calls = 0;

void fib_pow(ull* c, const ull* a, ull n) {
    ++fib_pow_calls;

    if (n == 1) {
        c[0] = a[0];
        c[1] = a[1];
        c[2] = a[2];
        c[3] = a[3];
    } else {
        if (n%2==0) {
            ull tmp[4];
            fib_pow(tmp, a, n/2);
            fib_mul(c, tmp, tmp);
        } else {
            ull tmp[4]; ull tmp2[4];
            fib_pow(tmp, a, (n-1)/2);
            fib_mul(tmp2, tmp, tmp);
            fib_mul(c, a, tmp2);
        }
    }
}

ull fib_matrix(ull n) {
    if (n < 3)
        return 1;
    const ull a[4] = {1, 1, 1, 0};
    ull c[4];
    fib_pow(c, a, n-1);
    return c[0];
}

//====================================================================
static int fib_div2_calls = 0;

ull fib_div2(ull n) {
    std::cout << n << std::endl;
    ++fib_div2_calls;

    ull res = 0;

    if (n == 0)
        res = 0;
    else if (n < 3)
        res = 1;
    else {
        if (n%2==0) {
            ull f2 = fib_div2(n/2);
            res = (2*fib_div2(n/2+1) - f2) % FIB_MOD;
            res = (f2*res) % FIB_MOD;
        } else {
            ull f1 = fib_div2((n-1)/2);
            ull f2 = fib_div2((n+1)/2);
            res = (f1*f1 + f2*f2) % FIB_MOD;
        }
    }

    return res;
}

//====================================================================
std::vector<ull> fib = {
    0, 1, 1, 2, 3,
    5, 8, 13, 21, 34,
    55, 89, 144, 233, 377,
    610, 987, 1597, 2584, 4181,
    6765, 10946};

//====================================================================
/*
TEST(Fibonachi, Simple) {
    for (int i = 1; i < (int)fib.size(); ++i) {
        ull f = fib_simple_rec(i);
        std::cout << f << " ";
        EXPECT_EQ(fib[i]%FIB_MOD, f);
    }
    std::cout << std::endl;
}

TEST(Fibonachi, RemRec) {
    for (int i = 1; i < (int)fib.size(); ++i) {
        ull f = fib_rem_rec(i);
        std::cout << f << " ";
        EXPECT_EQ(fib[i]%FIB_MOD, f);
    }
    std::cout << std::endl;
}

TEST(Fibonachi, Upres) {
    for (int i = 1; i < (int)fib.size(); ++i) {
        ull f = fib_upres(i);
        std::cout << f << " ";
        EXPECT_EQ(fib[i]%FIB_MOD, f);
    }
    std::cout << std::endl;
}

TEST(Fibonachi, UpresSimpler) {
    for (int i = 1; i < (int)fib.size(); ++i) {
        ull f = fib_upres_simpler(i);
        std::cout << f << " ";
        EXPECT_EQ(fib[i]%FIB_MOD, f);
    }
    std::cout << std::endl;
}
*/

TEST(Fibonachi, Matrix) {
    for (int i = 1; i < (int)fib.size(); ++i) {
        ull f = fib_matrix(i);
        std::cout << f << " ";
        EXPECT_EQ(fib[i]%FIB_MOD, f);
    }
    std::cout << std::endl;
}
//====================================================================
//TEST(Fibonachi, SimpleTime) {
//    std::cout <<  fib_simple_rec(40) << std::endl;
//}

//TEST(Fibonachi, RemRecTime) { // 17365 ms
//    ull f = 0;
//    for (int i = 1; i < 30000; ++i) {
//        f += fib_rem_rec(i);
//        f %= div;
//    }
//    std::cout << f << std::endl;
//}

//TEST(Fibonachi, UpresTime) { // 9150 sm
//    ull f = 0;
//    for (int i = 1; i < 30000; ++i) {
//        f += fib_upres(i);
//        f %= div;
//    }
//    std::cout << f << std::endl;
//}

//TEST(Fibonachi, UpresSimplerTime) { // 4762 sm
//    ull f = 0;
//    for (int i = 1; i < 30000; ++i) {
//        f += fib_upres_simpler(i);
//        f %= div;
//    }
//    std::cout << f << std::endl;
//}

//TEST(Fibonachi, UpresSumTime) {
//    std::cout << fib_upres_sum(30000) << std::endl;
//}

//====================================================================
//const ull FIB_MAX = 10000000ULL;

//TEST(Fibonachi, UpresMaxTime) { // std::bad_alloc
//    std::cout << fib_upres(FIB_MAX) << std::endl;
//}

//TEST(Fibonachi, Div2MaxTime) {
//    std::cout << fib_div2(FIB_MAX) << std::endl;
//    std::cout << FIB_MAX << " - fib_div2_calls: " << fib_div2_calls << std::endl;
//}

//TEST(Fibonachi, SimplerMaxTime) {
//    std::cout << fib_upres_simpler(FIB_MAX) << std::endl;
//}

TEST(Fibonachi, MatrixMaxTime) {
    const ull FIB_MAX = ULONG_LONG_MAX;
    fib_pow_calls = 0;
    std::cout << fib_matrix(FIB_MAX) << std::endl;
    std::cout << FIB_MAX << " - fib_pow_calls: " << fib_pow_calls << std::endl;
}

#endif
