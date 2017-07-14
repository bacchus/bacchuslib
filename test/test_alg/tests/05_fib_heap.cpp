#include "setting.h"

#include "math/fib_heap.h"

#if TEST_05_FIBHEA

using namespace bacchus;

void init_heap(FibHeap& heap) {
    std::vector<int> keys = {
        23, 7,  3,          17, 24
             , 18, 52, 38,  30, 26, 46
             , 39,     41,      35
    };

    std::vector<FibHeap::Node*> nodes;
    for (int i = 0; i < (int)keys.size(); ++i) {
        nodes.push_back(new FibHeap::Node(keys[i]));
    }

    heap.root = nodes[2];
    heap.n = keys.size();

    FibHeap::Node* x;

    x = nodes[0];   x->right = nodes[1];    x->left = nodes[4];
    x = nodes[1];   x->right = nodes[2];    x->left = nodes[0];
    x = nodes[2];   x->right = nodes[3];    x->left = nodes[1];
    x = nodes[3];   x->right = nodes[4];    x->left = nodes[2];
    x = nodes[4];   x->right = nodes[0];    x->left = nodes[3];

    x = nodes[5];   x->right = nodes[6];    x->left = nodes[7];
    x = nodes[6];   x->right = nodes[7];    x->left = nodes[5];
    x = nodes[7];   x->right = nodes[5];    x->left = nodes[6];

    x = nodes[8];   x->right = nodes[8];    x->left = nodes[8];

    x = nodes[9];   x->right = nodes[10];    x->left = nodes[10];
    x = nodes[10];   x->right = nodes[9];    x->left = nodes[9];

    x = nodes[11];   x->right = nodes[11];    x->left = nodes[11];
    x = nodes[12];   x->right = nodes[12];    x->left = nodes[12];
    x = nodes[13];   x->right = nodes[13];    x->left = nodes[13];

    x = nodes[2];   x->child = nodes[6];    x->degree = 3;
    x = nodes[5];   x->child = nodes[11];   x->degree = 1;
    x = nodes[7];   x->child = nodes[12];   x->degree = 1;
    x = nodes[3];   x->child = nodes[8];    x->degree = 1;
    x = nodes[4];   x->child = nodes[9];    x->degree = 2;
    x = nodes[9];   x->child = nodes[13];   x->degree = 1;

    x = nodes[5]; x->mark = true;
    x = nodes[9]; x->mark = true;
    x = nodes[11]; x->mark = true;

    x = nodes[5];   x->parent = nodes[2];
    x = nodes[6];   x->parent = nodes[2];
    x = nodes[7];   x->parent = nodes[2];

    x = nodes[11];   x->parent = nodes[5];
    x = nodes[12];   x->parent = nodes[7];

    x = nodes[8];   x->parent = nodes[3];

    x = nodes[13];   x->parent = nodes[9];
    x = nodes[9];   x->parent = nodes[4];
    x = nodes[10];   x->parent = nodes[4];

    heap.insert(new FibHeap::Node(21));
}

TEST(Fibheap, Print) {
    FibHeap heap;
    init_heap(heap);
    print(heap);

    //3   52
    //    38  41
    //    18* 39*
    //17  30
    //24  26* 35
    //    46
    //23
    //7
    //21
}

TEST(Fibheap, ExtrMin) {
    FibHeap heap;
    init_heap(heap);

    heap.extract_min();

    print(heap);
}

#endif
