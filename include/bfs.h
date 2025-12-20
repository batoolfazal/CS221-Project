#ifndef BFS_H
#define BFS_H

#include "GridMap.h"
#include "Queue.h"

// BFS connectivity check between start and goal on the grid map.
bool bfs(GridMap& map, int startRow, int startCol, int goalRow, int goalCol);

#endif // BFS_H

