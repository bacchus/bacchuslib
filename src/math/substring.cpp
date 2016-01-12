#include "substring.h"
#include "mdefs.h"

namespace bacchus {

bool cmp_strings(const std::string& sentence, const std::string& pattern, int from, int num) {
    bool res = true;
    for (int i = 0; i < num; ++i) {
        if (pattern[i] != sentence[i + from]) {
            res = false;
            break;
        }
    }
    return res;
}

void abc_pattern(int& d, int& q) {
    d = 'z' - 'a' + 1;
    q = 13;
    //q = std::numeric_limits<int>::max()/d + 1;
    //while (!is_prime(--q));
    //    std::cout << "d: " << d << " q: " << q << std::endl;

    //    std::map<char, int> alphabet;
    //    for (char c = 'a'; c <= 'z'; ++c) {
    //        alphabet[c] = c - 'a';
    //    }
    //    s = alphabet.size();
}

int abc_toi(char c) {
    return c - 'a';
}

std::vector<int> prefix_func_naive(const std::string& s) {
    std::vector<int> res(s.size(), 0);
    res[0] = 0;
    for (int i = 1; i < (int)s.size(); ++i) {
        for (int k = 0; k < i; ++k) {
            //            std::string s1 = s.substr(0,k+1);
            //            std::string s2 = s.substr(i-k,k+1);
            //            if (s1 == s2)
            //                res[i] = k+1;
            if (cmp_strings(s, s, i-k, k+1))
                res[i] = k+1;
        }
    }
    return res;
}

std::vector<int> prefix_func(const std::string& pattern) {
    const int m = pattern.size();
    std::vector<int> prefix(m, 0);
    prefix[0] = 0;
    for (int i = 1; i < m; ++i) {
        int k = prefix[i - 1];
        while (k > 0 && pattern[i] != pattern[k]) {
            k = prefix[k - 1];
        }
        if (pattern[i] == pattern[k]) {
            ++k;
        }
        prefix[i] = k;
    }
    return prefix;
}

int find_sub_first_naive(const std::string &sentence, const std::string &pattern) {
    int res = -1;
    int diff = sentence.size() - pattern.size() + 1;
    for (int i = 0; i < diff; ++i) {
        if (cmp_strings(sentence, pattern, i, pattern.size())) {
            res = i;
            break;
        }
    }
    return res;
}

std::vector<int> find_sub_all_naive(const std::string &sentence, const std::string &pattern) {
    std::vector<int> res;
    int diff = sentence.size() - pattern.size() + 1;
    for (int i = 0; i < diff; ++i) {
        if (cmp_strings(sentence, pattern, i, pattern.size())) {
            res.push_back(i);
        }
    }
    return res;
}

std::vector<int> rabin_carp_matcher(const std::string &sentence, const std::string &pattern) {
    int d, q;
    abc_pattern(d,q);

    const int n = sentence.size();
    const int m = pattern.size();
    const int h = pown(d, m-1) % q;
    int p = 0;
    int t = 0;
    std::vector<int> res;

    for (int i = 0; i < m; ++i) {
        p = (d*p + abc_toi(pattern[i])) % q;
        t = (d*t + abc_toi(sentence[i])) % q;
    }

    for (int i = 0; i < n-m+1; ++i) {
        if (p == t && cmp_strings(sentence, pattern, i, pattern.size())) {
            res.push_back(i);
        }
        if (i < n-m) {
            t = (d*(t - h*abc_toi(sentence[i])) + abc_toi(sentence[i+m])) % q;
        }
    }

    return res;
}

std::vector< std::vector<int> > compute_automat(const std::string& pattern) {
    //, const std::string& alphabet
    //int s = alphabet.size();
    //    int s, q;
    //    abc_pattern(s,q);
    int s = 'z' - 'a' + 1;
    int m = pattern.size();
    std::vector< std::vector<int> > automat(m+1, std::vector<int>(s,0));

    for (int i = 0; i < m; ++i) {
        automat[i][abc_toi(pattern[i])] = i + 1;
    }
    //    //for (char a: alphabet) {
    for (char a = 'a'; a <= 'z'; ++a) {
        if (a != pattern[0]) {
            automat[0][abc_toi(a)] = 0;
        }
    }
    std::vector<int> prefix = prefix_func(pattern);
    for (int i = 1; i < m+1; ++i) {
        //for (char a: alphabet) {
        for (char a = 'a'; a <= 'z'; ++a) {
            if (a != pattern[i]) {
                automat[i][abc_toi(a)] = automat[prefix[i-1]][abc_toi(a)];
            }
        }
    }
    return automat;
}

std::vector<int> automat_matcher(const std::string &sentence, const std::string &pattern) {
    std::vector< std::vector<int> > automat = compute_automat(pattern);
    int m = automat.size() - 1;
    int n = sentence.size();
    int q = 0;
    std::vector<int> res;
    for (int i = 0; i < n; ++i) {
        q = automat[q][abc_toi(sentence[i])];
        if (q == m) {
            res.push_back(i-m+1);
        }
    }
    return res;
}

std::vector<int> knuth_morris_pratt_matcher(const std::string &sentence, const std::string &pattern) {
    std::vector<int> res;
    int pl = pattern.size();
    int tl = sentence.size();
    std::vector<int> prefix = prefix_func(pattern + "#" + sentence);
    for (int i = 0; i < tl - pl + 1; ++i) {
        int pp = 2*pl + i;
        if (prefix[pp] == pl) {
            res.push_back(i);
        }
    }
    return res;
}

} // namespace bacchus