#pragma once
#ifndef VALUESTREE_H_
#define VALUESTREE_H_

#include <bits/stdc++.h>
#include "Node.h"
using namespace std;

class ValuesTree {
    private:
    Node* rotateLeft(Node* node) {
        Node* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        return newRoot;
    };

    Node* rotateRight(Node* node) {
        Node* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;
        return newRoot;
    };

    Node* _insert(Node* root, int key) {
        size++;

        if (!root) return new Node(key);

        if (key < root->key) {
            root->left = _insert(root->left, key);
            if (root->left->priority < root->priority)              //min treap
                root = rotateRight(root);
        } else {
            root->right = _insert(root->right, key);
            if (root->right->priority < root->priority)
                root = rotateLeft(root);
        }
        return root;
    };

    Node* _erase(Node* root, int key) {
        size--;

        if (!root) return nullptr;

        if (key < root->key)
            root->left = _erase(root->left, key);
        else if (key > root->key)
            root->right = _erase(root->right, key);
        else {
            if (!root->left) {
                Node* temp = root->right;
                delete root;
                return temp;
            } 
            else if (!root->right) {
                Node* temp = root->left;
                delete root;
                return temp;
            } 
            else {
                if (root->left->priority < root->right->priority) {
                    root = rotateRight(root);
                    root->right = _erase(root->right, key);
                } 
                else {
                    root = rotateLeft(root);
                    root->left = _erase(root->left, key);
                }
            }
        }
        return root;
    };

    Node* _min(Node* node){
        if(node->left) return node->left;
        return node;
    };

    void inorderTraversal(Node* node) {
        if (node) {
            inorderTraversal(node->left);
            cout << node->key << " ";
            inorderTraversal(node->right);
        }
    };

    public:
    Node* root;
    int size;
    
    ValuesTree(){
        root=nullptr;
        size=0;
    };

    void insert(int key) {
        root= _insert(root,key);
    }

    void erase(int key) {
        root=_erase(root,key);
    };

    int min() {
        if (root) return _min(root)->key;
        return INT_MAX;
    }

    void traverse(){
        inorderTraversal(root);
    }
};


#endif
