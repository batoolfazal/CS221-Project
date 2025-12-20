/**
 * No-Fly Zone (NFZ) Database - AVL Tree Implementation
 * 
 * This module implements a self-balancing AVL tree to store and query
 * No-Fly Zone grid coordinates (row, col) for the drone mission planner.
 * 
 * WHY AVL TREE IS SUITABLE FOR NFZ DATABASE:
 * -------------------------------------------
 * 1. Fast Lookup: O(log n) search time ensures quick safety validation
 *    during path planning, even with thousands of NFZ coordinates.
 * 
 * 2. Self-Balancing: AVL tree maintains height balance automatically,
 *    guaranteeing O(log n) performance in worst-case scenarios (unlike
 *    unbalanced BST which could degrade to O(n)).
 * 
 * 3. Ordered Storage: Coordinates are stored in sorted order (by row, then col),
 *    which is useful for range queries and spatial operations.
 * 
 * 4. Memory Efficient: Only stores coordinates that are actually NFZs,
 *    unlike a 2D array which would require O(rows * cols) space.
 * 
 * 5. Dynamic: Supports efficient insertion of new NFZs without rebuilding
 *    the entire data structure.
 * 
 * TIME COMPLEXITY:
 * ----------------
 * - insert(row, col):     O(log n) - balanced tree ensures logarithmic depth
 * - isNoFlyZone(row, col): O(log n) - binary search through balanced tree
 * - Space:                 O(n) - where n is the number of NFZ coordinates
 * 
 * DSA Course Project - GIKI Institute
 */

#include <iostream>

using namespace std;

// ============ AVL NODE STRUCTURE ============
struct NFZNode {
    int row;              // Grid row coordinate
    int col;              // Grid column coordinate
    int height;           // Height of subtree rooted at this node
    NFZNode* left;        // Left child pointer
    NFZNode* right;       // Right child pointer
    
    NFZNode(int r, int c) 
        : row(r), col(c), height(1), left(NULL), right(NULL) {}
};

// ============ NFZ DATABASE CLASS ============
class NFZDatabase {
private:
    NFZNode* root;
    
