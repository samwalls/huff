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
    HuffmanEncoder(std::istream& input, std::ostream& output, HuffmanTree<T>& tree) : HuffmanCoder<T>(input, output, tree), reader(input) {}

    //encode the next character from the input stream, place the encoded data in the output BitWriter (stores internally)
    void encode() {
        while (reader.nextBufferGood()) {
            T c = reader.template read<T>();
            if (!tree.findLeaf(c)) {
                tree.outputPath(tree.getNYTNode(), output);
                //the NYT list itself is assumed to be the ASCII table, hence we write the code for the value of c
                output.template write<T>(c);
            } else
                tree.outputPath(c, output);
            this->tree.update(c);
        }
        /*
         * end of the stream. To get around the issue of the byte buffer in BitWriter
         * possibly not being complete, we output the NYT path (repeatedly) until the buffer is full. This way, the decoder
         * will never reach a leaf in the last step and can safely assume that the stream has ended.
         */
        tree.outputPathUntilBufferFull(tree.getNYTNode(), output);
    }

    //reset the encoder by resetting the code tree
    void reset() {
        tree.reset();
    }

private:
    //use this to read individual bits from the input stream
    BitReader<T> reader;
};

#endif //DATA_ENCODING_P01_HUFFMANENCODER_HPP
