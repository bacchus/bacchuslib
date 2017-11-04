#pragma once

#include <vector>
#include <string>

namespace bacchus {

// O(m*n), one time search
std::vector<int> find_sub_all_naive(const std::string& sentence, const std::string& pattern);
// O(m) - precompute, avg - O(m+n), worst - O(m*n); after precomp can use diff sentence to match
std::vector<int> rabin_carp_matcher(const std::string& sentence, const std::string& pattern);
std::vector<int> automat_matcher(const std::string& sentence, const std::string& pattern);
std::vector<int> knuth_morris_pratt_matcher(const std::string& sentence, const std::string& pattern);

// compress naive: aabccc -> a2b1c3 -> a2bc3 simplify removing 1s
// compress Huffman codes: 0 101 100 111 1101 1100

} // namespace bacchus
