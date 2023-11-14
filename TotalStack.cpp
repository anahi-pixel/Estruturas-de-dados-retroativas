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
    int key;            //for leaves: time, for internal nodes: min_time (min time)
    int value;          //only for leaves
    int priority;
    Node* left;
    Node* right;
    int weight;         //1 if push -1 if pop
    int leftover;
    bool is_leaf;
    int size; 

    Node(int k, int v, int w) {    //Leaf
        key=k;
        value=v;
        left=nullptr;
        right=nullptr;
        priority=0;             //leaves do not have priority
        is_leaf=true;
        weight=w; 

        if (w == -1) leftover = 0;
        else leftover = 1;

        size=1;
    }
    
    Node(int k, int lo, int w, Node* l, Node* r){   //internal node
        key=k;
        value=INT_MAX;
        left=l;
        right=r;
        priority=rand();
        weight=w;
        leftover=lo;
        is_leaf=false;
        size=0;
    }
};

//TREAP
class Treap{
    private:
    Node* rotateLeft(Node* node);
    Node* rotateRight(Node* node);
    Node* _insert(Node* node, int key, int value, int k);
    Node* _erase(Node* node, int key);
    int _count(Node* node,int time);
    vector<int> _Kth(Node* node, int time, int k);
    int get(Node* node, int value);
    void update(Node* node);
    int _size(Node* node, int key);

    public:
    Node* root;
    Treap(){
        root=nullptr;
    };
    void insert(int key, int value, int k);   
    void erase(int key);     
    int count(int time);
    int Kth(int time, int k);
    void inorderTraversal(Node* node);
    void traverse();
    int size(int key);
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

void Treap::insert(int key, int value, int k){                  //k=1 or -1 (weight)
    if (!root) root= new Node (key,value,k);
    else root= _insert(root,key,value,k);
}

Node* Treap::_insert(Node* node, int key, int value, int w) {   //w=weight
    if (node->is_leaf){
        Node* new_node;
        Node* _leaf=new Node(key,value,w);
        int sum = node->weight + w;

         if(key < node->key){
            int lo;

            if (w < node->weight) lo = 1;
            else if (w == node->weight && w > 0)lo = 2;
            else lo = 0;

            new_node =new Node(key, lo, sum, _leaf, node);
         }
        else {
            int lo;
            if (node->weight < w) lo = 1;
            else if (w == node->weight && w > 0) lo = 2;
            else lo = 0;
            new_node = new Node(node->key, lo, sum, node, _leaf);
        }
        return new_node;
    }
    else if (key < node->right->key) { 
        node->left = _insert(node->left, key, value, w);
        if (node->left->priority < node->priority)
            node = rotateRight(node);
    }
    else {
        node->right = _insert(node->right, key, value, w);
        if (node->right->priority < node->priority)
            node = rotateLeft(node);
    }
    update(node);
    return node;
}

void Treap::erase(int key){          //k is weight
    if(root) root=_erase(root,key);
}

Node* Treap::_erase(Node* node, int key) {         
    int lo=0;
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

//returns number of leaves with key<= time
int Treap::count(int time){
    if(root) return (_count(root,time));
    return 0;
};

int Treap::_count(Node* node, int time){
    if (node->is_leaf){
        if(node->key<=time){          //key=time
            return node->weight;
        }
        return 0;
    }
    else if(time<node->right->key){         
        return _count(node->left,time);
    }
    else {
        return _count(node->right,time)+ node->left->weight;
    }
};;

int Treap::Kth(int time, int k){
    if (count(time) == 0) { 
        return INT_MAX;        
    }
    int kth = _Kth(root,time, k)[0]; 
    return(kth);
};

vector<int> Treap::_Kth(Node* node,int time, int k){        //k=position
    if (node->is_leaf){
        if (node->weight == -1 || k != 1){
            vector<int> a={INT_MAX,node->weight};  
            return a;
        }
        else{ 
            vector<int> a={node->value};
            return a;
        }
    }
    else if (time < node->right->key){
            return _Kth(node->left, time, k);
    }
    else{
            vector<int> right = _Kth(node->right, time, k);
            if (right[0] == INT_MAX){                              
                k = k- right[1];
                if (node->left->leftover >= k) {
                    vector<int> a = {get(node->left,k)};
                    return a;
                }
                else{
                    vector<int> a = {INT_MAX, right[1] + node->left->weight};
                    return a;
                }
            }
            else return right;
    }
};

int Treap::get(Node* node, int k)  {                            //k=position
    if (node->is_leaf)
            return node->value;
    else if (node->right->leftover >= k)
            return get(node->right, k);
    else
        return get(node->left, k - node->right->weight);
};      

void Treap::inorderTraversal(Node* node) {
    if (node) {
        inorderTraversal(node->left);
        if(node->value != INT_MAX)cout << node->key << ":" << node->value << " ";
        inorderTraversal(node->right);
    }
}

void Treap::traverse(){
    inorderTraversal(root);
}

void Treap::update(Node* node){
    if(node->is_leaf) return;

    node->size=node->right->size+node->left->size+1;

    int lo=node->left->leftover+node->right->weight;
    if(node->right->leftover> lo) {
        lo=node->right->leftover;
    }

    node->leftover=lo;

    node->weight=node->left->weight+node->right->weight;

    root->key=root->left->key;
}

int Treap::size(int key){
    if (count(key) == 0) { 
        return INT_MAX;        
    }
    int size = _size(root,key); 
    return(size);
}

int Treap::_size(Node* node,int key){        
    if (node->is_leaf){
        return node->weight;
    }
    else if (key < node->right->key){
            return _size(node->left, key);
    }
    else{
            int right = _size(node->right,key);
            right+=node->left->weight;
            return right;
    }
};

class TotalStack{
    public:  
    Treap* treap;  

    TotalStack(){
        treap= new Treap();
    }

    void add_push(int time, int value){
        treap->insert(time,value,1);
    }
  
    void add_pop(int time){
        treap->insert(time,INT_MAX,-1);
    }
    
    void remove_push(int time) {
        treap->erase(time);
    }

    void remove_pop(int time){
        treap->erase(time);
    }

    int query_top(int time){
        return treap->Kth(time,1);
    }

    int query_kth(int time,int k){
        return treap->Kth(time,k);
    }

    int query_size(int time){
        return treap->size(time);
    }

    void print_stack(int time){
        //cout<<"Pilha no instante "<<time<< ":"<<endl;
        int i=1;
        int kth = treap->Kth(time,i);
        while (kth!=INT_MAX){
            cout<<kth<< " ";
            i++;
            kth=treap->Kth(time,i);
        };
        cout<<endl;
    }

    void test(){
        ifstream inputFile("stack.txt");

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
                    add_push(v[1],v[2]);
                    break;
                case 2:
                    add_pop(v[1]);
                    break;
                case 3:
                    remove_push(v[1]);
                    break;
                case 4:
                    cout<<query_size(v[1])<<endl;
                    break;
                case 5:
                    cout<<query_top(v[1])<<endl;
                    break;
                case 6:
                    cout<<query_kth(v[1],v[2])<<endl;
                    break;
                case 7:
                    print_stack(v[1]);
                    break;
            }
        };
        inputFile.close();
    }
};

int main(){
    TotalStack totalStack = TotalStack();
    totalStack.test();
}
