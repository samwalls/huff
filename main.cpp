#include "Node.h"
#include "NTree.h"
#include <iostream>

/*
 * Adaptive Huffman Coding Generator
 */
int main() {
    NTree<int, 3>* tree = new NTree<int, 3>(5);
    tree->getRoot()->setChild(0, 1);
    tree->getRoot()->setChild(1, 2);
    tree->getRoot()->setChild(2, 3);
    for (Node<int, 3>* child : tree->getRoot()->getChildren()) {
        std::cout << child->toString() << std::endl;
    }
    delete tree;
}
