#ifndef DATA_ENCODING_P01_BITBUFFER_HPP
#define DATA_ENCODING_P01_BITBUFFER_HPP

#include <climits>

/*
 * Class to represent a buffered bit writer, useful for writing individual bits to an ostream as well packed as possible.
 * Adapted (to use generic ostreams instead of FILE*, and to write MSB-first) from answer by 'Nils Pipenbrinck' on Stack Overflow:
 * (1/10/2016): http://stackoverflow.com/questions/1856514/writing-files-in-bit-form-to-a-file-in-c?noredirect=1&lq=1
 *
 * T_BUFFER is the buffer type used - should be any integral type up to and including long; other types may produce undefined/unexpected behaviour.
 */
template<typename T_BUFFER> class BitWriter {
public:
    static const int BITS = sizeof(T_BUFFER) * CHAR_BIT;

    BitWriter(std::ostream& output) : output(output) {
        reset();
    }

    ~BitWriter() = default;

    //write an individual bit to the bit buffer. The contents of the buffer will be sent to the output stream if the buffer size is reached.
    //bits are written to the buffer msb-first, left to right.
    void writeBit(int bit) {
        if (bit)
            buffer |= (1 << BITS - current_bit - 1);
        if (++current_bit == BITS) {
            //write output once a full buffer-size's worth of information is ready
            //write out in bytes because otherwise the stream operator [gets confused and] tries to to_string the value
            char* o = reinterpret_cast<char*>(&buffer);
            output.write(o,sizeof(T_BUFFER));
            reset();
            /*
            //Debug Code: print bits out
            for (int i = 0; i < BITS; i++)
                std::cout << std::noskipws << (std::to_string(((1 << BITS - i - 1) & buffer) >> BITS - i - 1));
            std::cout << std::noskipws << " ";
            */
        }
    }

    //write all bits in the value to the buffer
    template<typename T>
    void write(T value) {
        //output.write(reinterpret_cast<char*>(&value), sizeof(T));
        int t_bits = sizeof(T) * CHAR_BIT;
        for (int i = 0; i < t_bits; i++)
            writeBit((1 << CHAR_BIT - i - 1) & value);
    }

    //flush all current buffered information (if any) to the output stream with trailing zeroes
    void flush() {
        while (current_bit)
            writeBit(0);
    }

private:
    std::ostream& output;
    int current_bit;
    T_BUFFER buffer;

    void reset() {
        current_bit = 0;
        buffer = 0;
    }
};

#endif //DATA_ENCODING_P01_BITBUFFER_HPP
