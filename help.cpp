#include<iostream>
using namespace std;

void printHelp()
{
    std::cout << "Huffman Compression Tool\n"
        << "Usage:\n"
        << "  huffman [options] <input> \n\n"
        << "Options:\n"
        << "  -c, --compress       Compress the input file\n"
        << "  -d, --decompress     Decompress the input file\n"
        << "  -h, --help           Show this help message\n\n"
        << "Examples:\n"
        << "  huffman -c input.txt\n"
        << "  huffman -d output.huf\n";
}