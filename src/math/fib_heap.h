#pragma once

#include <iostream>
#include <cassert>
#include <vector>
#include <cmath>
#include <functional>

namespace bacchus {

template<class T>
void list_insert(T* to, T* x) {
    if (to == nullptr) {
        x->left = x;
        x->right = x;

    } else {
        to->left->right = x;
        x->left = to->left;
        to->left = x;
        x->right = to;
    }
}

template<class T>
void list_merge(T* to, T* x) {
    if (to != nullptr && x != nullptr) {

        T* left2 = x->left;

        to->left->right = x;
        x->left = to->left;

        to->left = left2;
        left2->right = to;
    }
}

template<class T>
void list_remove(T* x) {
    x->left->right = x->right;
    x->right->left = x->left;
}

//====================================================================
template<class T>
class FibHeap {
public:

    typedef std::function<bool(const T&, const T&)> Less;

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

    FibHeap(Less l = std::less<T>()): less(l) {}
    ~FibHeap() {}

    void insert(Node* x) {
        x->degree = 0;
        x->parent = nullptr;
        x->child = nullptr;
        x->mark = false;

        list_insert(root, x);

        if (root == nullptr) {
            root = x;
        } else if (less(x->key, root->key)) {
            root = x;
        }

        ++n;
    }

    Node* minimum() const {
        return root;
    }

    int size() const {
        return n;
    }

    bool empty() const {
        return n == 0;
    }

    void merge(FibHeap<T>* h2) {
        if (h2->root != nullptr) {

            if (root == nullptr) {
                root = h2->root;

            } else {

                list_merge(root, h2->root);

                if (less(h2->root->key, root->key)) {
                    root = h2->root;
                }
            }

            n += h2->n;
        }
    }

    Node* extract_min() {
        Node* z = root;

        if (z != nullptr) {

            if (z->child != nullptr) {
                // make all child.p=nil
                Node* x = z->child;
                do {
                    x->parent = nullptr;

                    x = x->right;
                } while (x != z->child);

                // insert children to root list
                list_merge(root, z->child);
            }

            list_remove(z);

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

    void decrease_key(Node* x, const T& k) {
        assert(less(k, x->key));

        x->key = k;
        Node* y = x->parent;
        if (y != nullptr && less(x->key, y->key)) {
            cut(x, y);
            cascading_cut(y);
        }

        if (less(x->key, root->key))
            root = x;
    }

    void remove(Node* x) {
        decrease_key(x, root->key - T(1));
        extract_min();
    }

private:
    void consolidate() {
        std::vector<Node*> a(degree_max(), nullptr);

        std::vector<Node*> root_list;
        Node* node = root;
        do {
            root_list.push_back(node);
            node = node->right;
        } while (node != root);

        for (Node* w: root_list) {
            Node* x = w;
            int d = x->degree;

            while (a[d] != nullptr) {
                Node* y = a[d];
                if (less(y->key, x->key)) {
                    std::swap(x,y);
                }

                link(y, x);

                a[d] = nullptr;
                ++d;
            }
            a[d] = x;
        }

        root = nullptr;
        for (int i = 0; i < (int)a.size(); ++i) {
            Node* x = a[i];
            if (x != nullptr) {

                list_insert(root, x);

                if (root == nullptr) {
                    x->parent = nullptr;
                    root = x;

                } else if (less(x->key, root->key)) {
                    root = x;
                }
            }
        }
    }

    void link(Node* y, Node* x) {
        list_remove(y);
        list_insert(x->child, y);

        if (x->child == nullptr) {
            x->child = y;
        }

        y->parent = x;
        ++x->degree;
        y->mark = false;
    }

    void cut(Node* x, Node* y) {
        list_remove(x);

        if (x == x->right) {
            y->child = nullptr;
        } else if (x == y->child) {
            y->child = x->right;
        }

        --y->degree;

        list_insert(root, x);

        x->parent = nullptr;
        x->mark = false;
    }

    void cascading_cut(Node* y) {
        Node* z = y->parent;
        if (z != nullptr) {
            if (y->mark) {
                cut(y,z);
                cascading_cut(z);
            } else {
                y->mark = true;
            }
        }
    }

    int degree_max() {
        return 2.08f*std::log(n);//2.08 ~ 1/log(golden_ratio)
    }

public:
    int n = 0;
    Node* root = nullptr;//min
    Less less;
};

template<class T>
void print(const typename FibHeap<T>::Node* node, int tab=1) {
    std::cout << node->key;
    if (node->mark)
        std::cout << "*";

    typename FibHeap<T>::Node* x = node->child;
    if (x) {
        std::cout << "\t";
        print<T>(x);

        if (x != x->right) {
            do {
                std::cout << std::endl;
                for (int i = 0; i < tab; ++i) {
                    std::cout << "\t";
                }

                x = x->right;
                print<T>(x, tab+1);
            } while (x->right != node->child);
        }
    }
}

template<class T>
void print(const FibHeap<T>& heap) {
    typename FibHeap<T>::Node* x = heap.root;
    do {
        print<T>(x);
        std::cout << std::endl;

        x = x->right;
    } while (x != heap.root);
    std::cout << std::endl;
}

} // namespace bacchus
