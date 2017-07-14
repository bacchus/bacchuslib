#pragma once

#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>

namespace bacchus {

class FibHeap {
public:
    typedef int T;

    class Node {
    public:
        Node(const T& t): key(t) {}

        Node* parent = nullptr;
        Node* child = nullptr;
        Node* left = nullptr;
        Node* right = nullptr;

        int degree = 0;
        bool mark = false;

        T key;
    };

    FibHeap();

    void insert(Node* x);
    Node* minimum() const;
    void merge(FibHeap* h2);
    Node* extract_min();
    void decrease_key(Node* x, const T& k);
    void remove(Node* x);

private:
    void consolidate();
    void link(Node* y, Node* x);
    void cut(Node* x, Node* y);
    void cascading_cut(Node* y);
    int degree_max();

public:
    int n = 0;
    Node* root = nullptr;//min
};

void print(const FibHeap& heap);

} // namespace bacchus
