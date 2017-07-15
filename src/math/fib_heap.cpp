#include "fib_heap.h"
#include "utils/logger.h"

namespace bacchus {

void list_insert(FibHeap::Node* to, FibHeap::Node* x) {
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

void list_merge(FibHeap::Node* to, FibHeap::Node* x) {
    if (to != nullptr && x != nullptr) {

        FibHeap::Node* left2 = x->left;

        to->left->right = x;
        x->left = to->left;

        to->left = left2;
        left2->right = to;
    }
}

void list_remove(FibHeap::Node* x) {
    x->left->right = x->right;
    x->right->left = x->left;
}

//====================================================================
FibHeap::FibHeap() {}

void FibHeap::insert(FibHeap::Node* x) {
    x->degree = 0;
    x->parent = nullptr;
    x->child = nullptr;
    x->mark = false;

    list_insert(root, x);

    if (root == nullptr) {
        root = x;
    } else if (x->key < root->key) {
        root = x;
    }

    ++n;
}

FibHeap::Node* FibHeap::minimum() const {
    return root;
}

void FibHeap::merge(FibHeap* h2) {
    if (h2->root != nullptr) {

        if (root == nullptr) {
            root = h2->root;

        } else {

            list_merge(root, h2->root);

            if (h2->root->key < root->key) {
                root = h2->root;
            }
        }

        n += h2->n;
    }
}

FibHeap::Node* FibHeap::extract_min() {
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

void FibHeap::consolidate() {
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
            if (x->key > y->key) {
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

            } else if (x->key < root->key) {
                root = x;
            }
        }
    }
}

void FibHeap::link(FibHeap::Node* y, FibHeap::Node* x) {
    list_remove(y);
    list_insert(x->child, y);

    if (x->child == nullptr) {
        x->child = y;
    }

    y->parent = x;
    ++x->degree;
    y->mark = false;
}

void FibHeap::decrease_key(FibHeap::Node* x, const FibHeap::T &k) {
    assert(k <= x->key);

    x->key = k;
    Node* y = x->parent;
    if (y != nullptr && x->key < y->key) {
        cut(x, y);
        cascading_cut(y);
    }

    if (x->key < root->key)
        root = x;
}

void FibHeap::cut(FibHeap::Node* x, FibHeap::Node* y) {
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

void FibHeap::cascading_cut(FibHeap::Node* y) {
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

void FibHeap::remove(FibHeap::Node* x) {
    decrease_key(x, root->key - T(1));
    extract_min();
}

int FibHeap::degree_max() {
    return 2.08f*std::log(n);//2.08 ~ 1/log(golden_ratio)
}

//====================================================================
void print(const FibHeap::Node* node, int tab=1) {

    std::cout << node->key;
    if (node->mark)
        std::cout << "*";

    FibHeap::Node* x = node->child;
    if (x) {
        std::cout << "\t";
        print(x);

        if (x != x->right) {
            do {
                std::cout << std::endl;
                for (int i = 0; i < tab; ++i) {
                    std::cout << "\t";
                }

                x = x->right;
                print(x, tab+1);
            } while (x->right != node->child);
        }
    }

}

void print(const FibHeap& heap) {
    FibHeap::Node* x = heap.minimum();
    do {
        print(x);
        std::cout << std::endl;

        x = x->right;
    } while (x != heap.minimum());
    std::cout << std::endl;
}



} // namespace bacchus
