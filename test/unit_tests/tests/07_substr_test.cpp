#include "setting.h"
#include <map>
#include "math/mdefs.h"
#include "math/substring.h"

using namespace bacchus;

/// internal helper funcs tests
//TEST(SubstrAll, AMPref) {
//    EXPECT_EQ(std::vector<int>({0,0,0,1,2,3,0}), prefix_func("abcabcd"));
//    EXPECT_EQ(std::vector<int>({0,0,0,1,2}), prefix_func("abbab"));
//    PRINT(prefix_func("abacaba"));
//}

//TEST(SubstrAll, AMAutomat) {
//    std::vector< std::vector<int> > aut = compute_automat("abbab");
//    aut = compute_automat("ababaca");
//    PRINT(prefix_func("ababaca"));
//    PRINT(aut);
//}

TEST(SubstrFirst, Naive) {
    EXPECT_EQ(4, find_sub_first_naive("sdlkfjkldh", "fjkl"));
    EXPECT_EQ(-1, find_sub_first_naive("sdlkfjkldh", "aaaa"));
    EXPECT_EQ(0, find_sub_first_naive("afsgrate", "afsgrate"));
    EXPECT_EQ(-1, find_sub_first_naive("afsgrate", "afsgrateasf"));
}

typedef std::vector<int>(*Matcher)(const std::string&, const std::string&);

TEST(SubstrAll, Naive) {
    Matcher match = find_sub_all_naive;
    EXPECT_EQ(std::vector<int>({4}), match("sdlkfjkldh", "fjkl"));
    EXPECT_EQ(std::vector<int>({}), match("sdlkfjkldh", "aaaa"));
    EXPECT_EQ(std::vector<int>({0}), match("afsgrate", "afsgrate"));
    EXPECT_EQ(std::vector<int>({}), match("afsgrate", "afsgrateasf"));
    EXPECT_EQ(std::vector<int>({2,9,16}), match("klasdlkklasdklklasddf", "asd"));
}

TEST(SubstrAll, RabinCarp) {
    Matcher match = rabin_carp_matcher;
    EXPECT_EQ(std::vector<int>({4}), match("sdlkfjkldh", "fjkl"));
    EXPECT_EQ(std::vector<int>({}), match("sdlkfjkldh", "aaaa"));
    EXPECT_EQ(std::vector<int>({0}), match("afsgrate", "afsgrate"));
    EXPECT_EQ(std::vector<int>({}), match("afsgrate", "afsgrateasf"));
    EXPECT_EQ(std::vector<int>({2,9,16}), match("klasdlkklasdklklasddf", "asd"));
}

TEST(SubstrAll, AutomatMatcher) {
    Matcher match = automat_matcher;
    EXPECT_EQ(std::vector<int>({4}), match("sdlkfjkldh", "fjkl"));
    EXPECT_EQ(std::vector<int>({}), match("sdlkfjkldh", "aaaa"));
    EXPECT_EQ(std::vector<int>({0}), match("afsgrate", "afsgrate"));
    EXPECT_EQ(std::vector<int>({}), match("afsgrate", "afsgrateasf"));
    EXPECT_EQ(std::vector<int>({2,9,16}), match("klasdlkklasdklklasddf", "asd"));
}

TEST(SubstrAll, KMPMatcher) {
    Matcher match = knuth_morris_pratt_matcher;
    EXPECT_EQ(std::vector<int>({0}), match("a", "a"));
    EXPECT_EQ(std::vector<int>({4}), match("sdlkfjkldh", "fjkl"));
    EXPECT_EQ(std::vector<int>({}), match("sdlkfjkldh", "aaaa"));
    EXPECT_EQ(std::vector<int>({0}), match("afsgrate", "afsgrate"));
    EXPECT_EQ(std::vector<int>({}), match("afsgrate", "afsgrateasf"));
    EXPECT_EQ(std::vector<int>({2,9,16}), match("klasdlkklasdklklasddf", "asd"));
}
