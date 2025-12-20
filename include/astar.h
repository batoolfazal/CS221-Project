#ifndef ASTAR_H
#define ASTAR_H

// Forward declarations of your project classes
class GridMap;
class HazardDetector;

// ======================= Cell Structure =======================
struct Cell {
    int parentRow;
    int parentCol;
    double f; // f = g + h
    double g; // cost from source to this cell
    double h; // heuristic (distance to destination)
};

// ======================= Function Prototypes =======================
bool isValid(int row, int col, GridMap& map, HazardDetector& detector);
double calculateH(int row, int col, int destRow, int destCol);
bool isDestination(int row, int col, int destRow, int destCol);
void tracePath(Cell** cellDetails, int destRow, int destCol);
void aStarSearch(GridMap& map, HazardDetector& detector,
                 int ROW, int COL, int srcRow, int srcCol,
                 int destRow, int destCol);

#endif // ASTAR_H

