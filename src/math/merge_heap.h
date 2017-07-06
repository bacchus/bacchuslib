#pragma once

#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>

namespace bacchus {

class MergeHeap {
public:
    typedef int T;

    class Node {
    public:
        Node() {}

        Node* p = nullptr;
        Node* child = nullptr;
        Node* left = nullptr;
        Node* right = nullptr;

        int degree = 0;
        bool mark = false;

        T key;
    };

    MergeHeap() {}

    void insert(Node* x) {
        x->degree = 0;
        x->p = nullptr;
        x->child = nullptr;
        x->mark = false;

        if (root == nullptr) {
            x->left = x;
            x->right = x;
            root = x;
        } else {
            root->left->right = x;
            x->left = root->left->right;
            root->left = x;
            x->right = root;

            if (x->key < root->key)
                root = x;
        }

        ++n;
    }

    Node* minimum() {
        return root;
    }

    void merge(MergeHeap* h2) {
        if (h2->root != nullptr) {

            if (root == nullptr) {
                root = h2->root;

            } else {
                Node* left2 = h2->root->left;

                root->left->right = h2->root;
                h2->root->left = root->left->right;

                root->left = left2;
                left2->right = root;

                if (h2->root->key < root->key) {
                    root = h2->root;
                }
            }

            n += h2->n;
        }
    }

    Node* pop_min() {
        Node* z = root;
        if (z != nullptr) {

            Node* x = z->child;
            if (x != nullptr) {
                // make all child.p=nil
                do {
                    x->p = nullptr;
                    x = x->right;
                } while (x != z->child);

                // insert children to root list
                Node* left2 = x->left;

                root->left->right = x;
                x->left = root->left->right;

                root->right->left = left2;
                left2->right = root->right;
            }

            if (z == z->right) {
                root = nullptr;
            } else {
                root = z->right;
                consolidate();
            }

            --n;
        }

        return z;
    }

    void consolidate() {
        std::vector<Node*> a(degree_max(), nullptr);

        Node* w = root;
        do {

            Node* x = w;
            int d = x->degree;
            while (a[d] != nullptr) {
                Node* y = a[d];
                if (x->key > y->key) {
                    Node* tmp = x;
                    x = y;
                    y = tmp;
                }

                link(y, x);

                a[d] = nullptr;
                ++d;
            }
            a[d] = x;

            w = w->right;
        } while (w != root);

        root = nullptr;
        for (int i = 0; i < a.size(); ++i) {
            Node* x = a[i];
            if (x != nullptr) {
                if (root == nullptr) {
                    x->left = x;
                    x->right = x;
                    x->p = nullptr;
                    root = x;

                } else {
                    root->left->right = x;
                    x->left = root->left->right;
                    root->left = x;
                    x->right = root;

                    if (x->key < root->key)
                        root = x;
                }
            }
        }
    }

    void link(Node* y, Node* x) {
        y->left->right = y->right;
        y->right->left = y->left;

        if (x->child == nullptr) {
            x->child = y;
            y->left = y;
            y->right = y;
        } else {
            x->child->left->right = y;
            y->left = x->child->left->right;
            x->child->left = y;
            y->right = x->child;
        }

        y->p = x;
        ++x->degree;
        y->mark = false;
    }

    void dec_key(Node* x, const T& k) {
        assert(k <= x->key);

        x->key = k;
        Node* y = x->p;
        if (y != nullptr && x->key < y->key) {
            cut(x, y);
            cascading_cut(y);
        }

        if (x->key < root->key)
            root = x;
    }

    void cut(Node* x, Node* y) {
        if (x == x->right) {
            y->child = nullptr;

        } else {
            x->left->right = x->right;
            x->right->left = x->left;

            if (x == y->child)
                y->child = x->right;

            --y->degree;
        }

        root->left->right = x;
        x->left = root->left->right;
        root->left = x;
        x->right = root;
        x->p = nullptr;
        x->mark = false;
    }

    void cascading_cut(Node* y) {
        Node* z = y->p;
        if (z != nullptr) {
            if (y->mark) {
                cut(y,z);
                cascading_cut(z);
            } else {
                y->mark = true;
            }
        }
    }

    void remove(Node* x) {
        dec_key(x, root->key - T(1));
        pop_min();
    }

    int degree_max() {
        return 2.08f*std::log(n);//2.08 ~ 1/log(golden_ratio)
    }

    int n = 0;
    Node* root = nullptr;//min
};

} // namespace bacchus
