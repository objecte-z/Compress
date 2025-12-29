#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <queue>
#include <vector>
#include"code.h"
#include"help.h"
#include"Huffman.h"

using namespace std;


bool compress(string in_path)
{
    ifstream in(in_path.c_str(), ios::binary);
    if (!in)
    {
        cout << "unable to open input file\n";
        return false;
    }

    string text;
    char ch;
    while (in.get(ch)) text += ch;
    in.close();

    // 2) 统计频率
    map<char, int> f;
    for (size_t i = 0; i < text.size(); ++i)
        f[text[i]]++;

    // 3) 建树 + 码表
    Node* root = buildTree(f);
    map<char, string> code;
    buildCode(root, "", code);


    string out_path = in_path.substr(0, in_path.find_last_of('.')) + ".huf";

    ofstream out(out_path.c_str(), ios::binary);
    if (!out) {
        cout << "unable to open output file\n";
        freeTree(root);
        return false;
    }

    // 4) 写 header
    const char magic[4] = { 'H','F','0','1' };
    int total = (int)text.size();//文本大小
    int sz = (int)f.size();//频率表大小

    out.write(magic, 4);

    size_t dot = in_path.find_last_of('.');
    string orig = (dot != string::npos) ? "_de" + in_path.substr(dot) : "";
    int orig_size = (int)orig.size();

    out.write((char*)&orig_size, sizeof(int));
    if (orig_size > 0)
        out.write(orig.c_str(), orig_size);


    out.write((char*)&total, sizeof(int));
    out.write((char*)&sz, sizeof(int));

    // 5) 写频率表：每项 (char + int)
    map<char, int>::iterator it;
    for (it = f.begin(); it != f.end(); ++it)
    {
        out.write(&it->first, 1);
        out.write((char*)&it->second, sizeof(int));
    }

    // 6) 写数据位流（按8位打包成字节）
    unsigned char byte = 0;
    int bit = 0;
    unsigned char valid = 0;

    for (size_t i = 0; i < text.size(); ++i) {
        string s = code[text[i]];
        for (size_t j = 0; j < s.size(); ++j) {
            byte = (byte << 1) | (s[j] - '0');
            bit++;
            if (bit == 8) {
                out.put((char)byte);
                byte = 0;
                bit = 0;
            }
        }
    }

    // 7) 不足8位则补0，并记录 valid
    if (bit > 0) {
        valid = bit;
        byte <<= (8 - bit);
        out.put((char)byte);
    }

    // 8) 最后写 valid（1字节）
    out.write((char*)&valid, 1);

    out.close();
    freeTree(root);
	return true;
}


bool decompress(string in_path)
{
    ifstream in(in_path.c_str(), ios::binary);
    if (!in) {
        cout << "unable to open input file\n";
        return false;
    }


    // 1) 读 header： magic, total, sz
    char magic[4];
    int total = 0;
    int sz = 0;
    int orig_size = 0;
    string orig = "";


    in.read(magic, 4);
    if (!(magic[0] == 'H' && magic[1] == 'F' && magic[2] == '0' && magic[3] == '1'))
    {
        cout << "package is wrong\n";
        return false;
    }

    if (!in.read((char*)&orig_size, sizeof(int))) 
        return false;
    orig.resize((size_t)orig_size);

    if (orig_size > 0) 
    {
        in.read(&orig[0], orig_size);
        if (!in) 
        { 
            cout << "bad header\n";
            return false; 
        }
    }

    if (!in.read((char*)&total, sizeof(int)) || !in.read((char*)&sz, sizeof(int))) {
        cout << "bad header\n";
        return false;
    }

    // 2) 读频率表
    map<char, int> f;
    for (int i = 0; i < sz; ++i) {
        char c;
        int v;
        in.read(&c, 1);
        in.read((char*)&v, sizeof(int));
        f[c] = v;
    }

    // 3) 记下数据区起点
    streamoff data_begin = (streamoff)in.tellg();

    // 4) 读末尾 valid，并计算最后数据字节位置
    in.seekg(0, ios::end);
    streamoff file_end = (streamoff)in.tellg();
    streamoff pos_valid = file_end - 1;      // valid 的位置
    streamoff pos_lastdata = pos_valid - 1;  // 最后一个数据字节的位置

    in.seekg(pos_valid, ios::beg);
    unsigned char valid = 0;
    in.read((char*)&valid, 1);

    int last_bits = (valid == 0 ? 8 : (int)valid);

    // 5) 回到数据区开始

    in.seekg(data_begin, ios::beg);

    string bits;
    unsigned char b = 0;

    // (1) 读除最后一个数据字节之外的所有字节
    while (in.good() && (streamoff)in.tellg() < pos_lastdata)
    {
        if (!in.read((char*)&b, 1)) break;
        for (int i = 7; i >= 0; --i)
            bits += ((b >> i) & 1) ? '1' : '0';
    }

    // (2) 读最后一个数据字节，只取 last_bits 位
    in.seekg(pos_lastdata, ios::beg);
    if (in.read((char*)&b, 1)) {
        for (int i = 7; i >= 8 - last_bits; --i)
            bits += ((b >> i) & 1) ? '1' : '0';
    }

    in.close();

    // 6) 建树并解码
    Node* root = buildTree(f);
    Node* cur = root;

    string base = in_path.substr(0, in_path.find_last_of('.'));
    string out_path = base + orig;
    ofstream out(out_path.c_str(), ios::binary);

	if (root && !root->l && !root->r)
    {
		for (int i = 0; i < total; ++i)
			out.put(root->c);
		out.close();
		freeTree(root);
		return true;
	}

    if (!out)
    {
        cout << "unable to open output file\n";
        freeTree(root);
        return false;
    }


    int dec = 0;
    for (size_t i = 0; i < bits.size() && dec < total; ++i) {
        cur = (bits[i] == '0') ? cur->l : cur->r;
        if (!cur->l && !cur->r) {
            out.put(cur->c);
            dec++;
            cur = root;
        }
    }

    out.close();
    freeTree(root);
	return true;
}

