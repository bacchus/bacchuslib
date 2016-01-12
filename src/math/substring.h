#pragma once

#include <vector>
#include <string>

namespace bacchus {

int find_sub_first_naive(const std::string& sentence, const std::string& pattern);
std::vector<int> find_sub_all_naive(const std::string& sentence, const std::string& pattern);

std::vector<int> rabin_carp_matcher(const std::string& sentence, const std::string& pattern);
std::vector<int> automat_matcher(const std::string& sentence, const std::string& pattern);
std::vector<int> knuth_morris_pratt_matcher(const std::string& sentence, const std::string& pattern);

} // namespace bacchus
