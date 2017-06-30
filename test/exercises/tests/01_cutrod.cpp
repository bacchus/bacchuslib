#include "setting.h"

//====================================================================
int cut_rod(const std::vector<int>& prices, int rod_length) {
    if (rod_length==0)
        return 0;
    int max_price = -1;
    for (int i = 1; i <= rod_length; ++i) {
        max_price = std::max(max_price, prices[i] + cut_rod(prices, rod_length - i));
    }
    return max_price;
}

//====================================================================
int cut_rod_mem_aux(const std::vector<int>& prices, int rod_length, std::vector<int>& cache) {
    if (cache[rod_length] >= 0)
        return cache[rod_length];

    int max_price = -1;

    if (rod_length==0) {
        max_price = 0;
    } else {
        for (int i = 1; i <= rod_length; ++i) {
            max_price = std::max(max_price, prices[i] + cut_rod_mem_aux(prices, rod_length - i, cache));
        }
    }

    cache[rod_length] = max_price;
    return max_price;
}

int cut_rod_mem(const std::vector<int>& prices, int rod_length) {
    std::vector<int> cache(rod_length+1, -1);
    return cut_rod_mem_aux(prices, rod_length, cache);
}

//====================================================================
int cut_rod_up(const std::vector<int>& prices, int rod_length) {
    std::vector<int> cache(rod_length+1, 0);

    for (int j = 1; j <= rod_length; ++j) {
        int max_price = -1;
        for (int i = 1; i <= j; ++i) {
            max_price = std::max(max_price, prices[i] + cache[j-i]);
        }
        cache[j] = max_price;
    }

    return cache[rod_length];
}

//====================================================================
std::vector<int> length = {0,1,2,3, 4, 5, 6, 7, 8, 9,10};
std::vector<int> prices = {0,1,5,8, 9,10,17,17,20,24,30};
std::vector<int> maxces = {0,1,5,8,10,13,17,18,22,25,30};

TEST(CutRod, Initial) {
    std::cout << "length: " << length << std::endl;
    std::cout << "prices: " << prices << std::endl;
    std::cout << "maxces: " << maxces << std::endl;
}

//====================================================================
TEST(CutRod, Slow) {
    for (int i = 0; i < (int)length.size(); ++i) {
        int max_price = cut_rod(prices, i);
        EXPECT_EQ(maxces[i], max_price);
    }
}

TEST(CutRod, Mem) {
    for (int i = 0; i < (int)length.size(); ++i) {
        int max_price = cut_rod_mem(prices, i);
        EXPECT_EQ(maxces[i], max_price);
    }
}

TEST(CutRod, Up) {
    for (int i = 0; i < (int)length.size(); ++i) {
        int max_price = cut_rod_up(prices, i);
        EXPECT_EQ(maxces[i], max_price);
    }
}

//====================================================================
//====================================================================
std::vector<int> time_prices(51,2);
int time_n = 25; //30 - slow: 13s
int time_res = 2*time_n;

TEST(CutRod, SlowTime) {
    int max_price = cut_rod(time_prices, time_n);
    std::cout << "max_price: " << max_price << std::endl;
    EXPECT_EQ(time_res, max_price);
}

TEST(CutRod, MemTime) {
    std::vector<int> time_prices(50001,2);
    //int time_n = 50000; //craches on rod_length = 6584
    int time_n = 43415; // 20s ;craches on 43416
    int time_res = 2*time_n;

    int max_price = cut_rod_mem(time_prices, time_n);
    std::cout << "max_price: " << max_price << std::endl;
    EXPECT_EQ(time_res, max_price);
}

TEST(CutRod, UpTime) {
    std::vector<int> time_prices(50001,2);
    int time_n = 43415; //50000 - up: 13.5s
                        // 12s vs 20s - mem max noncrash
    int time_res = 2*time_n;

    int max_price = cut_rod_up(time_prices, time_n);
    std::cout << "max_price: " << max_price << std::endl;
    EXPECT_EQ(time_res, max_price);
}
