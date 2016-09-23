#ifndef NODE_H
#define NODE_H

#include <array>
#include <sstream>

/*
 * Defines a tree node with elements of type T, and with N maximum children.
 */
template <typename T, unsigned short N> class Node {
public:

    Node(T element, std::array<Node<T, N>*, N> children) {
        this->element = element;
        this->children = children;
    }

    Node(T element) : Node(element, std::array<Node<T, N>*, N>()) {}

    Node() {}

    ~Node() {
        if (N > 0) {
            // destruct all children, leaf-first, recursively
            for (Node<T, N>* child : children)
                delete child;
        }
    }

    T getElement() {
        return this->element;
    }

    Node<T, N>* child(int i) {
        return children[i];
    }

    //the total number of currently populated children
    unsigned int numChildren() {
        unsigned int total = 0;
        for (int i = 0; i < N; i++) {
            if (children[i] != nullptr)
                total++;
        }
        return total;
    }

    //Append a child to this node's child list, will return false if the number of children is already at or above the template argument N.
    bool addChild(Node<T, N>* child) {
        for (int i = 0; i < N; i++) {
            //look for empty child slots
            if (children[i] == nullptr) {
                children[i] = child;
                return true;
            }
        }
        return false;
    }

    //Append a child to this node's child list, will return false if the number of children is already at or above the template argument N.
    bool addChild(T element) {
        return addChild(new Node(element));
    }

    //Set the ith child in this node's child list to the passed node object. Returns false if i is equal to or more than the template argument N.
    bool setChild(const int i, Node<T, N>* child) {
        if (i < 0 || i >= N)
            return false;
        children[i] = child;
        return true;
    }

    //Returns a string representation of this node. Uses the result of the << stream operator.
    std::string toString() {
        std::stringstream s;
        s << element;
        return s.str();
    }

private:
    //array of size N, containing pointers to child nodes, null pointer represents a non-existent child
    std::array<Node<T, N>*, N> children;
    T element;
};

#endif //NODE_H
