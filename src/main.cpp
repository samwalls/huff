#include <fstream>
#include <iostream>
#include <cmath>
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
static bool HELP = false, DECOMPRESS = false, REPORT = false;
static const std::string USAGE =
        "USAGE: huff [--puff] [-h|--help] <input-file> <output-file>\n"
        "<input-file>   the file treated as input\n"
        "<output-file>  the file treated as output (will overwrite if already exists)\n"
        "--puff         Tells huff to decompress the input file. Huff will compress files by default.\n"
        "-h|--help      Print this usage screen.\n"
        "-r|--report    Produce a report at the end, detailing the level of compression achieved, most common symbols etc..";

// parse command line arguments and react to them
static void parseArgs(int argc, char* argv[]);

// create a report on how much compression has been reached, as well as some information about the final code-tree
static void reportCompression(std::string uncompressed, std::string compressed, HuffmanTree<unsigned char>& finalTree);

//return the file size in bytes
static long getFileSize(std::string file);

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
    else
        exitCode = encode(input, output, tree);
    input.close();
    output.close();
    if (REPORT)
        reportCompression(INPUT, OUTPUT, tree);
    return exitCode;
}



/*--- METHOD IMPLEMENTATIONS ---*/



static void parseArgs(int argc, char* argv[]) {
    //read each argument except argv[0]
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        //set variables accordingly
        if (arg == "-h" || arg == "--help")
            HELP = true;
        else if (arg == "--puff")
            DECOMPRESS = true;
        else if (arg == "-r" || arg == "--report")
            REPORT = true;
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

static void reportCompression(std::string inputFile, std::string outputFile, HuffmanTree<unsigned char>& finalTree) {
    //provide report about the level of compression and the tree
    long inputSize = getFileSize(inputFile);
    long outputSize = getFileSize(outputFile);
    unsigned char mostCommonSymbol = 0;
    unsigned char leastCommonSymbol = 0;

    unsigned long maxI = 0, minI = 0;
    auto indices = finalTree.getIndices();
    for (auto pair : finalTree.getIndices()) {
        auto node = pair.first;
        if (node->isLeaf() && node != finalTree.getNYTNode()) {
            if (indices[node] > maxI) {
                maxI = indices[node];
                mostCommonSymbol = node->getElement().value.value();
            } if (indices[node] < minI) {
                minI = indices[node];
                leastCommonSymbol = node->getElement().value.value();
            }
        }
    }

    double ratio = round(100 * (DECOMPRESS ? (double)inputSize / (double)outputSize : (double)outputSize / (double)inputSize)) / 100;
    std::cout << "input file size    : " << (inputSize != -1 ? std::to_string(inputSize) : "failed to determine size") << (DECOMPRESS ? " (compressed)" : "") << "\n";
    std::cout << "output file size   : " << (outputSize != -1 ? std::to_string(outputSize) : "failed to determine size") << (DECOMPRESS ? "" : " (compressed)") << "\n";
    std::cout << "compression ratio  : " << ratio << "\n";
    std::cout << "final node count   : " << indices.size() << "\n";
    std::cout << "most common symbol : " << std::hex << "0x" << +mostCommonSymbol << " \"" << mostCommonSymbol << "\"" << "\n";
    if (mostCommonSymbol != leastCommonSymbol)
        std::cout << "least common symbol: " << std::hex << "0x" << +leastCommonSymbol << " \"" << leastCommonSymbol << "\"" << "\n";
}

static long getFileSize(std::string file) {
    std::ifstream f;
    f.open(file, std::ifstream::ate | std::ifstream::binary);
    auto size = f.tellg();
    f.close();
    return size;
}

static int encode(std::ifstream& input, std::ofstream& output, HuffmanTree<unsigned char>& tree) {
    std::cout << "compressing..." << std::endl;
    HuffmanEncoder<unsigned char> encoder = HuffmanEncoder<unsigned char>(input, output, tree);
    if (!output.good()) {
        std::cerr << "failed to find / write to " << OUTPUT << std::endl;
        return 1;
    } else {
        // peek() will cause good() to return false if the EOF is reached for instance
        if (input.peek(), input.good())
            encoder.encode();
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
        decoder.decode();
        std::cout << "decompressed " << INPUT << " into " << OUTPUT << std::endl;
        return 0;
    }
}
