#include "dfs.h"

// Recursively explores all reachable free cells from (row, col)
void dfsExplore(GridMap& map, int row, int col, bool** visited, int rows, int cols) {
    visited[row][col] = true; // Mark current cell as visited

    // Directions: Up, Down, Left, Right
    int dRow[] = {-1, 1, 0, 0};
    int dCol[] = {0, 0, -1, 1};

    // Explore each neighbor recursively
    for (int i = 0; i < 4; i++) {
        int neighborRow = row + dRow[i];
        int neighborCol = col + dCol[i];

        // If neighbor is inside grid, free, and unvisited, explore it
        if (map.isInBounds(neighborRow, neighborCol) &&
            map.isFree(neighborRow, neighborCol) &&
            !visited[neighborRow][neighborCol]) {
            dfsExplore(map, neighborRow, neighborCol, visited, rows, cols); // Recursive call
        }
    }
}

void runDfs(GridMap& map, int startRow, int startCol) {
    int rows = map.getRows();
    int cols = map.getCols();
    if (!map.isInBounds(startRow, startCol) || !map.isFree(startRow, startCol)) {
        return;
    }

    bool** visited = new bool*[rows];
    for (int i = 0; i < rows; i++) {
        visited[i] = new bool[cols];
        for (int j = 0; j < cols; j++) visited[i][j] = false;
    }

    dfsExplore(map, startRow, startCol, visited, rows, cols);

    for (int i = 0; i < rows; i++) delete[] visited[i];
    delete[] visited;
}
