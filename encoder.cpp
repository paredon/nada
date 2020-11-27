#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <bitset>

using namespace std;

struct tree
{
    int value;
    int key;
    tree *left;
    tree *right;
    tree(int value, int key)
    {
        this->value = value;
        this->key = key;
        left = NULL;
        right = NULL;
    }
};

struct node
{
    int value;
    int key;
    tree *root;
};

class FourWay
{
    vector<node> heap;
    int minChild(int parent);
    int nchild(int i, int n);
    int parent(int child);
    void moveUp(int location);
    void moveDown(int location);

    public:
    FourWay() {}

    void insert(int value, int key, tree *root);
    void deleteMin();
    node getMin();
    int fsize();
};

void FourWay::moveUp(int location)
{
    if ((location > 3) && (parent(location) >= 3) && (heap[parent(location)].key > heap[location].key))
    {
        swap(heap[parent(location)], heap[location]);
        moveUp(parent(location));
    }
}

void FourWay::moveDown(int location)
{
    int bestChild = minChild(location);
    if ((bestChild > 3) && (heap[location].key > heap[bestChild].key))
    {
        swap(heap[location], heap[bestChild]);
        moveDown(bestChild);
    }
}

int FourWay::nchild(int i, int n)
{
    int child = 4*(i-3) + (n + 3);
    if (child < heap.size())
    {
        return child;
    }
    else return -1;
}

int FourWay::minChild(int parent)
{
    int k = 1;
    int bestChild = nchild(parent, k);
    k++;
    while (k <= 4)
    {
        int otherChild = nchild(parent, k);
        if ((heap[otherChild].key < heap[bestChild].key) && (otherChild > 3))
        {
            bestChild = otherChild;
        }
        k++;
    }
    return bestChild;
}

int FourWay::parent(int child)
{
    int parent = (child - 4)/4 + 3;
    if (child != 0)
    {
        return parent;
    }
    else return -1;
}

int FourWay::fsize()
{
    return heap.size();
}

node FourWay::getMin()
{
    node node;
    node.value = 0;
    node.key = 0;

    if (heap.size() == 0)
    {        
        return node;
    }
    else return heap[3];    
}

void FourWay::deleteMin()
{
    if (heap.size() == 3)
    {
        return;
    }
    heap[3] = heap.at(heap.size() -1);
    heap.pop_back();
    moveDown(3);
}

void FourWay::insert(int value, int key, tree *root)
{
    if (heap.size() == 0)
    {
        for(int i = 0; i < 3; i++)
        {
            heap.push_back(node());
            heap[heap.size() - 1].value = -1;
            heap[heap.size() - 1].key = -1;
            heap[heap.size() - 1].root = NULL;
        }
    }
    heap.push_back(node());
    heap[heap.size() - 1].value = value;
    heap[heap.size() - 1].key = key;
    heap[heap.size() - 1].root = root;
    moveUp(heap.size() - 1);
}


map<int, string> huffmantree;

void print(string codepart, tree *root)
{
    if (root == NULL) {return;}
    if (root->value >= 0)
    {
        huffmantree[root->value] = codepart;
    }
    
    print(codepart + "1", root->right);
    print(codepart + "0", root->left);

}

void build_fwh(char* path)
{
    
    FourWay fwheap;
    tree *left = NULL, *right = NULL, *root = NULL;
    node alpha, beta;
    int freqs[1000000] = {0};
    int index = -1;
    ifstream file;

    while (huffmantree.empty() == false)
    {
        huffmantree.erase(huffmantree.begin());
    }
    remove( "code_table.txt" );
    remove( "encoded.bin" );
    file.open(path);
    int i;
    while(file >> i)
    {
        freqs[i]++;
    }
    for(i=0; i<1000000; i++)
    {
        if (freqs[i] != 0)
        {
            root = new tree(i, freqs[i]);
            fwheap.insert(i, freqs[i], root);
        } 
    }
    file.close();

    while(fwheap.fsize() > 4)
    {
        
        alpha = fwheap.getMin();
        fwheap.deleteMin();
        left = alpha.root; 
        beta = fwheap.getMin();
        fwheap.deleteMin();
        right = beta.root;
        root = new tree(index, alpha.key + beta.key);
        root->left = left;
        root->right = right;
        fwheap.insert(index--, alpha.key + beta.key, root);
    }
    print("", root);
}

int main(int argc, char *argv[])
{
    build_fwh(argv[1]);
    ifstream file;
    std::ofstream fileout("code_table.txt", std::ios::out | std::ios::app);

    for(std::map<int,string>::iterator iterator = huffmantree.begin(); iterator !=huffmantree.end(); ++iterator)
    {
        fileout << iterator->first << " " << iterator->second << endl;
    }

    string code;
    bitset<8> bits;
    int count = 7,i;
    file.open(argv[1]);
    bits.reset();
    std::ofstream fs("encoded.bin", std::ios::out | std::ios::binary | std::ios::app);
    while (file >> i)
    {
        code = huffmantree[i];
        for (int j=0; j < code.length(); j++)
        {
            if (code[j] != '0')
        
            bits.set(count);
            count--;
            if (count == -1)
            {
                count = 7;
                fs.write((char*)&bits, sizeof(char));
                bits.reset();
            }  
        }
    }
    fs.close();
    file.close();
    
    return 0;
}