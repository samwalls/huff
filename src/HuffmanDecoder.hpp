#ifndef DATA_ENCODING_P01_HUFFMANDECODER_HPP
#define DATA_ENCODING_P01_HUFFMANDECODER_HPP

#include <assert.h>
#include "HuffmanCoder.hpp"
#include "BitReader.hpp"
#include "HuffmanTree.hpp"

template<typename T> class HuffmanDecoder : HuffmanCoder<T> {
    using HuffmanCoder<T>::tree;
    using HuffmanCoder<T>::output;
    /*
     * Encoding procedure derived from figure 3.9,
     * Sayood, K. (2006). Introduction to data compression. Amsterdam: Elsevier, p64
     *
     * Some pseudo-code translated from the flowchart to help understand the process:
     *
     * Decode NYT Procedure:
     * - read e bits, let p = the e-bit number
     * - IF p < r
     * -     read another bit into p
     * - ELSE
     * -     add r to p
     * - output the p+1 element in the NYT list
     * - END
     *
     * Decode Procedure:
     * - DO
     * -     go to root
     * -     WHILE node is not external
     * -         read bit and go to corresponding node
     * -     IF node is NYT
     * -         CALL Decode NYT Procedure
     * -     ELSE
     * -         output the element corresponding to current node
     * -     CALL Update Procedure
     * - WHILE not last bit
     * - END
     */
public:
    /*
     * Based on: Sayood, K. (2006). Introduction to data compression. Amsterdam: Elsevier, section 3.4, p58
     *
     * E.g. for char based symbols, where m is the size of the alphabet:
     * m = UCHAR_MAX
     * e = CHAR_BIT - 1
     * r = m - (1 << e)
     *
     * The following constants: HUFF_SIZE, HUFF_EPSILON, HUFF_REMAINDER are analogous, respectively, to the variables above.
     * The constants are also general for all integral types up to and including long
     */
    //the size of the default alphabet (the maximum possible value given integral type T); e.g. cast 0 to T, perform bitwise negation
    static const unsigned long HUFF_SIZE = ~(T)0;
    static const unsigned long HUFF_EPSILON = sizeof (T) * CHAR_BIT - 1;
    static const unsigned long HUFF_REMAINDER = HUFF_SIZE - (1 << HUFF_EPSILON);

    //The implementation of HuffmanTree determines the update rule
    HuffmanDecoder(std::istream& input, std::ostream& output, HuffmanTree<T>& tree) : HuffmanCoder<T>(input, output, tree), reader(input) {
        streamCounter = 0;
    }

    //place the decoded data into the output bit-buffer
    void decodeAll() {
        //read the length value from the start of the stream, determines the number of bits to read
        unsigned long streamSize = (unsigned long) reader.template read<long>();
        streamCounter += sizeof(long) * CHAR_BIT;
        while (streamCounter < streamSize){
            Node<NodeData<T>, 2>* node = &tree.getRoot();
            T decoded = 0;
            //if not a leaf traverse down using the read bit as the path until we get to a leaf
            while (!node->isLeaf()) {
                if (reader.read())
                    node = node->child(1);
                else
                    node = node->child(0);
                streamCounter++;
            }
            if (node == tree.getNYTNode())
                decoded = decodeNYT();
            else {
                assert(node->getElement().value.exists());
                decoded = node->getElement().value.value();
            }
            output.template write<T>(decoded);
            tree.update(decoded);
        }
    }

protected:
    //decode a character based on the predicate that we've seen the code for NYT
    char decodeNYT() {
        //read 'e' bits into p
        T p = 0;
        reader.read(HUFF_EPSILON, p);
        streamCounter += HUFF_EPSILON;
        if (p < HUFF_REMAINDER) {
            reader.read(1, HUFF_EPSILON, p);
            streamCounter++;
        } else
            p += HUFF_REMAINDER;
        return p;
    }

private:
    BitReader<T> reader;
    unsigned long streamCounter;
};

#endif //DATA_ENCODING_P01_HUFFMANDECODER_HPP
