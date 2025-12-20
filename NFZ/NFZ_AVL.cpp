/**
 * No-Fly Zone (NFZ) Database Implementation
 * 
 * AVL Tree implementation for storing and querying No-Fly Zone coordinates.
 * 
 * DSA Course Project - GIKI Institute
 */

#include "NFZ_AVL.h"
#include <iostream>

using namespace std;

/**
 * NFZNode structure
 * 
 * Represents a single node in the AVL tree storing a No-Fly Zone coordinate.
 */
struct NFZNode {
    int x;              // Grid row coordinate
    int y;              // Grid column coordinate
    int height;         // Height of subtree rooted at this node
    NFZNode* left;      // Left child
    NFZNode* right;     // Right child
    
    NFZNode(int xVal, int yVal) 
        : x(xVal), y(yVal), height(1), left(nullptr), right(nullptr) {}
};

// ============ CONSTRUCTOR & DESTRUCTOR ============

NFZDatabase::NFZDatabase() : root(nullptr) {}

NFZDatabase::~NFZDatabase() {
    clearHelper(root);
    root = nullptr;
}

// ============ HELPER FUNCTIONS ============

/**
 * Get height of a node (returns 0 for nullptr)
 */
int NFZDatabase::getHeight(NFZNode* node) const {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

/**
 * Calculate balance factor of a node
 * Balance = height(left) - height(right)
 * 
 * Returns:
 *   > 1: left-heavy (needs right rotation)
 *   < -1: right-heavy (needs left rotation)
 *   -1 to 1: balanced
 */
int NFZDatabase::getBalance(NFZNode* node) const {
    if (node == nullptr) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

/**
 * Update height of a node based on its children
 */
void NFZDatabase::updateHeight(NFZNode* node) {
    if (node == nullptr) return;
    
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    
    // Use ternary operator instead of STL max()
    node->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

// ============ ROTATION FUNCTIONS ============

/**
 * Right Rotation (LL case)
 * 
 * Performs a right rotation to fix left-left imbalance.
 * 
 *     node              newRoot
 *    /    \            /      \
 * newRoot  C    ->    A      node
 *  /   \                     /   \
 * A     B                   B     C
 */
NFZNode* NFZDatabase::rotateRight(NFZNode* node) {
    NFZNode* newRoot = node->left;
    NFZNode* temp = newRoot->right;
    
    // Perform rotation
    newRoot->right = node;
    node->left = temp;
    
    // Update heights
    updateHeight(node);
    updateHeight(newRoot);
    
    return newRoot;
}

/**
 * Left Rotation (RR case)
 * 
 * Performs a left rotation to fix right-right imbalance.
 * 
 * node                    newRoot
 *  /  \                   /      \
 * A  newRoot      ->    node      C
 *      /  \             /   \
 *     B    C           A     B
 */
NFZNode* NFZDatabase::rotateLeft(NFZNode* node) {
    NFZNode* newRoot = node->right;
    NFZNode* temp = newRoot->left;
    
    // Perform rotation
    newRoot->left = node;
    node->right = temp;
    
    // Update heights
    updateHeight(node);
    updateHeight(newRoot);
    
    return newRoot;
}

/**
 * Left-Right Rotation (LR case)
 * 
 * Performs left rotation on left child, then right rotation on node.
 * Fixes left-right imbalance.
 */
NFZNode* NFZDatabase::rotateLeftRight(NFZNode* node) {
    node->left = rotateLeft(node->left);
    return rotateRight(node);
}

/**
 * Right-Left Rotation (RL case)
 * 
 * Performs right rotation on right child, then left rotation on node.
 * Fixes right-left imbalance.
 */
NFZNode* NFZDatabase::rotateRightLeft(NFZNode* node) {
    node->right = rotateRight(node->right);
    return rotateLeft(node);
}

// ============ COMPARISON FUNCTION ============

/**
 * Compare two coordinates (x, y)
 * 
 * Returns:
 *   -1 if (x1, y1) < (x2, y2)
 *    0 if (x1, y1) == (x2, y2)
 *    1 if (x1, y1) > (x2, y2)
 * 
 * Comparison rule: x first, then y if x is equal
 */
int NFZDatabase::compareCoordinates(int x1, int y1, int x2, int y2) const {
    if (x1 < x2) {
        return -1;
    } else if (x1 > x2) {
        return 1;
    } else {
        // x1 == x2, compare y
        if (y1 < y2) {
            return -1;
        } else if (y1 > y2) {
            return 1;
        } else {
            return 0;  // Equal
        }
    }
}

// ============ INSERT OPERATION ============

/**
 * Recursive helper function to insert a coordinate into the AVL tree
 * 
 * Steps:
 * 1. Perform standard BST insertion
 * 2. Update height of current node
 * 3. Get balance factor
 * 4. Rebalance if necessary using rotations
 */
NFZNode* NFZDatabase::insertHelper(NFZNode* node, int x, int y) {
    // Base case: create new node
    if (node == nullptr) {
        return new NFZNode(x, y);
    }
    
    // Compare coordinates to decide insertion direction
    int cmp = compareCoordinates(x, y, node->x, node->y);
    
    if (cmp < 0) {
        // Insert in left subtree
        node->left = insertHelper(node->left, x, y);
    } else if (cmp > 0) {
        // Insert in right subtree
        node->right = insertHelper(node->right, x, y);
    } else {
        // Duplicate coordinate - do nothing (or could return node as-is)
        return node;
    }
    
    // Update height of current node
    updateHeight(node);
    
    // Get balance factor
    int balance = getBalance(node);
    
    // Rebalance if necessary
    
    // Left Left case: node is left-heavy AND left child is left-heavy or balanced
    if (balance > 1 && getBalance(node->left) >= 0) {
        return rotateRight(node);
    }
    
    // Right Right case: node is right-heavy AND right child is right-heavy or balanced
    if (balance < -1 && getBalance(node->right) <= 0) {
        return rotateLeft(node);
    }
    
    // Left Right case: node is left-heavy AND left child is right-heavy
    if (balance > 1 && getBalance(node->left) < 0) {
        return rotateLeftRight(node);
    }
    
    // Right Left case: node is right-heavy AND right child is left-heavy
    if (balance < -1 && getBalance(node->right) > 0) {
        return rotateRightLeft(node);
    }
    
    // No rotation needed
    return node;
}

/**
 * Public insert function
 */
void NFZDatabase::insert(int x, int y) {
    root = insertHelper(root, x, y);
}

// ============ SEARCH OPERATION ============

/**
 * Recursive helper function to search for a coordinate in the AVL tree
 * 
 * Returns true if coordinate (x, y) is found, false otherwise
 */
bool NFZDatabase::searchHelper(NFZNode* node, int x, int y) const {
    // Base case: not found
    if (node == nullptr) {
        return false;
    }
    
    // Compare coordinates
    int cmp = compareCoordinates(x, y, node->x, node->y);
    
    if (cmp == 0) {
        // Found
        return true;
    } else if (cmp < 0) {
        // Search in left subtree
        return searchHelper(node->left, x, y);
    } else {
        // Search in right subtree
        return searchHelper(node->right, x, y);
    }
}

/**
 * Public search function
 * 
 * Returns true if (x, y) is a No-Fly Zone, false otherwise
 */
bool NFZDatabase::isNoFlyZone(int x, int y) const {
    return searchHelper(root, x, y);
}

// ============ MEMORY CLEANUP ============

/**
 * Recursive helper to delete all nodes in the tree
 */
void NFZDatabase::clearHelper(NFZNode* node) {
    if (node == nullptr) {
        return;
    }
    
    // Post-order traversal: delete children first
    clearHelper(node->left);
    clearHelper(node->right);
    
    // Delete current node
    delete node;
}


