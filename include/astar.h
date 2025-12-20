#ifndef ASTAR_H
#define ASTAR_H

#include "../Mapping/CampusMap.h"

struct Cell {
    int parentRow;
    int parentCol;
    double f; // f = g + h
    double g; // cost from source to this cell
    double h; // heuristic (distance to destination)
};

bool isValid(int row, int col, CampusMap& map);
double calculateH(int row, int col, int destRow, int destCol);
bool isDestination(int row, int col, int destRow, int destCol);
void tracePath(Cell** cellDetails, int destRow, int destCol);
void aStarSearch(CampusMap& map,
                 int ROW, int COL, int srcRow, int srcCol,
                 int destRow, int destCol);

#endif // ASTAR_H
