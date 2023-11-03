#include <bits/stdc++.h>
using namespace std;

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
    }
};

//TREAP
class Treap{
    private:
    Node* rotateLeft(Node* root);
    Node* rotateRight(Node* root);
    Node* _insert(Node* root, int key, int value, int k);
    Node* _erase(Node* root, int key, int k);
    int _count(Node* node,int time,int cnt);
    vector<int> _Kth(Node* Node, int time, int k);
    int get(Node* node, int value);

    public:
    Node* root;
    Treap(){
        root=nullptr;
    };
    void insert(int key, int value, int k);   
    void erase(int key, int k);               
    int count(int time);
    int Kth(int time, int k);
    void inorderTraversal(Node* root);
    void traverse();
    void print(int time);
};

Node* Treap::rotateLeft(Node* node) {
    Node* newRoot = node->right;
    node->right = newRoot->left;
    newRoot->left = node;

    node->weight=node->left->weight+node->right->weight;
    newRoot->weight=newRoot->left->weight+newRoot->right->weight;

    int lo=node->left->leftover+node->right->weight;
    if(node->right->leftover> lo) {
        lo=node->right->leftover;
    }
    if(lo<=0) node->leftover=0;
    else node->leftover=lo;

    lo=newRoot->left->leftover+newRoot->right->weight;
    if(newRoot->right->leftover> lo) {
        lo=newRoot->right->leftover;
    }
    if(lo<=0) newRoot->leftover=0;
    else newRoot->leftover=lo;

    return newRoot;
}

Node* Treap::rotateRight(Node* node) {
    Node* newRoot = node->left;
    node->left = newRoot->right;
    newRoot->right = node;

    node->weight=node->left->weight+node->right->weight;
    newRoot->weight=newRoot->left->weight+newRoot->right->weight;

    int lo=node->left->leftover+node->right->weight;
    if(node->right->leftover> lo) {
        lo=node->right->leftover;
    }
    if(lo<=0) node->leftover=0;
    else node->leftover=lo;

    lo=newRoot->left->leftover+newRoot->right->weight;
    if(newRoot->right->leftover> lo) {
        lo=newRoot->right->leftover;
    }
    if(lo<=0) newRoot->leftover=0;
    else newRoot->leftover=lo;

    return newRoot;
}

void Treap::insert(int key, int value, int k){                  //k=1 or -1 (weight)
    if (!root) root= new Node (key,value,k);
    else root= _insert(root,key,value,k);
}

Node* Treap::_insert(Node* root, int key, int value, int w) {   //k=weight

    if (root->is_leaf){
        Node* new_node;
        Node* _leaf=new Node(key,value,w);
        int sum = root->weight + w;

         if(key < root->key){
            int lo;

            if (w < root->weight) lo = 1;
            else if (w == root->weight && w > 0)lo = 2;
            else lo = 0;

            new_node =new Node(key, lo, sum, _leaf, root);
         }
        else {
            int lo;
            if (root->weight < w) lo = 1;
            else if (w == root->weight && w > 0) lo = 2;
            else lo = 0;
            new_node = new Node(root->key, lo, sum, root, _leaf);
        }
        return new_node;
    }
    else if (key < root->right->key) { 
        root->left = _insert(root->left, key, value, w);
        if (root->left->priority < root->priority)
            root = rotateRight(root);
    }
    else {
        root->right = _insert(root->right, key, value, w);
        if (root->right->priority < root->priority)
            root = rotateLeft(root);
    }

    int lo=root->left->leftover+root->right->weight;
    if(root->right->leftover> lo) {
        lo=root->right->leftover;
    }
    if(lo<=0) root->leftover=0;
    else root->leftover=lo;

    root->weight += w;
    root->key=root->left->key;
    return root;
}

void Treap::erase(int key, int k){          //k is weight
    if(root) root=_erase(root,key, k);
}

Node* Treap::_erase(Node* root, int key, int k) {         
    int lo=0;
    if (root->is_leaf) return nullptr;
    else if (key<root->right->key){
        root->left=_erase(root->left,key,k);
        if(!root->left) root=root->right;
        else {
            root->weight-=k;
            lo=root->left->leftover+root->right->weight;
            if(lo<=0)root->leftover=0;
            else root->leftover=lo;
            root->key=root->left->key;
        }
    }
    else {
        root->right=_erase(root->right,key,k);
        if(!root->right) root=root->left;
        else{
            root->weight-=k;
            lo=root->left->leftover+root->right->weight;
            if(lo<=0)root->leftover=0;
            else root->leftover=lo;
            root->key=root->left->key;
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
    if (node->is_leaf){
        if(node->key<=time){          //key=time
            cnt+=node->weight;
        }
        return cnt;
    }
    else if(time<node->right->key){         
        return _count(node->left,time,cnt);
    }
    else {
        cnt+=node->left->leftover;
        return _count(node->right,time,cnt);
    }
};

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

void Treap::inorderTraversal(Node* root) {
    if (root) {
        inorderTraversal(root->left);
        if(root->value != INT_MAX)cout << root->key << ":" << root->value << " ";
        inorderTraversal(root->right);
    }
}

void Treap::traverse(){
    inorderTraversal(root);
}

void Treap::print(int time){
    cout<<"Pilha no instante "<<time<< ":"<<endl;
    int i=1;
    int kth = Kth(time,i);
    while (kth!=INT_MAX){
        cout<<"Posicao "<<i<<": "<<kth<<endl;
        i++;
        kth=Kth(time,i);
    };
}

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
        treap->erase(time,1);
    }

    void remove_pop(int time){
        treap->erase(time,-1);
    }

    int query_top(int time){
        return treap->Kth(time,1);
    }

    int query_kth(int time,int k){
        return treap->Kth(time,k);
    }
};

int main(){
    TotalStack totalStack = TotalStack();
    totalStack.add_push(5,10); 
    cout<<"add_push(5,10)"<<endl;
    totalStack.treap->print(10);

    totalStack.add_push(18,15);
    cout<<"add_push(18,15)"<<endl;
    totalStack.treap->print(20);
    
    totalStack.add_push(2,3);
    cout<<"add_push(2,3)"<<endl;
    totalStack.treap->print(10);

    totalStack.add_push(11,7);
    cout<<"add_push(11,7)"<<endl;
    totalStack.treap->print(20);
    totalStack.treap->print(12);

    totalStack.add_pop(20);
    cout<<"add_pop(20)"<<endl;
    totalStack.treap->print(20);

    totalStack.remove_pop(20);
    cout<<"remove_pop(20)"<<endl;
    totalStack.treap->print(20);

    totalStack.remove_push(18);
    cout<<"remove_push(18)"<<endl;
    totalStack.treap->print(20);
}