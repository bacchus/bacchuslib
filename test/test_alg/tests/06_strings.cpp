#include "setting.h"
#include "math/substring.h"

#if TEST_06_STRING

using namespace bacchus;

const std::string sentence("thequickbrownfoxjumpsoverthelazydog");
const std::string sentence_repeat("thequickbrownfoxjumpsoverfoxlazyfoxdog");
const std::string mathes_repeat("fox");
const std::string matches("brownfox");
const std::string not_matches("redfox");
const std::string word1("test");
const std::string word2("testxyz");

const std::vector<int> res_eq = {0};
const std::vector<int> res_no = {};
const std::vector<int> res_01 = {8};
const std::vector<int> res_02 = {13, 25, 32};

//====================================================================
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


//====================================================================
TEST(SubstrAll, Naive) {
    auto match = find_sub_all_naive;

    EXPECT_EQ(res_eq, match("a", "a"));
    EXPECT_EQ(res_01, match(sentence, matches));
    EXPECT_EQ(res_no, match(sentence, not_matches));
    EXPECT_EQ(res_eq, match(word1, word1));
    EXPECT_EQ(res_no, match(word1, word2));
    EXPECT_EQ(res_02, match(sentence_repeat, mathes_repeat));
}

//====================================================================
TEST(SubstrAll, RabinCarp) {
    auto match = rabin_carp_matcher;

    EXPECT_EQ(res_eq, match("a", "a"));
    EXPECT_EQ(res_01, match(sentence, matches));
    EXPECT_EQ(res_no, match(sentence, not_matches));
    EXPECT_EQ(res_eq, match(word1, word1));
    EXPECT_EQ(res_no, match(word1, word2));
    EXPECT_EQ(res_02, match(sentence_repeat, mathes_repeat));
}

TEST(SubstrAll, Automat) {
    auto match = automat_matcher;

    EXPECT_EQ(res_eq, match("a", "a"));
    EXPECT_EQ(res_01, match(sentence, matches));
    EXPECT_EQ(res_no, match(sentence, not_matches));
    EXPECT_EQ(res_eq, match(word1, word1));
    EXPECT_EQ(res_no, match(word1, word2));
    EXPECT_EQ(res_02, match(sentence_repeat, mathes_repeat));
}

TEST(SubstrAll, KMP) {
    auto match = knuth_morris_pratt_matcher;

    EXPECT_EQ(res_eq, match("a", "a"));
    EXPECT_EQ(res_01, match(sentence, matches));
    EXPECT_EQ(res_no, match(sentence, not_matches));
    EXPECT_EQ(res_eq, match(word1, word1));
    EXPECT_EQ(res_no, match(word1, word2));
    EXPECT_EQ(res_02, match(sentence_repeat, mathes_repeat));
}

#endif
