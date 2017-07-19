#pragma once

#include "utils/except.h"

#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include <memory>
#include <vector>

namespace bacchus {

//====================================================================
template<class Key, class T = char, class Compare = std::less<Key>>
class FibHeap {
public:

    typedef std::pair<Key,T> value_type;

    class Node {
    public:
        Node(const Key& k, const T& v = T())
            : data(k,v)
        {}

        Node* parent = nullptr;
        Node* child = nullptr;
        Node* left = nullptr;
        Node* right = nullptr;

        int degree = 0;
        bool mark = false;

        value_type data;
    };

    FibHeap(const Compare& c = Compare())
        : comp(c)
    {

    }

    ~FibHeap() {
        //clear_slow();
        clear();
    }

    Node* insert(const Key& key, const T& val = T()) {
        Node* x = new Node(key, val);

        x->degree = 0;
        x->parent = nullptr;
        x->child = nullptr;
        x->mark = false;

        list_insert(root, x);

        if (root == nullptr) {
            root = x;
        } else if (compare(x, root)) {
            root = x;
        }

        ++n;
        return x;
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

    void merge(FibHeap* h2) {
        if (h2->root != nullptr) {

            if (root == nullptr) {
                root = h2->root;

            } else {

                list_merge(root, h2->root);

                if (comp(h2->root->key, root->key)) {
                    root = h2->root;
                }
            }

            n += h2->n;
        }
    }

    value_type extract_min() {
        std::unique_ptr<Node> z(root);

        if (z == nullptr) {
            throw LogicError("try to FibHeap::extract_min from empty heap");
        }

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

        list_remove(z.get()); //BCC: prnt list

        if (z.get() == z->right) {
            root = nullptr;
        } else {
            root = z->right;
            consolidate();
        }

        --n;
        return z->data;
    }

    void decrease_key(Node* x, const Key& k) {
        assert(comp(k, x->data.first));

        x->data.first = k;
        Node* y = x->parent;
        if (y != nullptr && compare(x, y)) {
            cut(x, y);
            cascading_cut(y);
        }

        if (compare(x, root))
            root = x;
    }

    void remove(Node* x) {
        decrease_key(x, root->data.first - Key(1));
        extract_min();
    }

    void clear_slow() {
        while (!empty())
            extract_min();
    }

    void clear() {
        Node* x = root;
        if (x != nullptr) {
            do {
                Node* next = x->right;
                clear(x);
                x = next;
            } while (x != root);
        }

        root = nullptr;
        assert(n==0);
    }

private:
    bool compare(Node* x, Node* y) {
        return comp(x->data.first, y->data.first);
    }

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
                if (compare(y, x)) {
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

                } else if (compare(x, root)) {
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
        return 1 + 2.08f*std::log(n);//2.08 ~ 1/log(golden_ratio)
    }

    void clear(Node* node) {
        Node* x = node->child;
        if (x != nullptr) {
            do {
                Node* next = x->right;
                clear(x);
                x = next;
            } while (x != node->child);
        }

        delete node;
        --n;
    }
    //====================================================================
    /// list ops
    void list_insert(Node* to, Node* x) {
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

    void list_merge(Node* to, Node* x) {
        if (to != nullptr && x != nullptr) {

            Node* left2 = x->left;

            to->left->right = x;
            x->left = to->left;

            to->left = left2;
            left2->right = to;
        }
    }

    void list_remove(Node* x) {
        x->left->right = x->right;
        x->right->left = x->left;
    }

public:
    int n = 0;
    Node* root = nullptr;
    Compare comp;
};

template<class Key, class T, class Compare>
inline void print_fib_heap_node(std::ostream& os, const typename FibHeap<Key,T,Compare>::Node* node, int tab=1) {
    os << node->data.first << ":" << node->data.second;

    if (node->mark)
        os << "*";

    typename FibHeap<Key,T,Compare>::Node* x = node->child;
    if (x) {
        os << "\t";
        print_fib_heap_node<Key,T,Compare>(os, x);

        if (x != x->right) {
            do {
                os << std::endl;
                for (int i = 0; i < tab; ++i) {
                    os << "\t";
                }

                x = x->right;
                print_fib_heap_node<Key,T,Compare>(os, x, tab+1);
            } while (x->right != node->child);
        }
    }
}

template<class Key, class T, class Compare>
inline std::ostream& operator <<(std::ostream& os, const FibHeap<Key,T,Compare>& heap) {
    typename FibHeap<Key,T,Compare>::Node* x = heap.root;
    do {
        print_fib_heap_node<Key,T,Compare>(os, x);
        os << std::endl;

        x = x->right;
    } while (x != heap.root);
    os << std::endl;
    return os;
}

} // namespace bacchus
