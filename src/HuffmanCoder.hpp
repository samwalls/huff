#ifndef DATA_ENCODING_P01_HUFFMANCODER_HPP
#define DATA_ENCODING_P01_HUFFMANCODER_HPP

#include <vector>
#include "HuffmanTree.hpp"
#include "BitWriter.hpp"
#include "Node.hpp"
#include "BitReader.hpp"

template<typename T>
class HuffmanCoder {
public:
    HuffmanCoder(std::istream& input, std::ostream& output, HuffmanTree<T>& tree) : input(input), output(output), tree(tree) {

    }

    ~HuffmanCoder() {
        output.flush();
    }

    HuffmanTree<T>& getTree() {
        return tree;
    }

    void flush() {
        output.flush();
    }

protected:
    HuffmanTree<T>& tree;
    BitWriter<T> output;

    //read a single char from the input stream
    char readChar() {
        char c;
        input >> std::noskipws >> c;
        return c;
    }

private:
    std::istream& input;
};

#endif //DATA_ENCODING_P01_HUFFMANCODER_HPP
