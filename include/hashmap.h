#ifndef HAZARD_HASHMAP_H
#define HAZARD_HASHMAP_H

#include <cstring>

// Custom hash map for dynamic hazard detection (no STL).
// Key: (row, col) coordinates. Value: short text description.

struct HazardEntry {
    int row;
    int col;
    char desc[64];
    HazardEntry* next;
};

class HazardMap {
private:
    int bucketCount;
    HazardEntry** buckets;

    int hash(int row, int col) const;
    HazardEntry* findEntry(int row, int col) const;

public:
    HazardMap(int buckets = 2048);
    ~HazardMap();

    void clear();
    void addHazard(int row, int col, const char* description);
    bool removeHazard(int row, int col);
    bool isHazard(int row, int col) const;
    const char* getHazardDescription(int row, int col) const; // returns "" if none
};

// Global helper functions (optional convenience)
void addHazard(int row, int col, const char* description);
bool removeHazard(int row, int col);
bool isHazard(int row, int col);
const char* hazardDescription(int row, int col);
void clearHazards();

// Utility: generate N random hazards within bounds [0, rows) x [0, cols)
// descriptionPrefix used to label hazards (e.g., "dyn").
void generateRandomHazards(int count, int rows, int cols, const char* descriptionPrefix);

#endif // HAZARD_HASHMAP_H
