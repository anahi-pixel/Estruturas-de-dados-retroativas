#pragma once
#ifndef OPERATIONSTREE_H_
#define OPERATIONSTREE_H_

#include <bits/stdc++.h>
#include "Node.h"
using namespace std;

//first bridge after

class Treap{
    private:
    void update(Node* node);
    Node* rotateLeft(Node* root);
    Node* rotateRight(Node* root);
    Node* _insert(Node* root, int key, int value, int k);
    Node* _erase(Node* root, int key);
    Node* _search(Node* root, int key);
    int  min_right_time(Node* node);
    Node* _min(Node* node);
    vector<int> _lastBridgeBefore(Node* node,int key,int cnt);
    vector<int> _firstBridgeAfter(Node* node,int key,int cnt);
    int getBridgeBefore(Node* node, int k);
    int getBridgeAfter(Node* node, int k);
    Node* _maxAfterBridge(Node* node, int time, int max);
    Node* _minBeforeBridge(Node* node, int time, int max);
    Node* getNode(Node* node, int m, bool type);
    void _setWeightZero(Node* node, int time,int value);
    void _setWeightOne(Node* node, int time,int value);

    public:
    Node* root;
    Treap(){
        root=nullptr;
    };
    void insert(int key, int value, int w);   
    void erase(int key); 
    Node* search(int key);
    int lastBridgeBefore(int time);      
    int firstBridgeAfter(int time);
    Node* maxAfterBridge(int time);
    Node* minBeforeBridge(int time);
    void setWeightZero(int time);
    void setWeightOne(int time);
    void inorderTraversal(Node* root, int a);
    void traverse();
};

void Treap::update(Node* node){
    if(node->is_leaf) return;

    int lo=node->left->smax+node->right->weight;
    if(node->right->smax> lo) {
        lo=node->right->smax;
    }

    node->smax=lo;

    lo=node->right->pmax+node->left->weight;
    if(node->left->pmax> lo) {
        lo=node->left->pmax;
    }

    node->pmax=lo;

    if (node->right->min_in<node->left->min_in) node->min_in=node->right->min_in;
    else node->min_in=node->left->min_in;
    if (node->right->max_out>node->left->max_out) node->max_out=node->right->max_out;
    else node->max_out=node->left->max_out;

    node->weight=node->left->weight+node->right->weight;

    node->key=node->left->key;
}

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

void Treap::insert(int key, int value, int w){                  //w=1 or -1 (weight)
    if (!root) root= new Node (key,value,w);
    else root= _insert(root,key,value,w);
}

Node* Treap::_insert(Node* node, int key, int value, int w) {   //w=weight
    if (node->is_leaf){
        Node* new_node;
        Node* _leaf=new Node(key,value,w);
        int sum = node->weight + w;

         if(key < node->key){
            /*int lo;

            if (w < node->weight) lo = 1;
            else if (w == node->weight && w > 0)lo = 2;
            else lo = 0;

            new_node =new Node(key, lo, sum, _leaf, node);*/
            new_node =new Node(key, sum, _leaf, node);
         }
        else {
            /*int lo;
            if (node->weight < w) lo = 1;
            else if (w == node->weight && w > 0) lo = 2;
            else lo = 0;
            new_node = new Node(node->key, lo, sum, node, _leaf);*/
            new_node = new Node(node->key, sum, node, _leaf);
        }
        update(new_node);
        return new_node;
    }
    else if (key < node->right->key) { 
        node->left = _insert(node->left, key, value, w);
        //update(node);
        if (node->left->priority < node->priority)
            node = rotateRight(node);
    }
    else {
        node->right = _insert(node->right, key, value, w);
        //update(node);
        if (node->right->priority < node->priority)
            node = rotateLeft(node);
    }
    update(node);
    return node;
}

