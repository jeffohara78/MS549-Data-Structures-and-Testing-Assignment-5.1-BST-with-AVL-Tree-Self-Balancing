#pragma once
#ifndef BST_H
#define BST_H

#include <iostream>

using namespace std;

class BST {
private:
    // Definition of a tree node (private, not accessible outside the class)
    struct Node {
        int data;       // The value stored in the node
        Node* left;     // Pointer to the left child
        Node* right;    // Pointer to the right child
        int height;     // Height of the node for balancing
        // Constructor to initialize a node
        Node(int val) : data(val), left(nullptr), right(nullptr), height(1) {}
    };
    Node* root;         // Pointer to the root of the tree

    // Helper functions for AVL tree operations
    Node* add(Node* node, int data);         // Recursive function to insert data
    Node* remove(Node* node, int data);      // Recursive function to remove data
    Node* find(Node* node, int data);        // Recursive function to find data
    int height(Node* node);                  // Utility to get the height of a node
    int getBalance(Node* node);              // Get the balance factor of a node
    Node* rightRotate(Node* y);              // Perform a right rotation
    Node* leftRotate(Node* x);               // Perform a left rotation
    Node* minValueNode(Node* node);          // Find the node with minimum value
    void inorderTraverse(Node* node);        // Recursive inorder traversal

public:
    BST();                                   // Constructor
    ~BST();                                  // Destructor
    void add(int data);                      // Public method to add data
    void remove(int data);                   // Public method to remove data
    bool find(int data);                     // Public method to find data (returns bool)
    int maximum();                           // Return the maximum value in the tree
    void inorderTraverse();                  // Inorder traversal starting from root
    void clear();                            // Clear the tree
};

#endif // BST_H
