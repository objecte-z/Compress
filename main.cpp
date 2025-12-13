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
        if (argc < 4)
        {
            cerr << "[Error] Missing input/output file.\n";
            printHelp();
            return 1;
        }

        string in_path = argv[2];
        string out_path = argv[3];
        compress(in_path, out_path);
        cout << "[Infor] Compressing\n" << in_path << "->" << out_path << endl;
        cout << "Compressed successfully\n";

    }
    else if (option == "-d")
    {
        if (argc < 4)
        {
            cerr << "[Error] Missing input/output file.\n";
            printHelp();
            return 1;
        }
        string in_path = argv[2];
        string out_path = argv[3];
        decompress(in_path, out_path);
        cout << "[Infor] Decompressing\n" << in_path << " -> " << out_path << endl;
        cout << "Decompressed successfully\n";
    }
    else
        printHelp();
    return 0;
}