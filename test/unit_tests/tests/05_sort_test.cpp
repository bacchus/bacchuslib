#include "setting.h"
#include "math/sort.h"

using namespace bacchus;

TEST(SortAlgs, QuickSort) {
    std::vector<float> a = {3,8,5,7,1,6,2,9,4};
    std::cout << "a: " << a << std::endl;
    quick_sort(a,0,9);
    std::cout << "b: " << a << std::endl;
}

TEST(SortAlgs, CombSort) {
    std::vector<float> a = {-45,86,-75,-20,44,-14,-12,0};
    std::cout << "a: " << a << std::endl;
    combsort(a);
    std::cout << "b: " << a << std::endl;
}

TEST(SortAlgs, MergeSort) {

}

TEST(SortAlgs, Median) {
    int a[] = {3, 13, 7, 5, 21, 23, 39, 23, 40, 23, 14, 12, 56, 23, 29};
    EXPECT_EQ(23, (int)find_median(a,sizeof(a)/sizeof(int)));
}