    /**
     * Get height of a node (returns 0 for NULL)
     */
    int getHeight(NFZNode* node) const {
        if (node == NULL) {
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
    int getBalance(NFZNode* node) const {
        if (node == NULL) {
            return 0;
        }
        return getHeight(node->left) - getHeight(node->right);
    }
    
    /**
     * Update height of a node based on its children
     */
    void updateHeight(NFZNode* node) {
        if (node == NULL) return;
        
        int leftHeight = getHeight(node->left);
        int rightHeight = getHeight(node->right);
        
        // Use ternary operator instead of STL max()
        node->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
    }
    
    /**
     * Compare two coordinates (row, col)
     * 
     * Returns:
     *   -1 if (row1, col1) < (row2, col2)
     *    0 if (row1, col1) == (row2, col2)
     *    1 if (row1, col1) > (row2, col2)
     * 
     * Comparison rule: row first, then col if row is equal
     */
    int compareCoordinates(int row1, int col1, int row2, int col2) const {
        if (row1 < row2) {
            return -1;
        } else if (row1 > row2) {
            return 1;
        } else {
            // row1 == row2, compare col
            if (col1 < col2) {
                return -1;
            } else if (col1 > col2) {
                return 1;
            } else {
                return 0;  // Equal
            }
        }
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
    NFZNode* rotateRight(NFZNode* node) {
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
    NFZNode* rotateLeft(NFZNode* node) {
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
    NFZNode* rotateLeftRight(NFZNode* node) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    
    /**
     * Right-Left Rotation (RL case)
     * 
     * Performs right rotation on right child, then left rotation on node.
     * Fixes right-left imbalance.
     */
    NFZNode* rotateRightLeft(NFZNode* node) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    
    // ============ RECURSIVE HELPER FUNCTIONS ============
    
    /**
     * Recursive helper function to insert a coordinate into the AVL tree
     * 
     * Steps:
     * 1. Perform standard BST insertion
     * 2. Update height of current node
     * 3. Get balance factor
     * 4. Rebalance if necessary using rotations
     */
    NFZNode* insertHelper(NFZNode* node, int row, int col) {
        // Base case: create new node
        if (node == NULL) {
            return new NFZNode(row, col);
        }
        
        // Compare coordinates to decide insertion direction
        int cmp = compareCoordinates(row, col, node->row, node->col);
        
        if (cmp < 0) {
            // Insert in left subtree
            node->left = insertHelper(node->left, row, col);
        } else if (cmp > 0) {
            // Insert in right subtree
            node->right = insertHelper(node->right, row, col);
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
     * Recursive helper function to search for a coordinate in the AVL tree
     * 
     * Returns true if coordinate (row, col) is found, false otherwise
     */
    bool searchHelper(NFZNode* node, int row, int col) const {
        // Base case: not found
        if (node == NULL) {
            return false;
        }
        
        // Compare coordinates
        int cmp = compareCoordinates(row, col, node->row, node->col);
        
        if (cmp == 0) {
            // Found
            return true;
        } else if (cmp < 0) {
            // Search in left subtree
            return searchHelper(node->left, row, col);
        } else {
            // Search in right subtree
            return searchHelper(node->right, row, col);
        }
    }
    
    /**
     * Recursive helper to delete all nodes in the tree
     */
    void clearHelper(NFZNode* node) {
        if (node == NULL) {
            return;
        }
        
        // Post-order traversal: delete children first
        clearHelper(node->left);
        clearHelper(node->right);
        
        // Delete current node
        delete node;
    }

public:
    /**
     * Constructor
     */
    NFZDatabase() : root(NULL) {}
    
    /**
     * Destructor
     */
    ~NFZDatabase() {
        clearHelper(root);
        root = NULL;
    }
    
    /**
     * Public insert function
     * 
     * Inserts a No-Fly Zone coordinate (row, col) into the AVL tree.
     * The tree automatically balances itself after insertion.
     * 
     * Time Complexity: O(log n)
     */
    void insert(int row, int col) {
        root = insertHelper(root, row, col);
    }
    
    /**
     * Public search function
     * 
     * Returns true if (row, col) is a No-Fly Zone, false otherwise.
     * Uses binary search through the balanced AVL tree.
     * 
     * Time Complexity: O(log n)
     */
    bool isNoFlyZone(int row, int col) const {
        return searchHelper(root, row, col);
    }
};

// ============ DEMONSTRATION MAIN ============
int main() {
    cout << "=== No-Fly Zone (NFZ) Database - AVL Tree Implementation ===" << endl;
    cout << endl;
    
    // Create NFZ Database (AVL Tree)
    NFZDatabase nfzDB;
    
    // Insert No-Fly Zone coordinates
    // Based on known NFZ regions from campus map:
    // - Admin Block: rows 10-14, cols 35-50
    // - Research Labs: rows 29-36, cols 55-70
    // - Server Room: rows 40-45, cols 55-62
    
    cout << "Inserting No-Fly Zone coordinates into AVL tree..." << endl;
    
    // Admin Block NFZ coordinates
    for (int i = 10; i <= 14; i++) {
        for (int j = 35; j <= 50; j++) {
            nfzDB.insert(i, j);
        }
    }
    
    // Research Labs NFZ coordinates
    for (int i = 29; i <= 36; i++) {
        for (int j = 55; j <= 70; j++) {
            nfzDB.insert(i, j);
        }
    }
    
    // Server Room NFZ coordinates
    for (int i = 40; i <= 45; i++) {
        for (int j = 55; j <= 62; j++) {
            nfzDB.insert(i, j);
        }
    }
    
    cout << "NFZ coordinates inserted successfully!" << endl;
    cout << endl;
    
    // Test NFZ lookups using AVL tree
    cout << "=== NFZ Lookup Tests (O(log n) AVL Tree Search) ===" << endl;
    
    int testCoords[][2] = {
        {12, 40},  // Should be in Admin Block NFZ
        {32, 60},  // Should be in Research Labs NFZ
        {42, 58},  // Should be in Server Room NFZ
        {20, 20},  // Should NOT be NFZ (free path)
        {50, 50},  // Should NOT be NFZ (free path)
        {10, 35},  // Should be in Admin Block NFZ (boundary)
        {14, 50},  // Should be in Admin Block NFZ (boundary)
        {29, 55},  // Should be in Research Labs NFZ (boundary)
        {36, 70}   // Should be in Research Labs NFZ (boundary)
    };
    
    const char* descriptions[] = {
        "Admin Block area",
        "Research Labs area",
        "Server Room area",
        "Free path area",
        "Free path area",
        "Admin Block boundary",
        "Admin Block boundary",
        "Research Labs boundary",
        "Research Labs boundary"
    };
    
    for (int i = 0; i < 9; i++) {
        int row = testCoords[i][0];
        int col = testCoords[i][1];
        bool isNFZ = nfzDB.isNoFlyZone(row, col);
        
        cout << "Coordinate (" << row << "," << col << ") - " << descriptions[i] << ": ";
        cout << (isNFZ ? "NO-FLY ZONE" : "SAFE") << endl;
    }
    
    cout << endl;
    
    // Demonstrate dynamic addition
    cout << "=== Dynamic NFZ Addition Test ===" << endl;
    cout << "Adding new NFZ at (5, 5)..." << endl;
    nfzDB.insert(5, 5);
    
    cout << "Checking (5, 5): " 
         << (nfzDB.isNoFlyZone(5, 5) ? "NO-FLY ZONE" : "SAFE") << endl;
    
    cout << "Adding new NFZ at (7, 7)..." << endl;
    nfzDB.insert(7, 7);
    
    cout << "Checking (7, 7): " 
         << (nfzDB.isNoFlyZone(7, 7) ? "NO-FLY ZONE" : "SAFE") << endl;
    
    cout << "Checking (6, 6) (not inserted): " 
         << (nfzDB.isNoFlyZone(6, 6) ? "NO-FLY ZONE" : "SAFE") << endl;
    
    cout << endl;
    cout << "=== AVL Tree Operations Summary ===" << endl;
    cout << "? Insert operation: O(log n) - Self-balancing insertion" << endl;
    cout << "? Lookup operation: O(log n) - Binary search in balanced tree" << endl;
    cout << "? All rotations implemented: LL, RR, LR, RL" << endl;
    cout << "? Comparison rule: row first, then col if row is equal" << endl;
    cout << endl;
    cout << "NFZ Database (AVL Tree) demonstration completed successfully!" << endl;
    
    return 0;
}



