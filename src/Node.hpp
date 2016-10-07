#ifndef DATA_ENCODING_P01_NODE_HPP
#define DATA_ENCODING_P01_NODE_HPP

#include <array>
#include <sstream>
#include <functional>
#include <queue>

/*
 * Defines a tree node with elements of type T, and with N maximum children.
 * Nodes have access to a list of pointers to child nodes, as well as a pointer to its parent node (if it exists)
 */
template <typename T, const unsigned short N> class Node {
public:

    Node(Node<T, N>* parent, T element, std::array<Node<T, N>*, N> children) {
        this->parent = parent;
        this->element = element;
        this->children = children;
        for (auto node : children) {
            if (node != nullptr)
                node->setParent(this);
        }
    }

    Node(Node<T, N>* parent, T element) : Node(parent, element, std::array<Node<T, N>*, N>()) {}

    Node(T element, std::array<Node<T, N>*, N> children) : Node(nullptr, element, children) {}

    Node(T element) : Node(nullptr, element) {}

    Node() : Node(T()) {}

    ~Node() {
        if (N > 0) {
            // destruct all children, leaf-first, recursively
            for (auto child : children)
                delete child;
        }
    }

    //Get a reference to this node's element.
    T& getElement() {
        return this->element;
    }

    void setElement(T element) {
        this->element = element;
    }

    std::array<Node<T, N>*, N> getChildren() {
        return children;
    }

    //set the array of children to the passed array, all passed children will have their parent assigned as this
    void setChildren(std::array<Node<T, N>*, N> children) {
        this->children = children;
        for (auto node : this->children) {
            if (node != nullptr)
                node->setParent(this);
        }
    }

    //Get the ith child in this node's child list.
    Node<T, N>* child(int i) {
        return children[i];
    }

    //The total number of currently populated children.
    unsigned short totalChildren() {
        unsigned short total = 0;
        for (int i = 0; i < N; i++) {
            if (children[i] != nullptr)
                total++;
        }
        return total;
    }

    //Set the ith child in this node's child list to the passed node object. Returns false if i is equal to or more than the template argument N.
    bool setChild(const unsigned short i, Node<T, N>* child) {
        if (i < 0 || i >= N)
            return false;
        child->setParent(this);
        children[i] = child;
        return true;
    }

    //Set the ith child in this node's child list to a new instance of a node with the passed element value.
    bool setChild(const short i, T element) {
        return setChild(i, new Node(element));
    }

    Node<T, N>* getParent() {
        return this->parent;
    }

    void setParent(Node<T, N>* parent) {
        this->parent = parent;
    }

    //Returns a string representation of this node. Uses the result of the << stream operator on this node's element (of type T).
    std::string toString() {
        std::stringstream s;
        s << element;
        return s.str();
    }

    Node<T, N>* operator[](const unsigned short i) {
        return child(i);
    };

    Node<T, N>* findLeaf(std::function<bool(T)> predicate) {
        //base case, this is a leaf, either return this node or null if the search predicate is not matched
        if (isLeaf())
            return predicate(getElement()) ? this : nullptr;
        else {
            //recursive call to all existing children
            for (auto node : children) {
                if (node != nullptr) {
                    Node<T, N>* potentialLeaf = node->findLeaf(predicate);
                    //check the return value of the child's findLeaf
                    if (potentialLeaf != nullptr)
                        return potentialLeaf;
                }
            }
            //no child returned a non-null value, return null
            return nullptr;
        }
    }

    bool isLeaf() {
        for (auto node : children) {
            if (node != nullptr)
                return false;
        }
        return true;
    }

    bool isChild(Node<T, N>* node) {
        for (auto child : children) {
            if (node == child)
                return true;
        }
        return false;
    }

    bool isChild(int i, Node<T, N>* node) {
        return child(i) == node;
    }

    //perform the action at every node, traversing in level order (Right To Left)
    void levelTraverseRTL(std::function<void(Node<T, N>*, int i)> action) {
        /*
         * Adapted from answer by 'Omnifarious' (4/10/2016) to perform functional action at each step, with an added counter
         * http://stackoverflow.com/questions/3589716/level-order-traversal-of-a-binary-tree
         */
        std::queue<Node<T, N>*> queue = std::queue<Node<T, N>*>();
        queue.push(this);
        for (int i = 0; !queue.empty(); i++) {
            Node<T, N>* node = queue.front();
            //perform action on node
            action(node, i);
            queue.pop();
            for (int j = N - 1; j >= 0; j--) {
                if (node->child(j))
                    queue.push(node->child(j));
            }
        }
    }

    //find all nodes that match the predicate
    std::vector<Node<T, N>*> findAllMatching(std::function<bool(T)> predicate) {
        std::vector<Node<T, N>*> matched = std::vector<Node<T, N>*>();
        levelTraverseRTL([&](Node<T, N>* node, int i) {
            if (predicate(node->getElement()))
                matched.push_back(node);
        });
        return matched;
    }

protected:
    //the parent node to this node
    Node<T, N>* parent;
    //array of size N, containing pointers to child nodes, null pointer represents a non-existent child
    std::array<Node<T, N>*, N> children;
    T element;
};

#endif //DATA_ENCODING_P01_NODE_HPP
