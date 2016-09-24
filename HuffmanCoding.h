#ifndef HUFFMAN_CODING_H
#define HUFFMAN_CODING_H

#include "Probable.h"
using namespace std;

template<typename T> class HuffmanCoding {
public:
    HuffmanCoding(vector<Probable<T>> source) {
        this.source = source;
    }

    ~HuffmanCoding() {
        delete source;
        delete code;
        delete coding;
    }
private:
    vector<Probable<T>>* source;
    vector<T>* code;
    //a map of source objects (with probability values) to output object
    map<Probable<T>, T>* coding;
};

#endif //HUFFMAN_CODING_H
