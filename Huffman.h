#pragma once
#ifndef HUFFMAN_H
#define HUFFMAN_H

#include<string>
#include<map>
using namespace std;

struct Node
{
    char c;
    int  f;
    unsigned char minc;   // 子树中最小字符，确保同频时顺序固定
    Node* l;
    Node* r;

    Node(char ch, int w_, Node* L = 0, Node* R = 0)
        : c(ch), f(w_), minc((unsigned char)ch), l(L), r(R)
    {
        if (L && R) {
            unsigned char lm = L->minc;
            unsigned char rm = R->minc;
            minc = (lm < rm) ? lm : rm;
        }
    }
};

struct cmp
{
    bool operator()(const Node* a, const Node* b) const
    {
        if (a->f != b->f) return a->f > b->f;   // f 小的优先
        return a->minc > b->minc;               // minc 小的优先
    }
};


Node* buildTree(map<char, int>& f);

void buildCode(Node* n, const string& s, map<char, string>& code);

void freeTree(Node* n);


#endif // !HUFFMAN_H
