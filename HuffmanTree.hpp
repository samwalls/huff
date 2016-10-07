#ifndef DATA_ENCODING_P01_HUFFMANTREE_HPP
#define DATA_ENCODING_P01_HUFFMANTREE_HPP

#include <istream>
#include <set>
#include <vector>
#include <map>
#include <stack>
#include "NodeData.hpp"
#include "Node.hpp"
#include "BitWriter.hpp"
#include "NodeData.hpp"

template<typename T> class HuffmanTree {
public:
    HuffmanTree() {
        //alphabet is specified, may have less elements than the full alphabet possible with char type
        indices = std::map<Node<NodeData<T>, 2>*, unsigned long>();
        //(~(T)0) is the maximum possible value (unsigned) of a given integral type
        maxNodes = 2 * (~(T)0) - 1;
        root = makeNYT();
        reassignIndices();
    }

    ~HuffmanTree() {}

    //update the tree, defined by implementation class
    virtual void update(T c) = 0;

    Node<NodeData<T>, 2>& getRoot() {
        return root;
    }

    //output the path to the leaf value, return length of the path
    long outputPath(T leafValue, BitWriter<T>& output) {
        std::vector<T> path = std::vector<T>();
        //find a leaf with the leaf value
        Node<NodeData<T>, 2>* node = root.findLeaf([leafValue](NodeData<T> data) {
            return data.value.exists() && data.value.value() == leafValue;
        });
        return outputPath(node, output);
    }

    //output the path to take to get to the node, return the length of the path (in bits)
    long outputPath(Node<NodeData<T>, 2>* node, BitWriter<T>& output) {
        if (node != nullptr) {
            Node<NodeData<T>, 2>* parent = node->getParent();
            int n = 0;
            std::stack<char> path = std::stack<char>();
            while (parent != nullptr) {
                //going up the tree, store a bit representing which path to take if the tree was being traversed downwards
                if (node == parent->child(0))
                    path.push(0);
                else if (node == parent->child(1))
                    path.push(1);
                n++;
                //set the node to the parent, and the parent to the next parent up the hierarchy
                node = parent;
                parent = node->getParent();
            }
            //output the path itself
            while (!path.empty()) {
                char bit = path.top();
                output.writeBit(bit);
                path.pop();
            }
            return n;
        }
        return 0;
    }

    //get the NYT node, returns null if the indices list has no non-null elements
    Node<NodeData<T>, 2>* getNYTNode() {
        Node<NodeData<T>, 2>* nyt = nullptr;
        //the nyt node should be the first non-null node in the indices list (the lowest index), where the root is the highest
        for (const auto &pair : indices) {
            Node<NodeData<T>, 2>* node = pair.first;
            if (node != nullptr && !node->getElement().value.exists() && node->getElement().weight == 0) {
                nyt = pair.first;
                break;
            }
        }
        return nyt;
    }

    Node<NodeData<T>, 2>* findLeaf(std::function<bool(NodeData<T>)> predicate) {
        return root.findLeaf(predicate);
    }

    Node<NodeData<T>, 2>* findLeaf(T symbol) {
        //find a leaf such that it's value exists and equals symbol
        return findLeaf([symbol](NodeData<T> data) {
           return data.value.exists() && data.value.value() == symbol;
        });
    }

    void reset() {
        root = makeNYT();
    }

    std::map<Node<NodeData<T>, 2>*, unsigned long> getIndices() {
        return indices;
    }

protected:
    Node<NodeData<T>, 2> root;
    std::map<Node<NodeData<T>, 2>*, unsigned long> indices;

    static Node<NodeData<T>, 2> makeNYT() {
        NodeData<T> data = NodeData<T> { 0, Optional<T>() };
        return Node<NodeData<T>, 2>(data);
    }

    //search for all nodes with a specific weight group
    std::vector<Node<NodeData<T>, 2>*> findWeightGroup(int weight) {
        return root.findAllMatching([weight](NodeData<T> data) {
            return data.weight == weight;
        });
    }

    unsigned long getMaxNodes() {
        return maxNodes;
    }

    void reassignIndices() {
        this->indices = std::map<Node<NodeData<T>, 2>*, unsigned long>();
        root.levelTraverseRTL([this](Node<NodeData<T>, 2>* node, long i) {
            this->indices[node] = getMaxNodes() - i;
        });
    }

private:
    unsigned long maxNodes;
};

#endif //DATA_ENCODING_P01_HUFFMANTREE_HPP
