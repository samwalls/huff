#ifndef DATA_ENCODING_P01_FGKTREE_HPP
#define DATA_ENCODING_P01_FGKTREE_HPP

#include "HuffmanTree.hpp"
#include "BitReader.hpp"

template<typename T> class FGKTree : public HuffmanTree<T> {
    using HuffmanTree<T>::root;
    using HuffmanTree<T>::indices;
    using HuffmanTree<T>::reassignIndices;
    using HuffmanTree<T>::getNYTNode;
    using HuffmanTree<T>::makeNYT;
    using HuffmanTree<T>::findWeightGroup;
    using HuffmanTree<T>::findLeaf;
    /*
     * Update procedure derived from figure 3.6,
     * Sayood, K. (2006). Introduction to data compression. Amsterdam: Elsevier, p59
     *
     * Some pseudo-code translated from the flowchart to help understand the process:
     *
     * Swap Procedure:
     * - WHILE true
     * -     IF current node number is the maximum in the weight group (block)
     * -         increment node weight
     * -     ELSE
     * -         swap this node with the highest numbered node in the weight group (block)
     * -     IF current node is root node
     * -         BREAK
     * -     ELSE
     * -         go to the parent node
     * - END
     *
     * New Node Procedure:
     * - NYT gives birth to new NYT on left, and an external node on the right using the symbol
     * - increment weight of new external node, and the node in the old NYT position
     * - go to the old NYT node
     * - IF node is not the root node
     * -     go to the parent node
     * -     CALL Swap Procedure
     * - END
     *
     * Update Procedure:
     * - IF first appearance of symbol
     * -     CALL New Node Procedure
     * - ELSE
     * -     go to symbol's corresponding external node
     * -     CALL Swap Procedure
     * - END
     *
     */
public:
    FGKTree() : HuffmanTree<T>() {}

    virtual void update(T c) override {
        Node<NodeData<T>, 2>* leaf = findLeaf(c);
        //if this is the first appearance of symbol
        if (leaf == nullptr) {
            Node<NodeData<T>, 2>* oldNYT = newNode(c);
            if (oldNYT != &root)
                swapTowardsRoot(oldNYT->getParent());
        } else
            swapTowardsRoot(leaf);
        //reorganise the indices
        reassignIndices();
    }

private:
    //create a new node at the NYT position, the left child being the new NYT node, and the right a new external node with weight 1 and a source alphabet value
    //returns the old NYT node
    Node<NodeData<T>, 2>* newNode(T value) {
        Node<NodeData<T>, 2> *node, *nyt, *newNyt;
        nyt = getNYTNode();
        node = new Node<NodeData<T>, 2>(NodeData<T> { 0, Optional<T>(value) });
        newNyt = new Node<NodeData<T>, 2>(makeNYT());
        nyt->getElement().increment();
        node->getElement().increment();
        nyt->setChild(0, newNyt);
        nyt->setChild(1, node);
        return nyt;
    }

    //Swap this node with the highest node in its weight group, increment its weight value.
    //Perform this on each node upwards until the root is reached.
    void swapTowardsRoot(Node<NodeData<T>, 2> *inputNode) {
        Node<NodeData<T>, 2>* node = inputNode;
        while (node != nullptr) {
            Node<NodeData<T>, 2>* max = getMaxInWeightGroup(node->getElement().weight);
            //do not swap if the max node is this node's parent [not specified in (Sayood 2006)]
            if (node == max || max == node->getParent())
                node->getElement().increment();
            else {
                NodeData<T> nodeData = node->getElement();
                std::array<Node<NodeData<T>, 2>*, 2> nodeChildren = node->getChildren();
                NodeData<T> maxData = max->getElement();
                std::array<Node<NodeData<T>, 2>*, 2> maxChildren = max->getChildren();
                //swap the nodes' values and list of children
                node->setElement(maxData);
                node->setChildren(maxChildren);
                max->setElement(nodeData);
                max->setChildren(nodeChildren);
                //increment the weight of what was the current node (node corresponding to the input value)
                max->getElement().increment();
            }
            node = node->getParent();
        }
    }

    //find the node with weight 'block' with the highest index
    Node<NodeData<T>, 2>* getMaxInWeightGroup(int block) {
        Node<NodeData<T>, 2>* max = nullptr;
        long maxIndex = 0;
        for (auto node : findWeightGroup(block)) {
            if (max == nullptr || indices[node] > maxIndex) {
                max = node;
                maxIndex = indices[node];
            }
        }
        return max;
    }
};

#endif //DATA_ENCODING_P01_FGKTREE_HPP
