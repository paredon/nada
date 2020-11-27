#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <bitset>
#include <iterator>

using namespace std;

struct tree
{  
    string key;
    tree *left;
    tree *right;
    tree(string key)
    {
        this->key = key;
        left, right = NULL;
    }
};

int main(int argc, char *argv[])
{
    remove("decoded.txt");
    map<string, string> huffmantree;
    string line;
    tree *hufftree = NULL, *htree = NULL;
    string placement;

    ifstream file;
    file.open(argv[2]);
    int i = 0;
    while (file >> line)
    {
        if (i%2 == 1)
        {
            huffmantree[placement] = line;
        }
        placement = line;
        i++;
    }
    hufftree = new tree("-1");
    for(map<string, string>::iterator iter = huffmantree.begin(); iter!=huffmantree.end(); ++iter)
    {
        htree = hufftree;
        for (i = 0; i < iter->second.length(); i++)
        {
            if (iter->second[i] == '0')
            {
                if (htree->left == NULL)
                {
                    htree->left = new tree("-1");
                }
                htree = htree->left;
            }
            else
            {
                if (htree->right == NULL)
                {
                    htree->right = new tree("-1");
                }
                htree = htree->right;
            }    
        }
        if (i == iter->second.length())
        {
            htree->key = iter->first;
        }
    }
    file.close();

    bitset<8> bits;
    ofstream decoded("decoded.txt", ios::out | ios::app);
    char chara;
    htree = hufftree;
    ifstream data(argv[1], ios::binary | ios::in);
    while (data.get(chara))
    {
        for (int bit = 7; bit >= 0; bit--)
        {
            if (((chara >> bit) & 1) == 0)
            {
                htree = htree->left;
            }
            else
            {
                htree = htree->right;
            }
            if (htree->key != "-1")
            {
                decoded << htree->key << endl;
                htree = hufftree;
            }  
        }
    }
    data.close();
    return 0;
}