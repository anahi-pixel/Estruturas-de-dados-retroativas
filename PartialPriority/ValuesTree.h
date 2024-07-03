#pragma once
#ifndef PARTIALPRIORITY_H_
#define PARTIALPRIORITY_H_

#include <bits/stdc++.h>
#include "ValuesTree.h"
#include "OperationsTree.h"

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

//Partial priority queue
class PartialPriority{
    public:  
    ValuesTree* q_now;
    ValuesTree* q_del;
    Treap* updates_tree;  

    PartialPriority(){
        q_now= new ValuesTree();
        q_del= new ValuesTree();
        updates_tree=new Treap();
    }

    void add_insert(int time, int value){
        int q_now_value;
        int time_bridge = updates_tree->lastBridgeBefore(time);
        Node* max_bridge = updates_tree->maxAfterBridge(time_bridge);
        if(max_bridge == nullptr || value>max_bridge->value){
            updates_tree->insert(time,value,0);
            q_now_value=value;
        }
        else {
            updates_tree->setWeightZero(max_bridge->key);
            updates_tree->insert(time,value,1);
            q_now_value=max_bridge->value;
            q_del->insert(value);
            q_del->erase(max_bridge->value);
        }
        q_now->insert(q_now_value);             //nodes in q_now do not have weights, only in updates_tree
    };

    void add_delete_min(int time){
        int time_bridge = updates_tree->firstBridgeAfter(time);
        Node* min_bridge = updates_tree->minBeforeBridge(time_bridge);
        updates_tree->setWeightOne(min_bridge->key);
        updates_tree->insert(time,min_bridge->value,-1);
        q_now->erase(min_bridge->value); 
        q_del->insert(min_bridge->value);          
    };

    void remove_insert(int time) {
        int q_now_value;
        Node* delete_node=updates_tree->search(time);
        if (delete_node->weight==0){
            q_now_value=delete_node->value;
        }
        else if (delete_node->weight==1) {
            int time_bridge = updates_tree->firstBridgeAfter(time);
            Node* min_bridge = updates_tree->minBeforeBridge(time_bridge);
            updates_tree->setWeightOne(min_bridge->key);
            q_now_value=min_bridge->value;
        }
        updates_tree->erase(time);
        q_now->erase(q_now_value);
        q_del->insert(q_now_value);   
    };

    void remove_delete_min(int time){
        int time_bridge = updates_tree->lastBridgeBefore(time-1);
        Node* max_bridge = updates_tree->maxAfterBridge(time_bridge);
        updates_tree->setWeightZero(max_bridge->key);
        updates_tree->erase(time);
        q_now->insert(max_bridge->value);
        q_del->erase(max_bridge->value);

    };

    int query_min(){
        return q_now->min();
    };

    int query_size(){
        return q_now->size;
    };

    void print_queue(){
        cout<<"Fila de prioridade no instante atual: "<<endl;
        q_now->traverse();
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
        int i=0;
        while (getline(inputFile,line)){
            v=split(line);
            int option = v[0];
            switch(option){
                case 1:
                    add_insert(v[1],v[2]);
                    break;
                case 2:
                    add_delete_min(v[1]);
                    break;
                case 3:
                    remove_insert(v[1]);
                    break;
                case 4:
                    remove_delete_min(v[1]);
                    break;
                case 5:
                    cout << query_min() << endl;
                    break;
                case 6:
                    cout<< query_size() <<endl;
                    break;
                case 7:
                    i++;
                    cout<<i<<" ";
                    print_queue();
                    break;
                case 8:
                    updates_tree->traverse();
                    cout << endl;
                    break;
            }
        };
        inputFile.close();
    }
};

#endif
