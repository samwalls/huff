#ifndef NODE_H
#define NODE_H

#include <vector>

template <typename T> class Node {
public:
    Node() : Node(m_default()) {}

    Node(T element, std::vector<Node<T>*>* children) {
        this->element = element;
        this->children = children == nullptr ? std::vector<Node<T>*> {} : *(children);
    }

    Node(T element) : Node(element, nullptr) {}

    ~Node() {

    }

    T getElement() {
        return this->element;
    }

    std::vector<Node<T>*> getChildren() {
        return this->children;
    }

    Node<T>* child(int i) {
        return getChildren()[i];
    }

    Node<T> &operator[](const int i) {
        return *(child(i));
    }
private:
    std::vector<Node<T>*> children;
    T element;
    static const T& m_default;
};

#endif //NODE_H
