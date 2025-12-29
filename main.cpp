#include <iostream>
#include <string>
#include"code.h"
#include"help.h"
using namespace std;


int main(int argc, char* argv[])
{

    if (argc < 2)
    {
        printHelp();
        return 0;
    }
    string option = argv[1];


    if (option == "-h")
    {
        printHelp();
        return 0;
    }
    else if (option == "-c")
    {

        if (argc < 3 || argc > 3)
        {
            printHelp();
            return 1;
        }
        string in_path = argv[2];
        if (compress(in_path))
            cout << "Compressed successfully\n";
        else
			cout << "Compression failed\n";

    }
    else if (option == "-d")
    {
        if (argc < 3 || argc > 3)
        {
            printHelp();
            return 1;
        }
        string in_path = argv[2];
        if (decompress(in_path))
            cout << "Decompressed successfully\n";
        else
			cout << "Decompression failed\n";
    }
    else
    {
        printHelp();
    }
 
    return 0;
}