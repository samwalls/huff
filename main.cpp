#include <fstream>
#include <iostream>
#include "FGKTree.hpp"
#include "HuffmanEncoder.hpp"
#include "HuffmanDecoder.hpp"


/*
 * Adaptive Huffman Coding File Compressor: "huff"
 * ID: 150013828
 * 7/10/2016
 */

/*--- CONSTANTS, VARIABLES & DECLARATIONS ---*/

static std::string INPUT = "", OUTPUT = "";

static bool HELP = false, DECOMPRESS = false;

static const std::string USAGE =
        "USAGE: huff [--puff] [-h|--help] <input-file> <output-file>\n"
        "\t<input-file>  the file treated as input\n"
        "\t<output-file> the file treated as output\n"
        "\t--puff        Tells huff to decompress the input file. Huff will compress files by default.\n"
        "\t-h|--help     Print this usage screen.";

// parse command line arguments and react to them
static void parseArgs(int argc, char* argv[]);

// create a report on how much compression has been reached, as well as some information about the final code-tree
static void reportCompression(std::string uncompressed, std::string compressed, HuffmanTree<unsigned char>& finalTree);

// encodes the input file, outputting to the output file
static int encode(std::ifstream& input, std::ofstream& output, HuffmanTree<unsigned char>& tree);

// decodes the input file, outputting to the output file
static int decode(std::ifstream& input, std::ofstream& output, HuffmanTree<unsigned char>& tree);

/*--- MAIN METHOD --- */

int main(int argc, char* argv[]) {
    parseArgs(argc, argv);
    FGKTree<unsigned char> tree = FGKTree<unsigned char>();

    //check if help was requested, exit early if so
    if (HELP) {
        std::cout << USAGE << std::endl;
        return 0;
    }

    std::ifstream input;
    std::ofstream output;
    input.open(INPUT, std::ios::in | std::ios::binary);
    output.open(OUTPUT, std::ios::out | std::ios::binary);
    int exitCode = 0;
    if (DECOMPRESS)
        exitCode = decode(input, output, tree);
    else {
        exitCode = encode(input, output, tree);
        reportCompression(INPUT, OUTPUT, tree);
    }
    input.close();
    output.close();
    return exitCode;
}

/*--- METHOD IMPLEMENTATIONS ---*/

static void parseArgs(int argc, char* argv[]) {
    //read each argument except argv[0]
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        //set variables accordingly
        if (arg == "-h" || arg == "--h")
            HELP = true;
        else if (arg == "--puff")
            DECOMPRESS = true;
        else if (INPUT.empty() || INPUT.length() == 0)
            INPUT = arg;
        else if (OUTPUT.empty() || OUTPUT.length() == 0)
            OUTPUT = arg;
        else {
            std::cerr << "unrecognised argument: " << arg << "\n" << USAGE << std::endl;
            std::exit(1);
        }
    }

    if (!HELP) {
        //check that the input and output were both specified
        if (INPUT.empty() || INPUT.length() == 0) {
            std::cerr << "input file not specified!\n" << USAGE << std::endl;
            std::exit(1);
        }
        if (OUTPUT.empty() || OUTPUT.length() == 0) {
            std::cerr << "output file not specified!\n" << USAGE << std::endl;
            std::exit(1);
        }
    }
}

static void reportCompression(std::string uncompressed, std::string compressed, HuffmanTree<unsigned char>& finalTree) {
    //provide report about the level of compression and the tree
    unsigned long nodeCount = finalTree.getIndices().size();
}

static int encode(std::ifstream& input, std::ofstream& output, HuffmanTree<unsigned char>& tree) {
    std::cout << "compressing..." << std::endl;
    HuffmanEncoder<unsigned char> encoder = HuffmanEncoder<unsigned char>(input, tree);
    //continually peek the character ahead, then encode
    // peek() will cause good() to return false if the EOF is reached for instance
    while (input.peek(), input.good())
        encoder.encode();
    encoder.flush();
    //release the encoder's stream to the output
    if (!output.good()) {
        std::cerr << "failed to find / write to " << OUTPUT << std::endl;
        return 1;
    } else {
        encoder.release(output);
        std::cout << "compressed " << INPUT << " into " << OUTPUT << std::endl;
        return 0;
    }
}

static int decode(std::ifstream& input, std::ofstream& output, HuffmanTree<unsigned char>& tree) {
    std::cout << "decompressing..." << std::endl;
    HuffmanDecoder<unsigned char> decoder = HuffmanDecoder<unsigned char>(input, output, tree);
    // peek() will cause good() to return false if the EOF is reached for instance
    if (!(input.peek(), input.good())) {
        std::cerr << "failed to read " << INPUT << std::endl;
        return 1;
    } else if (!output.good()) {
        std::cerr << "failed to find / write to " << OUTPUT << std::endl;
        return 1;
    } else {
        //decode everything
        decoder.decodeAll();
        std::cout << "decompressed " << INPUT << " into " << OUTPUT << std::endl;
        return 0;
    }
}
