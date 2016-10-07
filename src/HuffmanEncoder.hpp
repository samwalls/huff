#ifndef DATA_ENCODING_P01_HUFFMANENCODER_HPP
#define DATA_ENCODING_P01_HUFFMANENCODER_HPP

#include "HuffmanTree.hpp"
#include "HuffmanCoder.hpp"

template <typename T> class HuffmanEncoder : public HuffmanCoder<T> {
    using HuffmanCoder<T>::output;
    using HuffmanCoder<T>::tree;
    /*
     * Encoding procedure derived from figure 3.8,
     * Sayood, K. (2006). Introduction to data compression. Amsterdam: Elsevier, p62
     *
     * Some pseudo-code translated from the flowchart to help understand the process:
     *
     * basic procedure:
     * - read symbol
     * - IF first appearance
     * -     send NYT code + index of symbol in the NYT list
     * - ELSE
     * -     send the code path from the root to the symbol leaf
     * - CALL update procedure
     *
     *
     */
public:
    //The implementation of HuffmanTree determines the update rule
    HuffmanEncoder(std::istream& input, HuffmanTree<T>& tree) : HuffmanCoder<T>(input, intermediateStream, tree), reader(input) {
        bitLength = sizeof(long) * CHAR_BIT;
    }

    //encode the next character from the input stream, place the encoded data in the output BitWriter (stores internally)
    void encode() {
        T c = reader.template read<T>();
        if (!tree.findLeaf(c)) {
            bitLength += tree.outputPath(tree.getNYTNode(), output);
            //the NYT list itself is assumed to be the ASCII table, hence we write the code for the value of c
            output.template write<T>(c);
            bitLength += sizeof(T) * CHAR_BIT;
        } else
            bitLength += tree.outputPath(c, output);
        this->tree.update(c);
    }

    //release the internal output stream to the passed ostream
    void release(std::ostream& output) {
        BitWriter<T> writer(output);
        //write the size header
        writer.template write<long>(bitLength);
        //write the compressed data
        output << std::noskipws << intermediateStream.str();
    }

    //reset the encoder by resetting the code tree
    void reset() {
        bitLength = sizeof(long) * CHAR_BIT;
        tree.reset();
    }

private:
    //number to record the number of bits written to the output BitWriter (defined in HuffmanCoder base class), expressed as a long
    unsigned long bitLength;
    //all encoded output is stored here until release is called
    std::ostringstream intermediateStream;
    //use this to read individual bits from the input stream
    BitReader<T> reader;
};

#endif //DATA_ENCODING_P01_HUFFMANENCODER_HPP
