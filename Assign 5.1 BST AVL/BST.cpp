#include "BST.h"
#include <iostream>
#include <functional>

using namespace std;

// Constructor initializes the root to nullptr
BST::BST() : root(nullptr) {}

// Destructor calls clear to delete all nodes
BST::~BST() {
    clear();
}

// Clear the tree by deleting all nodes
void BST::clear() {
    // Use a lambda function for post-order traversal to delete nodes
    function<void(Node*)> deleteNodes = [&](Node* node) {
        if (node) {
            deleteNodes(node->left);     // Delete left subtree
            deleteNodes(node->right);    // Delete right subtree
            delete node;                 // Delete current node
        }
        };
    deleteNodes(root);  // Start deletion from the root
    root = nullptr;     // Set root to nullptr after deletion
}

// Utility function to get the height of a node
int BST::height(Node* node) {
    return node ? node->height : 0;
}

// Calculate the balance factor of a node
int BST::getBalance(Node* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

// Perform a right rotation around node y
BST::Node* BST::rightRotate(Node* y) {
    Node* x = y->left;         // x is y's left child
    Node* T2 = x->right;       // T2 is x's right subtree

    // Perform rotation
    x->right = y;              // x's right child becomes y
    y->left = T2;              // y's left child becomes T2

    // Update heights
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Return new root after rotation
    return x;
}

// Perform a left rotation around node x
BST::Node* BST::leftRotate(Node* x) {
    Node* y = x->right;        // y is x's right child
    Node* T2 = y->left;        // T2 is y's left subtree

    // Perform rotation
    y->left = x;               // y's left child becomes x
    x->right = T2;             // x's right child becomes T2

    // Update heights
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Return new root after rotation
    return y;
}

// Recursive function to insert data into the AVL tree
BST::Node* BST::add(Node* node, int data) {
    // Perform normal BST insertion
    if (node == nullptr)
        return new Node(data);

    if (data == node->data) {
        // Duplicate data not allowed
        cout << "Duplicate data " << data << " not allowed." << endl;
        return node;  // Do not insert the duplicate
    }
    else if (data < node->data) {
        // Insert into the left subtree
        node->left = add(node->left, data);
    }
    else {
        // Insert into the right subtree
        node->right = add(node->right, data);
    }

    // Update height of this ancestor node
    node->height = 1 + max(height(node->left), height(node->right));

    // Get the balance factor to check if this node became unbalanced
    int balance = getBalance(node);

    // Balance the tree if unbalanced

    // Left Left Case
    if (balance > 1 && data < node->left->data)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && data > node->right->data)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && data > node->left->data) {
        node->left = leftRotate(node->left);  // Left rotate left child
        return rightRotate(node);             // Right rotate current node
    }

    // Right Left Case
    if (balance < -1 && data < node->right->data) {
        node->right = rightRotate(node->right);  // Right rotate right child
        return leftRotate(node);                 // Left rotate current node
    }

    // Return the (unchanged) node pointer
    return node;
}

// Public method to add data to the AVL tree
void BST::add(int data) {
    root = add(root, data);
}

// Find the node with minimum value (used in deletion)
BST::Node* BST::minValueNode(Node* node) {
    Node* current = node;
    // Loop down to find the leftmost leaf
    while (current->left != nullptr)
        current = current->left;
    return current;
}

// Recursive function to delete a node with given data
BST::Node* BST::remove(Node* node, int data) {
    if (node == nullptr) {
        cout << "Value " << data << " not found in BST." << endl;
        return node;
    }

    // Perform standard BST delete
    if (data < node->data) {
        // Data is in the left subtree
        node->left = remove(node->left, data);
    }
    else if (data > node->data) {
        // Data is in the right subtree
        node->right = remove(node->right, data);
    }
    else {
        // Node with the data found

        // Node with only one child or no child
        if ((node->left == nullptr) || (node->right == nullptr)) {
            Node* temp = node->left ? node->left : node->right;

            // No child case
            if (temp == nullptr) {
                temp = node;
                node = nullptr;
            }
            else {
                // One child case: copy the child to current node
                *node = *temp;
            }
            delete temp;  // Delete the old node
        }
        else {
            // Node with two children:
            // Get the inorder successor (smallest in the right subtree)
            Node* temp = minValueNode(node->right);

            // Copy the inorder successor's data to this node
            node->data = temp->data;

            // Delete the inorder successor
            node->right = remove(node->right, temp->data);
        }
    }

    // If the tree had only one node then return
    if (node == nullptr)
        return node;

    // Update height of the current node
    node->height = 1 + max(height(node->left), height(node->right));

    // Get the balance factor to check if this node became unbalanced
    int balance = getBalance(node);

    // Balance the tree if unbalanced

    // Left Left Case
    if (balance > 1 && getBalance(node->left) >= 0)
        return rightRotate(node);

    // Left Right Case
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = leftRotate(node->left);   // Left rotate left child
        return rightRotate(node);              // Right rotate current node
    }

    // Right Right Case
    if (balance < -1 && getBalance(node->right) <= 0)
        return leftRotate(node);

    // Right Left Case
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rightRotate(node->right);  // Right rotate right child
        return leftRotate(node);                 // Left rotate current node
    }

    return node;
}

// Public method to remove data from the AVL tree
void BST::remove(int data) {
    if (root == nullptr) {
        cout << "Cannot remove from an empty BST." << endl;
        return;
    }
    root = remove(root, data);
}

// Recursive function to find a node with given data
BST::Node* BST::find(Node* node, int data) {
    if (node == nullptr || node->data == data)
        return node;  // Node found or reached end without finding
    if (data < node->data)
        return find(node->left, data);  // Search in left subtree
    else
        return find(node->right, data); // Search in right subtree
}

// Public method to find data in the AVL tree (returns bool)
bool BST::find(int data) {
    return find(root, data) != nullptr;
}

// Find the maximum value in the AVL tree
int BST::maximum() {
    if (root == nullptr) {
        cout << "BST is empty." << endl;
        return -1; // Indicate error or empty tree
    }
    Node* current = root;
    // Traverse to the rightmost node
    while (current->right != nullptr)
        current = current->right;
    return current->data;
}

// Recursive function for inorder traversal
void BST::inorderTraverse(Node* node) {
    if (node) {
        inorderTraverse(node->left);     // Visit left subtree
        cout << node->data << " ";       // Visit node
        inorderTraverse(node->right);    // Visit right subtree
    }
}

// Public method to perform inorder traversal starting from root
void BST::inorderTraverse() {
    inorderTraverse(root);
    cout << endl;
}
