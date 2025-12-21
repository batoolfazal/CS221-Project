#ifndef ASTAR_H
#define ASTAR_H

#include "../Mapping/GridMap.h"
#include "heap.h"

struct Cell {
    int parentRow;
    int parentCol;
    double f; // f = g + h
    double g; // cost from source to this cell
    double h; // heuristic (distance to destination)
};

bool isValid(int row, int col, GridMap& map);
double calculateH(int row, int col, int destRow, int destCol);
bool isDestination(int row, int col, int destRow, int destCol);
void tracePath(Cell** cellDetails, int destRow, int destCol);
void aStarSearch(GridMap& map,
                 int ROW, int COL, int srcRow, int srcCol,
                 int destRow, int destCol);

// Embedded BFS (array queue) for connectivity checks.
bool bfsConnectivity(GridMap& map, int startRow, int startCol, int goalRow, int goalCol);

// Embedded DFS (recursive) for reachability/traversal analysis.
void dfsExplore(GridMap& map, int row, int col, bool** visited);
void runDfs(GridMap& map, int startRow, int startCol);

#endif // ASTAR_H
