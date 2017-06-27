#include "setting.h"
#include "math/sort.h"

using namespace bacchus;

//====================================================================
TEST(InsertSort, General) {
    std::vector<int> a = {5,2,4,6,1,3};
    std::cout << "a: " << a << std::endl;
    insert_sort(a);
    std::cout << "b: " << a << std::endl;
    ASSERT_TRUE(test_sort(a));
}

TEST(InsertSort, TwoElem) {
    std::vector<int> a = {5,2};
    std::cout << "a: " << a << std::endl;
    insert_sort(a);
    std::cout << "b: " << a << std::endl;
    ASSERT_TRUE(test_sort(a));
}

TEST(InsertSort, OneElem) {
    std::vector<int> a = {7};
    std::cout << "a: " << a << std::endl;
    insert_sort(a);
    std::cout << "b: " << a << std::endl;
    ASSERT_TRUE(test_sort(a));
}

TEST(InsertSort, Empty) {
    std::vector<int> a;
    std::cout << "a: " << a << std::endl;
    insert_sort(a);
    std::cout << "b: " << a << std::endl;
    ASSERT_TRUE(test_sort(a));
}

//====================================================================
TEST(MergeSort, General) {
    std::vector<int> a = {2,4,5,7,1,2,3,6};
    std::cout << "a: " << a << std::endl;
    merge_sort(a);
    std::cout << "b: " << a << std::endl;
    ASSERT_TRUE(test_sort(a));
}

TEST(MergeSort, TwoElem) {
    std::vector<int> a = {7,4};
    std::cout << "a: " << a << std::endl;
    merge_sort(a);
    std::cout << "b: " << a << std::endl;
    ASSERT_TRUE(test_sort(a));
}

TEST(MergeSort, OneElem) {
    std::vector<int> a = {5};
    std::cout << "a: " << a << std::endl;
    merge_sort(a);
    std::cout << "b: " << a << std::endl;
    ASSERT_TRUE(test_sort(a));
}

TEST(MergeSort, Empty) {
    std::vector<int> a;
    std::cout << "a: " << a << std::endl;
    merge_sort(a);
    std::cout << "b: " << a << std::endl;
    ASSERT_TRUE(test_sort(a));
}

//====================================================================
TEST(BubbleSort, General) {
    std::vector<int> a = {2,4,5,7,1,2,3,6};
    std::cout << "a: " << a << std::endl;
    bubble_sort(a);
    std::cout << "b: " << a << std::endl;
    ASSERT_TRUE(test_sort(a));
}

TEST(BubbleSort, TwoElem) {
    std::vector<int> a = {7,4};
    std::cout << "a: " << a << std::endl;
    bubble_sort(a);
    std::cout << "b: " << a << std::endl;
    ASSERT_TRUE(test_sort(a));
}

TEST(BubbleSort, OneElem) {
    std::vector<int> a = {5};
    std::cout << "a: " << a << std::endl;
    bubble_sort(a);
    std::cout << "b: " << a << std::endl;
    ASSERT_TRUE(test_sort(a));
}

TEST(BubbleSort, Empty) {
    std::vector<int> a;
    std::cout << "a: " << a << std::endl;
    bubble_sort(a);
    std::cout << "b: " << a << std::endl;
    ASSERT_TRUE(test_sort(a));
}

//====================================================================
//TEST(SortAlgs, QuickSort) {
//    std::vector<float> a = {3,8,5,7,1,6,2,9,4};
//    std::cout << "a: " << a << std::endl;
//    quick_sort(a,0,9);
//    std::cout << "b: " << a << std::endl;
//}

//TEST(SortAlgs, CombSort) {
//    std::vector<float> a = {-45,86,-75,-20,44,-14,-12,0};
//    std::cout << "a: " << a << std::endl;
//    combsort(a);
//    std::cout << "b: " << a << std::endl;
//}


//TEST(SortAlgs, Median) {
//    int a[] = {3, 13, 7, 5, 21, 23, 39, 23, 40, 23, 14, 12, 56, 23, 29};
//    EXPECT_EQ(23, (int)find_median(a,sizeof(a)/sizeof(int)));
//}
