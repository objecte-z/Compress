#include"Huffman.h"
#include<string>
#include<map>
#include<queue>
using namespace std;

Node* buildTree(map<char, int>& f)
{
    if (f.empty())
        return 0;

    priority_queue<Node*, vector<Node*>, cmp> q;

    map<char, int>::iterator it;
    for (it = f.begin(); it != f.end(); ++it) {
        q.push(new Node(it->first, it->second));
    }

    while (q.size() > 1) {
        Node* a = q.top(); q.pop();
        Node* b = q.top(); q.pop();

        // ¹Ì¶¨×óÓÒ
        if (b->f < a->f || (b->f == a->f && b->minc < a->minc)) {
            Node* t = a; a = b; b = t;
        }

        Node* parent = new Node('\0', a->f + b->f, a, b);
        q.push(parent);
    }

    return q.top();
}



void buildCode(Node* n, const string& s, map<char, string>& code)
{
    if (!n) return;  // ·ÀÖ¹¿ÕÖ¸Õë

    if (!n->l && !n->r)
    {
        if (s.empty())
            code[n->c] = "0";
        else
            code[n->c] = s;
        return;
    }

    buildCode(n->l, s + "0", code);
    buildCode(n->r, s + "1", code);
}


void freeTree(Node* n)
{
    if (!n)
        return;
    freeTree(n->l);
    freeTree(n->r);
    delete n;
}
