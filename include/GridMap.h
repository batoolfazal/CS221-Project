#ifndef GRIDMAP_H
#define GRIDMAP_H

// Simple grid-based map without STL containers.
// Cell values: 0 = free, 1 = obstacle/building, 2 = no-fly.

class GridMap {
private:
    int rows;
    int cols;
    int** cells;

    void allocate(int r, int c);
    void release();

public:
    GridMap(int r = 0, int c = 0);
    ~GridMap();

    int getRows() const;
    int getCols() const;

    bool isInBounds(int r, int c) const;
    bool isFree(int r, int c) const;
    bool isObstacle(int r, int c) const;
    bool isNoFlyZone(int r, int c) const;

    void setCell(int r, int c, int value);
};

// Simple hazard detector placeholder; can be extended to use hashing later.
class HazardDetector {
private:
    int rows;
    int cols;
    bool** hazard;

    void allocate(int r, int c);
    void release();

public:
    HazardDetector(int r = 0, int c = 0);
    ~HazardDetector();

    void setHazard(int r, int c, bool present);
    bool isHazard(int r, int c) const;
};

#endif // GRIDMAP_H

