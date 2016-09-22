#include "node.h"
#include <iostream>

/*
 * Adaptive Huffman Coding Generator
 */

int main() {
    Node<int>* child1 = new Node<int>(1);
    Node<int>* child2 = new Node<int>(3);
    std::vector<Node<int>*> children = std::vector<Node<int>*> { child1, child2 };
    Node<int>* myNode = new Node<int>(2, &children);
    Node<int> tst = myNode[0];
    printf("node layout: %i -> { %i, %i }", myNode->getElement(), myNode->child(0)->getElement(), myNode->child(1)->getElement());
}