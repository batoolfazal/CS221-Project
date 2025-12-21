#ifndef CAMPUS_MAP_H
#define CAMPUS_MAP_H

#include <cstring>

// Cell type constants
const int CELL_FREE_PATH = 0;
const int CELL_BUILDING = 1;
const int CELL_NO_FLY_ZONE = 2;
const int CELL_HAZARD = 3;

struct Region;
class Graph;

// Campus map representation
class CampusMap {
private:
    int rows;
    int cols;
    int** grid;
    Region* regions;
    int regionCount;
    int maxRegions;
    Graph* campusGraph;

    void initializeRegions();
    void fillRegion(int r1, int c1, int r2, int c2, int cellType);
    void buildGraph();

public:
    CampusMap(int r = 80, int c = 80);
    ~CampusMap();

    int getRows() const;
    int getCols() const;

    bool isInBounds(int row, int col) const;
    bool isFree(int row, int col) const;
    bool isObstacle(int row, int col) const;
    bool isNoFlyZone(int row, int col) const;
    bool isHazard(int row, int col) const;
    int getCell(int row, int col) const;

    // Mutators for mapping
    void addWall(int row, int col);
    void removeWall(int row, int col);
    bool isBlocked(int row, int col) const;

    void buildMap();
    bool isPathPossible(int startR, int startC, int goalR, int goalC);
    bool loadFromFile(const char* filename);
    bool saveToFile(const char* filename);
    void visualize() const;
    void printStats() const;
};

#endif // CAMPUS_MAP_H