void Treap::erase(int key){          //talvez precise levar em consideração peso?
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

Node* Treap::search(int key){
    return _search(root,key);
}

Node* Treap::_search(Node* node, int key) {
    if (!node)
        return nullptr;

    if (key < min_right_time(node)){
        if(node->left) return _search(node->left, key);
    }
    else {
        if (node->right) return _search(node->right, key);
    }
    return node;
}

int Treap::min_right_time(Node* node) {
    if (node->right) return (_min(node->right)->key);
    return node->key;
}

Node* Treap::_min(Node* node){
        if(node->left) {
            return _min(node->left);
        }
        return node;
};

void Treap::setWeightZero(int time){    //ori
    int value;
    if (root) _setWeightZero(root,time,value);
    return;
}

void Treap::_setWeightZero(Node* node, int time, int value){    //era max bridge (max out>bridge)
    if (node->is_leaf){
        node->weight=0;
        node->max_out=-INT_MAX;
        node->min_in=node->value;
        value=node->value;
        return;
    }
    else if (time < node->right->key){
        _setWeightZero(node->left,time,value);
        /*if(node->max_out==value) {
            if(node->left->value==value){
                node->max_out=node->right->max_out;
            }
            else {
                int max=node->right->max_out;
                if (node->left->max_out>node->right->max_out) max=node->left->max_out;
                node->max_out=max;
            }
        }
        if(value<node->min_in) node->min_in=value;*/
    }
    else{
        _setWeightZero(node->right,time,value);
        /*if(node->max_out==value) {
            if(node->right->value==value){
                node->max_out=node->left->max_out;
            }
            else {
                int max=node->right->max_out;
                if (node->left->max_out>node->right->max_out) max=node->left->max_out;
                node->max_out=max;
            }
        }
        if(value<node->min_in) node->min_in=value;*/
    }

    /*if (node->right->min_in<node->left->min_in) node->min_in=node->right->min_in;
    else node->min_in=node->left->min_in;
    if (node->right->max_out>node->left->max_out) node->max_out=node->right->max_out;
    else node->max_out=node->left->max_out;
    node->weight=node->right->weight+node->left->weight;*/
    update(node);
    return;
}

void Treap::setWeightOne(int time){    
    int value;
    if (root) _setWeightOne(root,time,value);
    return;
}

void Treap::_setWeightOne(Node* node,int time, int value){     
    if (node->is_leaf){
        node->weight=1;
        node->max_out=node->value;
        node->min_in=INT_MAX;
        value=node->value;
        return;
    }
    else if (time < node->right->key){
        _setWeightOne(node->left,time,value);
    }
    else{
        _setWeightOne(node->right,time,value);
    }
    
    /*if (node->right->min_in<node->left->min_in) node->min_in=node->right->min_in;
    else node->min_in=node->left->min_in;
    if (node->right->max_out>node->left->max_out) node->max_out=node->right->max_out;
    else node->max_out=node->left->max_out;
    node->weight=node->right->weight+node->left->weight;*/
    update(node);
    return;
}

//bridge = any moment in which the set of keys in the queue is contained in the current set of keys
int Treap::lastBridgeBefore(int time){      //ori
    int cnt=0;
    if (root) {
        int bridge=_lastBridgeBefore(root,time,cnt)[0];
        if (bridge==INT_MAX) return time;
        return bridge;
    }
    return time;
}

vector<int> Treap::_lastBridgeBefore(Node* node,int key, int cnt){        
    if (node->is_leaf){
        if (node->key <= key) {
            cnt+=node->weight;
            if(cnt==0) {                                            //found spot
                return {node->key};
            }
            else {
                cnt-=node->weight;
            } ;                                
        }
        return {INT_MAX,cnt};                                       //not found
    }
    else if (key < node->right->key){
            return  _lastBridgeBefore(node->left,key,cnt);
    }
    else{
            cnt=cnt+node->left->weight;
            vector<int> right=_lastBridgeBefore(node->right,key,cnt);
            if (right[0]==INT_MAX) {                                //if spot hasn't been found yet
                if(node->left->smax>=right[1]){
                    return {
                        getBridgeBefore(node->left,right[1])
                    };
                }
                else{
                    return {INT_MAX, right[1]-node->left->weight};
                }
            }
            else return right;                                      //if found, return
    }
};

int Treap::getBridgeBefore(Node* node, int cnt)  {                
    if (node->is_leaf)
            return node->key;                                     
    else if (node->right->smax >= cnt)
            return getBridgeBefore(node->right, cnt);
    else
        return getBridgeBefore(node->left, cnt - node->right->weight);
}; 

int Treap::firstBridgeAfter(int time){
    int cnt=0;
    if (root) return(_firstBridgeAfter(root,time,cnt)[0]);
    return INT_MAX;
}

vector<int> Treap::_firstBridgeAfter(Node* node,int key, int cnt){        
    if (node->is_leaf){
        if (node->key >= key) {
            cnt+=node->weight;
            if(cnt==0) {                                            //found spot
                return {node->key};
            }
            /*else {
                cnt-=node->weight;
            } ;  */                              
        }
        return {INT_MAX,cnt};                                       //not found
    }
    else if (key < node->right->key){          
        //cnt=cnt+node->right->weight;
        vector<int> left= _firstBridgeAfter(node->left,key,cnt);
        if (left[0]==INT_MAX) {
            if(node->right->smax==0 && -node->right->weight>=left[1]){
                return {
                    getBridgeAfter(node->right,left[1])
                };
            }
            else {
                return {INT_MAX,left[1]+node->right->weight};
            }
        }
        else return left;
    }
    else{
        cnt=cnt+node->left->weight;
        return (_firstBridgeAfter(node->right,key,cnt));
    }
};

int Treap::getBridgeAfter(Node* node, int cnt)  {                
    if (node->is_leaf)
            return node->key;                                     
    else if (node->right->smax == 0 && -node->left->weight >= cnt)
            return getBridgeAfter(node->left, cnt);
    else
        return getBridgeAfter(node->right, cnt + node->right->weight);
}; 

//returns the maximum element outside of q_now inserted after or at time 
Node* Treap::maxAfterBridge(int time){  //ori
    int max=-INT_MAX;
    //Node* result;
    if (root && root->max_out!=-INT_MAX){
        return _maxAfterBridge(root, time, max);
        //result = _maxAfterBridge(root, time, max);
        //if (max==-INT_MAX) return nullptr;
        //else return result;
    } 
    else return nullptr;
}

Node* Treap::_maxAfterBridge(Node* node, int time, int max){
    if (node->is_leaf){
        if (node->weight==1) {
            if (node->max_out > max) {
                max = node->max_out;
                return node;
            }
        }
        return nullptr;
    }
    else if (time < node->right->key){
        if (node->right->max_out > max) max = node->right->max_out;
        Node* left=_maxAfterBridge(node->left,time,max);
        if (left!=nullptr){
            return left;
        }   
        if(node->right->max_out==max  && max!= -INT_MAX) {
            //cout<<"a"<<endl;
            return(getNode(node->right,max,true));
        }
        return left;
    }
    else{
        return(_maxAfterBridge(node->right,time, max));
    }
}

Node* Treap::minBeforeBridge(int time){
    int min=INT_MAX;
    if (root) return _minBeforeBridge(root, time, min);
    else return nullptr;
}

Node* Treap::_minBeforeBridge(Node* node, int time, int min){
    if (node->is_leaf){
        return nullptr;
    }
    else if (time < node->right->key){
        return(_minBeforeBridge(node->left,time, min));
    }
    else{
        if (node->left->min_in < min) min = node->left->min_in;
        Node* right=_minBeforeBridge(node->right,time,min);   
        if(right!=nullptr){
            return right;
        }
        if(node->left->min_in==min && min != INT_MAX) {
            return(getNode(node->left,min,false));
        }
        return right;
    }
}

Node* Treap::getNode(Node* node, int m, bool type){
    if (type){
        if(node->is_leaf) return node;
        else if (node->right->max_out==m) return getNode(node->right,m,type);
        else return getNode(node->left,m,type);
    }
    else {
        if(node->is_leaf) return node;
        else if (node->right->min_in==m) return getNode(node->right,m,type);
        else return getNode(node->left,m,type);
    } 
}

void Treap::inorderTraversal(Node* node, int a) {
    if (node) {
        inorderTraversal(node->left,a+3);
        for (int i = 0; i < a; i++)
        {
            cout<< " ";
        }
        cout<< node->key << ":" << node->weight << " " << node->smax << " " << node->value << endl;
        inorderTraversal(node->right,a+3);
    }
}

void Treap::traverse(){
    inorderTraversal(root,0);
}

#endif
