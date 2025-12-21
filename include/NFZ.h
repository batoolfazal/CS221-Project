#ifndef NFZ_H
#define NFZ_H

// No-Fly Zone AVL database (no STL).
class NFZDatabase;

// Global pointer used by A* for quick NFZ queries (optional).
extern NFZDatabase* gNFZPtr;

class NFZDatabase {
private:
    struct NFZNode {
        int x;
        int y;
        int height;
        NFZNode* left;
        NFZNode* right;
        NFZNode(int xVal, int yVal) : x(xVal), y(yVal), height(1), left(0), right(0) {}
    };

    NFZNode* root;

    int getHeight(NFZNode* node) const;
    int getBalance(NFZNode* node) const;
    void updateHeight(NFZNode* node);
    int compareCoordinates(int x1, int y1, int x2, int y2) const;

    NFZNode* rotateRight(NFZNode* node);
    NFZNode* rotateLeft(NFZNode* node);
    NFZNode* rotateLeftRight(NFZNode* node);
    NFZNode* rotateRightLeft(NFZNode* node);

    NFZNode* insertHelper(NFZNode* node, int x, int y);
    NFZNode* minValueNode(NFZNode* node);
    NFZNode* removeHelper(NFZNode* node, int x, int y);
    bool searchHelper(NFZNode* node, int x, int y) const;
    void clearHelper(NFZNode* node);

public:
    NFZDatabase();
    ~NFZDatabase();

    void insert(int x, int y);
    void remove(int x, int y);
    bool isNoFlyZone(int x, int y) const;
};

// Public API wrappers
void insertNFZ(int x, int y);
void removeNFZ(int x, int y);
bool isNFZ(int x, int y);

#endif // NFZ_H

