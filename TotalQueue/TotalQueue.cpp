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
    int key;            //for leaves: time, for internal nodes: min_time (min time in subtree)
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
    Node(int k, int s, Node* l, Node* r) {           //internal node
        key=k;            
        value=INT_MAX;        
        left=l;
        right=r;
        priority=rand();
        is_leaf=false;
        leaves=s;            
    }
};

//TREAP
class Treap{
    private:
    Node* rotateLeft(Node* node);
    Node* rotateRight(Node* node);
    Node* _insert(Node* node, int key, int value);
    Node* _erase(Node* node, int key);
    int _count(Node* node,int time,int cnt);
    Node* _Kth(Node* node, int k);
    void inorderTraversal(Node* node, int a);
    void update(Node* node);

    public:
    Node* root;
    Treap(){
        root=nullptr;
    };
    void insert(int key, int value);    
    void erase(int key);                  
    int count(int time);
    int Kth(int time, int k);
    void traverse();
};

Node* Treap::rotateLeft(Node* node) {
    Node* newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;

    update(node);
    update(newRoot);

    return newRoot;
}

Node* Treap::rotateRight(Node* node) {
    Node* newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;

    update(node);
    update(newRoot);

    return newRoot;
}

void Treap::insert(int key, int value){
    if (!root) root= new Node (key,value);
    else root= _insert(root,key,value);
}

Node* Treap::_insert(Node* node, int key, int value) {
    if (node->is_leaf){
        Node* new_node;
        Node* _leaf=new Node(key,value);
        int sum=node->leaves+_leaf->leaves;
        if(key < node->key){
            new_node =new Node(key, sum, _leaf, node);
        }
        else {
            new_node = new Node(node->key, sum, node, _leaf);
        }
        return new_node;
    }
    else if (key < node->right->key) {
        node->left = _insert(node->left, key, value);
        if (node->left->priority < node->priority)
            node = rotateRight(node);
    } 
    else {
        node->right = _insert(node->right, key, value);
        if (node->right->priority < node->priority)
            node = rotateLeft(node);
    }
    update(node);
    return node;
}

void Treap::erase(int key){
    if (root) root=_erase(root,key);
}

Node* Treap::_erase(Node* node, int key) {
    if (node->is_leaf) return nullptr;
    else if (key<node->right->key){
        node->left=_erase(node->left,key);
        if(!node->left) node=node->right;
        else {
            update(node);
        }
    }
    else {
        node->right=_erase(node->right,key);
        if(!node->right) node=node->left;
        else{
            update(node);
        }
    }
    return node;
}

//update node fields
void Treap::update(Node* node){
    if(!node->right) {
        node->is_leaf=true;
        return;
    }

    node->is_leaf=false;

    node->leaves=node->right->leaves+node->left->leaves;

    node->key=node->left->key;
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
    return node->value;
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

void Treap::inorderTraversal(Node* node, int a) {
    if (node) {
        inorderTraversal(node->left,a+3);
        for (int i = 0; i < a; i++)
        {
            cout<< " ";
        }
        cout<< node->key << ":" << node->value << " " << node <<endl;
        inorderTraversal(node->right,a+3);
    }
}

void Treap::traverse(){
    inorderTraversal(root,0);
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
        cout <<"Fila no instante "<<time<< ":"<<endl;
        int size=enqueueTreap->count(time)-dequeueTreap->count(time);
        for (int i = 1; i <= size; i++)
        {
            cout<<query_kth(time,i)<< " ";
        }
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
                case 8:
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
