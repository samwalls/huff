#ifndef HUFFMAN_CODING_H
#define HUFFMAN_CODING_H

#include "Probable.h"

template<typename T> class HuffmanCoding {
public:
    HuffmanCoding(std::vector<Probable<T>> source) {
        this.source = source;
    }

    ~HuffmanCoding() {}
private:
    std::vector<Probable<T>> source;
    std::vector<T> code;
    //a map of source objects (with probability values) to output object
    std::map<Probable<T>, T> coding;
};

#endif //HUFFMAN_CODING_H
