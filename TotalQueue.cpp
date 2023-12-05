#include <bits/stdc++.h>
using namespace std;

//Utility
vector<int> split(string str){ 
    string s;
    stringstream ss(str);
    vector<int> v;

    while (getline(ss, s, ' ')) {
        v.push_back(stoi(s));
    }

    return v;
}

//STRUCT
struct Node {
    int key;            //for leaves: time, for internal nodes: min_right_time (min time in right subtree)
    int value;          //only for leaves
    int priority;
    Node* left;
    Node* right;

    bool is_leaf; 
    int leaves;         //number of leaves of the subtree with root on this node (for internal nodes)

    Node(int k, int v) {    //Leaf
        key=k;
        value=v;
        left=nullptr;
        right=nullptr;
        priority=0;             //leaves dont have priority
        is_leaf=true;
        leaves=1;             
    }
    Node(int k) {           //internal node
        key=k;              //must update
        value=INT_MAX;        
        left=nullptr;
        right=nullptr;
        priority=rand();
        is_leaf=false;
        leaves=0;           //must update 
    }
};

//TREAP
class Treap{
    private:
    Node* rotateLeft(Node* root);
    Node* rotateRight(Node* root);
    Node* _insert(Node* root, int key, int value);
    Node* _erase(Node* root, int key);
    int _count(Node* node,int time,int cnt);
    Node* _Kth(Node* Node, int k);
    void inorderTraversal(Node* root);

    public:
    Node* root;
    Treap(){
        root=nullptr;
    };
    void insert(int key, int value);    //needs to be changed
    void erase(int key);                //needs to be changed    
    int count(int time);
    int Kth(int time, int k);
    void traverse();
};

Node* Treap::rotateLeft(Node* node) {
    Node* newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;
    return newRoot;
}

Node* Treap::rotateRight(Node* node) {
    Node* newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;
    return newRoot;
}

void Treap::insert(int key, int value){
    root= _insert(root,key,value);
}

Node* Treap::_insert(Node* root, int key, int value) {
    if (!root)
        return new Node(key, value);

    if (key < root->key) {
        root->left = _insert(root->left, key, value);
        if (root->left->priority < root->priority)
            root = rotateRight(root);
    } else {
        root->right = _insert(root->right, key, value);
        if (root->right->priority < root->priority)
            root = rotateLeft(root);
    }

    root->is_leaf=false;
    if(root->right)root->leaves+=root->right->leaves;
    if(root->left)root->leaves+=root->left->leaves;
    return root;
}

void Treap::erase(int key){
    root=_erase(root,key);
}

Node* Treap::_erase(Node* root, int key) {
    if (!root)
        return NULL;

    if (key < root->key)
        root->left = _erase(root->left, key);
    else if (key > root->key)
        root->right = _erase(root->right, key);
    else {
        if (!root->left && !root->right) {
            delete root;
            return NULL;
        } else if (!root->left) {
            Node* temp = root->right;
            delete root;
            return temp;
        } else if (!root->right) {
            Node* temp = root->left;
            delete root;
            return temp;
        } else {
            if (root->left->priority < root->right->priority) {
                root = rotateRight(root);
                root->right = _erase(root->right, key);
            } else {
                root = rotateLeft(root);
                root->left = _erase(root->left, key);
            }
        }
    }

    return root;
}

//returns number of leaves with key<= time
int Treap::count(int time){
    if(root) return (_count(root,time,0));
    return 0;
};

int Treap::_count(Node* node, int time, int cnt){
    if(node->is_leaf){
        if(node->key<=time){
            cnt++;
        }
        return cnt;
    }
    else if(time<node->right->key){         //min_right_time
        return _count(node->left, time, cnt);
    }
    else {
        if (node->left->is_leaf) cnt++;
        //else cnt=cnt+node->left->leaves;
        else cnt = cnt + node->left->leaves;
        return _count(node->right, time, cnt);
    }
};

int Treap::Kth(int time, int k){
    Node* node=_Kth(root,k);
    if (node->key>time) return INT_MAX;
    return node->key;
};

Node* Treap::_Kth(Node* node, int k){
    if(node->is_leaf) return node;
    else if (node->left->is_leaf) {
        if (k==1) return _Kth(node->left,k);
        return (_Kth(node->right,k-1));
    }
    else {
        if (k>node->left->leaves){
            return _Kth(node->right,k-node->left->leaves);
        }
        return _Kth(node->left,k);
    }
};

void Treap::inorderTraversal(Node* root) {
    if (root) {
        inorderTraversal(root->left);
        cout << root->key << ":" << root->value << " ";
        inorderTraversal(root->right);
    }
}

void Treap::traverse(){
    inorderTraversal(root);
}

class TotalQueue{
    public:
    Treap* enqueueTreap;
    Treap* dequeueTreap;

    TotalQueue(){
        enqueueTreap=new Treap();
        dequeueTreap= new Treap();
    }

    void add_enqueue(int time, int value){
        enqueueTreap->insert(time,value);
    }

    void add_dequeue(int time){
        dequeueTreap->insert(time,-1);         
    }
    
    void remove_enqueue(int time) {
        enqueueTreap->erase(time);
    }

    void remove_dequeue(int time){
        dequeueTreap->erase(time);
    }

    int query_first(int time){
        return(query_kth(time,1));
    }

    int query_kth(int time, int k){
        int cnt=dequeueTreap->count(time);
        return enqueueTreap->Kth(time, cnt+k);
    }

    void print_queue(int time){
        cout<<"Pilha no instante "<<time<< ":"<<endl;
        int i=1;
        int kth = query_kth(time,i);
        while (kth!=INT_MAX){
            cout<<kth<< " ";
            i++;
            kth=query_kth(time,i);
        };
        cout<<endl;
    }

    void test(string file){
        ifstream inputFile(file);

        if(!inputFile.is_open()){
            cerr<<"Error opening file"<<endl;
            return;
        }

        string line;
        vector<int> v;
        while (getline(inputFile,line)){
            v=split(line);
            int option = v[0];
            switch(option){
                case 1:
                    add_enqueue(v[1],v[2]);
                    break;
                case 2:
                    add_dequeue(v[1]);
                    break;
                case 3:
                    remove_enqueue(v[1]);
                    break;
                case 4:
                    remove_dequeue(v[1]);
                    break;
                case 5:
                    cout << query_first(v[1]) << endl;
                    break;
                case 6:
                    cout << query_kth(v[1],v[2]) << endl;
                    break;
                case 7:
                    print_queue(v[1]);
                    break;
                case 9:
                    enqueueTreap->traverse();
                    cout << endl;
                    dequeueTreap->traverse();
                    break;
            }
        };
        inputFile.close();
    }
};

int main(){
    string file = "queue.txt";
    TotalQueue totalQ = TotalQueue();
    totalQ.test(file);
}