#ifndef DATA_ENCODING_P01_BITREADER_HPP
#define DATA_ENCODING_P01_BITREADER_HPP

#include <istream>
#include <climits>

// A class to represent a buffered bit reader. Similar to BitWriter, but rather than writing to an ostream, this reads
// bits (buffered, read each byte MSB first, left-to-right) from an istream. The reader will not check for EOF.
//T_BUFFER is the buffer type used - should be any integral type up to and including long; other types may produce undefined/unexpected behaviour.
template<typename T_BUFFER> class BitReader {
public:
    static const int BITS = sizeof(T_BUFFER) * CHAR_BIT;

    BitReader(std::istream& input) : input(input) {
        reset();
    }

    //read another bit from the buffer, if the buffer is empty a byte will be read from the input stream
    int read() {
        if (current_bit == 0) {
            //read into the buffer if the bit counter has just started
            //we would use reinterpret_cast here to convert into bytes, but reinterpret_cast orders the bytes backwards..
            /*
            for (int i = 0; i < sizeof(T_BUFFER); i++) {
                char byte = 0;
                input >> std::noskipws >> byte;
                buffer |= (((T_BUFFER)byte) << CHAR_BIT * (sizeof(T_BUFFER) - i - 1));
            }
            */
            input.read(reinterpret_cast<char*>(&buffer), sizeof(T_BUFFER));
        }
        //get the value for a particular bit
        long index = BITS - current_bit - 1;
        int bit = (buffer & (1 << index)) >> index;
        if (++current_bit == BITS) {
            //reset the bit counter
            current_bit = 0;
        }
        return bit;
    }

    template<typename T>
    T read() {
        T t = 0;
        for (int i = 0; i < sizeof(T) * CHAR_BIT; i++)
            t |= (read() << BITS - i - 1);
        return t;
    }

    //read n bits into c
    void read(unsigned long n, T_BUFFER& c) {
        read(n, 0, c);
    }

    //read n bits into c, starting from bit i
    void read(unsigned long n, unsigned long i, T_BUFFER& c) {
        for (unsigned long j = 0; j < n; j++)
            c |= (read() << BITS - ((i + j) % BITS) - 1);
    }

    //returns true if the input stream is in a good state to get a new buffer from the input
    bool nextBufferGood() {
        //peek at the input stream, then tell if it is good or not
        //todo this likely only works for a single char
        return input.peek(), input.good();
    }

    int getCurrentBit() {
        return current_bit;
    }

private:
    std::istream& input;
    int current_bit;
    T_BUFFER buffer;

    void reset() {
        current_bit = 0;
        buffer = 0;
    }
};

#endif //DATA_ENCODING_P01_BITREADER_HPP
