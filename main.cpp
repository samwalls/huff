#include "Node.h"
#include "NTree.h"
#include <iostream>

/*
 * Adaptive Huffman Coding Generator
 */
int main() {
    NTree<int, 3>* tree = new NTree<int, 3>(5);
    tree->getRoot()->addChild(1);
    tree->getRoot()->addChild(2);
    tree->getRoot()->addChild(3);
    for (int i = 0; i < 3; i++) {
        Node<int, 3>* child = tree->getRoot()->child(i);
        std::cout << child->toString() << std::endl;
    }
    delete tree;
}