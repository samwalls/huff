#ifndef NODE_H
#define NODE_H

#include <array>
#include <sstream>

/*
 * Defines a tree node with elements of type T, and with N maximum children.
 */
template <typename T, unsigned short N> class Node {
public:

    Node(T& element, std::array<Node<T, N>*, N> children) {
        this->element = element;
        this->children = children;
    }

    Node(T& element) : Node(element, std::array<Node<T, N>*, N>()) {}

    Node() : Node(T()) {}

    ~Node() {
        if (N > 0) {
            // destruct all children, leaf-first, recursively
            for (Node<T, N>* child : children)
                delete child;
        }
    }

    //Get this node's element.
    T getElement() {
        return this->element;
    }

    std::array<Node<T, N>*, N> getChildren() {
        return children;
    }

    //Get the ith child in this node's child list.
    Node<T, N>* child(int i) {
        return children[i];
    }

    //The total number of currently populated children.
    unsigned int numChildren() {
        unsigned int total = 0;
        for (int i = 0; i < N; i++) {
            if (children[i] != nullptr)
                total++;
        }
        return total;
    }

    //Set the ith child in this node's child list to the passed node object. Returns false if i is equal to or more than the template argument N.
    bool setChild(const short i, Node<T, N>* child) {
        if (i < 0 || i >= N)
            return false;
        children[i] = child;
        return true;
    }

    //Set the ith child in this node's child list to a new instance of a node with the passed element value.
    bool setChild(const short i, T element) {
        return setChild(i, new Node(element));
    }

    //Returns a string representation of this node. Uses the result of the << stream operator on this node's element (of type T).
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
