#ifndef DATA_ENCODING_P01_NODEDATA_HPP
#define DATA_ENCODING_P01_NODEDATA_HPP

#include "Optional.hpp"

template<typename T>
struct NodeData {
public:
    int weight;
    Optional<T> value;

    NodeData(int weight, Optional<T> value) {
        this->weight = weight;
        this->value = value;
    }

    NodeData() : NodeData(0, Optional<T>()) {}

    void increment() {
        weight++;
    }
};

#endif //DATA_ENCODING_P01_NODEDATA_HPP
