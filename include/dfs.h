#ifndef DFS_H
#define DFS_H

#include "GridMap.h"

// Recursive DFS exploration from a starting cell.
void dfsExplore(GridMap& map, int row, int col, bool** visited, int rows, int cols);

// Convenience wrapper that allocates the visited grid.
void runDfs(GridMap& map, int startRow, int startCol);

#endif // DFS_H

