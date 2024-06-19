#include <bits/stdc++.h>
using namespace std;

struct cell{
    int time;
    int value;
    cell* next;
    cell* past;

    cell(int t, int v){
        time=t;
        value=v;
        next=nullptr;
        past=nullptr;
    }

    cell(int t, int v, cell* p,cell* n){
        time=t;
        value=v;
        next=n;
        past=p;
    }
};

struct Node {
    int key;
    int priority;
    cell* value;
    Node* left;
    Node* right;

    Node(int k, cell* v) {
        key=k;
        value=v;
        left=nullptr;
        right=nullptr;
        priority=rand();    
    }
};

class Treap{
    private:
    Node* rotateLeft(Node* root);
    Node* rotateRight(Node* root);
    Node* _insert(Node* root, int key, cell* value);
    Node* _erase(Node* root, int key);
    cell* _search(Node* root, int key);

    public:
    Node* root;
    Treap(){
        root=nullptr;
    };
    void insert(int key, cell* value);
    void erase(int key);
    cell* search(int key);
    void inorderTraversal(Node* root);
};

class linkedList{
    public:
    cell* head;
    cell* last;
    Treap* treap;

    linkedList(){
        head=nullptr;
        last=nullptr;
        treap= new Treap();
    };

    void insert(int time, int value);
    void remove(int time);
    cell* forward(cell* ptr);
    cell* backward(cell*ptr);
    void display();
};

// LINKED LIST

void linkedList::insert(int time, int value){
    cell* parent= treap->search(time);              //returns parent of cell to be added in tree
    cell* my_cell;
    if (parent==nullptr) {                          //if tree is empty
        my_cell = new cell(time,value);
        head=my_cell;
        last=head;
    }
    else if (time<parent->time){                    //if new cell comes before parent in the list
        my_cell=new cell (time,value,parent->past,parent);
        if(parent->past){              
            parent->past->next=my_cell;             //if parent->past exists, fix its next
        }
        parent->past=my_cell;                       //fixing pointers
        //parent->past->next=parent;                  //!!!
        if(parent==head){                           //fixing head           
            head=parent->past;
        }
    }
    else{                                           //if new cell comes after parent in the list
        my_cell=new cell(time,value,parent,parent->next);
        if(parent->next){
            parent->next->past=my_cell;             //if parent->next exists, fix its past
        }
        parent->next=my_cell;                       //fixing pointer
        //parent->next->past=parent;                  //!!!
        if(parent==last){                           //fixing last
            last=parent->next;
        }
    }
    treap->insert(my_cell->time,my_cell);           
};

void linkedList::remove(int time){
    cell* cell = treap->search(time);
    treap->erase(time);
    if (cell==head){                    //fix head if cell to be removed is head
        head=cell->next;
    }
    else{
        cell->past->next=cell->next;   
    }
    if(cell==last){                     //fix last if cell to be removed is last
        last=cell->past;
    }
    else{
        cell->next->past=cell->past;
    }
};

cell* linkedList::forward(cell* ptr){
    return ptr->next;
};

cell* linkedList::backward(cell* ptr){
    return ptr->past;   
};

void linkedList::display(){
    cout<<"LIST"<<endl;
    auto it=head;
    while (it!=nullptr){
        cout<< "time: "<<it->time<<" value "<<it->value<<endl;
        it=it->next;
    }
    
};

// TREAP

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

void Treap::insert(int key, cell* value){
    root= _insert(root,key,value);
}

Node* Treap::_insert(Node* root, int key, cell* value) {
    if (!root)
        return new Node(key, value);

    if (key < root->key) {
        root->left = _insert(root->left, key, value);
        if (root->left->priority < root->priority)              //min treap
            root = rotateRight(root);
    } else {
        root->right = _insert(root->right, key, value);
        if (root->right->priority < root->priority)
            root = rotateLeft(root);
    }

    return root;
}

void Treap::erase(int key){
    root=_erase(root,key);
}

Node* Treap::_erase(Node* root, int key) {
    if (!root)
        return nullptr;

    if (key < root->key)
        root->left = _erase(root->left, key);
    else if (key > root->key)
        root->right = _erase(root->right, key);
    else {
        if (!root->left) {
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

cell* Treap::search(int key){
    return _search(root,key);
}

cell* Treap::_search(Node* root, int key) {
    if (!root)
        return nullptr;

    if (key < root->key){
        if(root->left) return _search(root->left, key);
    }
    else {
        if (root->right) return _search(root->right, key);
    }
    return root->value;
}

void Treap::inorderTraversal(Node* root) {
    if (root) {
        inorderTraversal(root->left);
        cout << root->key << ":" << root->value->value << " ";
        inorderTraversal(root->right);
    }
}

//PARTIAL QUEUE

class PartialQueue{
    public:
    linkedList* list;
    cell* front;
 
    PartialQueue(){
        list=new linkedList();
        front=nullptr;
    }

    void add_enqueue(int time, int value){
        list->insert(time,value);
        if(front==nullptr){         //if queue is empty 
            front=list->head;
        }
        //if new addition happens before front
        else if(time<front->time){   //enqueue added after time if times are equal
            front=list->backward(front);
        }
    }

    void add_dequeue(int time){
        if(front->next!=nullptr) front= list->forward(front);
    }
    
    void remove_enqueue(int time) {
        list->remove(time);
        if(time<=front->time && front->next!= nullptr){
            front=list->forward(front);
        }
    }

    void remove_dequeue(int time){
        if(front->past != nullptr)front=list->backward(front);
    }

    int query_first(){
        return front->value;
    }

    int query_last(){
        return list->last->value;
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
                    cout << query_first() << endl;
                    break;
                case 6:
                    cout << query_last() << endl;
                    break;
            }
        };
        inputFile.close();
    }
};

//Testing the implementation
int main(){
    string file = "queue.txt";
    PartialQueue partQ = PartialQueue();
    partQ.test(file);
}
