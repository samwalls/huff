#ifndef NTREE_H
#define NTREE_H

#include "Node.h"

template<typename T, int N> class NTree {
public:

    NTree(T element, std::array<Node<T, N>*, N> children) {
        root = new Node<T, N>(element, children);
    }

    NTree(T element) : NTree(element, std::array<Node<T, N>*, N>()) {}

    //recursive nodes in the root will delete themselves once root reaches the end of its life
    ~NTree() {
        delete root;
    }

    virtual Node<T, N>* getRoot() {
        return root;
    }

private:
    Node<T, N>* root;
};

#endif //NTREE_H
