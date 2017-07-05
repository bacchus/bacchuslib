#pragma once

#include <iostream>
#include <cassert>

namespace bacchus {

template<class T>
class RbNode {
public:

    enum Color {
        BLACK = 0,
        RED = 1
    };

    RbNode(const T& k): key(k) {}

    RbNode<T>* left = nullptr;
    RbNode<T>* right = nullptr;
    RbNode<T>* p = nullptr;
    Color color = BLACK;

    T key;
};

template<class T>
class RbTree {
public:
    typedef RbNode<T> Node;

    RbTree() {}

    Node* root = nullptr;

    ///     |                            |
    ///     y      <--left_rot(x)--      x
    ///    / |                          / |
    ///   x   c    --right_rot(y)->    a   y
    ///  / |                              / |
    /// a   b                            b   c

    void right_rot(Node* y) {
        Node* x = y->left;      // init x
        assert(x != nullptr);

        y->left = x->right;     // move b
        if (x->right != nullptr)
            x->right->p = y;

        transplant(y, x);
//        x->p = y->p;            // reparent x
//        if (y->p == nullptr)
//            root = x;           // if y was root, now y - root
//        else if (y == y->p->left)
//            y->p->left = x;     // if y was left child, now x
//        else
//            y->p->right = x;    // if y was right child, now x

        x->right = y;           // y become right child of x
        y->p = x;               // y parent now x
    }

    void left_rot(Node* x) {
        Node* y = x->right;     // init y
        assert(y != nullptr);

        x->right = y->left;     // move b
        if (y->left != nullptr)
            y->left->p = x;

        transplant(x, y);
//        y->p = x->p;            // reparent y
//        if (x->p == nullptr)
//            root = y;           // if x was root, now y - root
//        else if (x == x->p->left)
//            x->p->left = y;     // if x was left child, now y
//        else
//            x->p->right = y;    // if x was right child, now y

        y->left = x;            // x become left child of y
        x->p = y;               // x parent now y
    }

    void insert(Node* z) {
        Node* y = nullptr;
        Node* x = root;

        while (x != nullptr) {
            y = x;
            if (z->key < x->key)
                x = x->left;
            else
                x = x->right;
        }

        z->p = y;
        if (y == nullptr)
            root = z;
        else if (z->key < y->key)
            y->left = z;
        else
            y->right = z;

        z->left = nullptr;
        z->right = nullptr;
        z->color = Node::RED;

        insert_fix(z);
    }

    void insert_fix(Node* z) {
        while (z->p->color == Node::RED) {
            if (z->p == z->p->p->left) {
                Node* y = z->p->p->right;
                if (y->color == Node::RED) {
                    z->p->color = Node::BLACK;
                    y->color = Node::BLACK;
                    z->p->p->color = Node::RED;
                    z = z->p->p;
                } else {
                    if (z == z->p->right) {
                        z = z->p;
                        left_rot(z);
                    }

                    z->p->color = Node::BLACK;
                    z->p->p->color = Node::RED;
                    right_rot(z->p->p);
                }
            } else {
                Node* y = z->p->p->left;
                if (y->color == Node::RED) {
                    z->p->color = Node::BLACK;
                    y->color = Node::BLACK;
                    z->p->p->color = Node::RED;
                    z = z->p->p;
                } else {
                    if (z == z->p->left) {
                        z = z->p;
                        right_rot(z);
                    }

                    z->p->color = Node::BLACK;
                    z->p->p->color = Node::RED;
                    left_rot(z->p->p);
                }
            }
        }

        root->color = Node::BLACK;
    }

    void transplant(Node* u, Node* v) {
        if (u->p == nullptr)
            root = v;
        else if (u == u->p->left)
            u->p->left = v;
        else
            u->p->right = v;

        v->p = u->p;
    }

    void remove(Node* z) {
        Node* x = nullptr;
        Node* y = z;
        typename Node::Color y_orig_col = y->color;

        if (z->left == nullptr) {
            x = z->right;
            transplant(z, x);
        } else if (z->right == nullptr) {
            x = z->left;
            transplant(z, x);
        } else {
            y = tree_min(z->right);
            y_orig_col = y->color;
            x = y->right;
            if (y->p == z) {
                x->p = y;
            } else {
                transplant(y, x);
                y->right = z->right;
                y->right->p = y;
            }

            transplant(z, y);
            y->left = z->left;
            y->left->p = y;
            y->color = z->color;
        }

        if (y_orig_col == Node::BLACK)
            remove_fix(x);
    }

    void remove_fix(Node* x) {
        while (x != root && x->color == Node::BLACK) {
            if (x == x->p->left) {
                Node* w = x->p->right;

                if (x->color == Node::RED) {
                    w->color = Node::BLACK;
                    x->p->color = Node::RED;
                    left_rot(x->p);
                    w = x->p->right;
                }

                if (w->left->color == Node::BLACK
                        && w->right->color == Node::BLACK) {
                    w->color = Node::RED;
                    x = x->p;
                } else {
                    if (w->right->color == Node::BLACK) {
                        w->left->color = Node::BLACK;
                        x->color = Node::RED;
                        right_rot(w);
                        w = x->p->right;
                    }
                    w->color = x->p->color;
                    x->p->color = Node::BLACK;
                    left_rot(x->p);
                    x = root;
                }
            } else {
                Node* w = x->p->left;

                if (x->color == Node::RED) {
                    w->color = Node::BLACK;
                    x->p->color = Node::RED;
                    right_rot(x->p);
                    w = x->p->left;
                }

                if (w->right->color == Node::BLACK
                        && w->left->color == Node::BLACK) {
                    w->color = Node::RED;
                    x = x->p;
                } else {
                    if (w->left->color == Node::BLACK) {
                        w->right->color = Node::BLACK;
                        x->color = Node::RED;
                        left_rot(w);
                        w = x->p->left;
                    }
                    w->color = x->p->color;
                    x->p->color = Node::BLACK;
                    right_rot(x->p);
                    x = root;
                }
            }
        }
    }

    void inorder_walk(Node* x) {
        if (x != nullptr) {
            inorder_walk(x->left);
            std::cout << x->key << " ";
            inorder_walk(x->right);
        }
    }

    Node* search_req(Node* x, const T& k) {
        if (x == nullptr || k == x->key)
            return x;
        if (k < x->key)
            return search(x->left, k);
        else
            return search(x->right, k);
    }

    Node* search(Node* x, const T& k) {
        while (x != nullptr && k != x->key) {
            if (k < x->key)
                x = x->left;
            else
                x = x->right;
        }
        return x;
    }

    Node* tree_min(Node* x) {
        while (x->left != nullptr)
            x = x->left;
        return x;
    }

    Node* tree_max(Node* x) {
        while (x->right != nullptr)
            x = x->right;
        return x;
    }

    Node* tree_succ(Node* x) {
        if (x->right != nullptr)
            return tree_min(x->right);
        Node* y = x->p;
        while (y != nullptr && x == y->right) {
            x = y;
            y = y->p;
        }
        return y;
    }

    Node* tree_pred(Node* x) {
        if (x->left != nullptr)
            return tree_max(x->left);
        Node* y = x->p;
        while (y != nullptr && x == y->left) {
            x = y;
            y = y->p;
        }
        return y;
    }
};

} // namespace bacchus
