#include "GridMap.h"
#include "Queue.h"
#include <iostream>

using namespace std;

// Performs BFS to check if the goal is reachable from the start
bool bfs(GridMap& map, int startRow, int startCol, int goalRow, int goalCol) {
    int rows = map.getRows();
    int cols = map.getCols();
    if (!map.isInBounds(startRow, startCol) || !map.isInBounds(goalRow, goalCol)) {
        return false;
    }

    // Dynamic visited grid sized to the map
    bool** visited = new bool*[rows];
    for (int i = 0; i < rows; i++) {
        visited[i] = new bool[cols];
        for (int j = 0; j < cols; j++) visited[i][j] = false;
    }

    // Directions: Up, Down, Left, Right
    int dRow[] = {-1, 1, 0, 0};
    int dCol[] = {0, 0, -1, 1};

    // Initialize queue with maximum possible size
    Queue queue(rows * cols);

    // Enqueue starting position and mark visited
    queue.enqueue({startRow, startCol});
    visited[startRow][startCol] = true;

    while (!queue.isEmpty()) {
        GridCell current = queue.dequeue(); // Get next cell

        // If we reached the goal, return true
        if (current.row == goalRow && current.col == goalCol) {
            for (int i = 0; i < rows; i++) delete[] visited[i];
            delete[] visited;
            return true;
        }

        // Explore all 4 neighbors
        for (int i = 0; i < 4; i++) {
            int neighborRow = current.row + dRow[i];
            int neighborCol = current.col + dCol[i];

            // Check if neighbor is valid, free, and unvisited
            if (map.isInBounds(neighborRow, neighborCol) &&
                map.isFree(neighborRow, neighborCol) &&
                !visited[neighborRow][neighborCol]) {
                
                visited[neighborRow][neighborCol] = true; // Mark as visited
                queue.enqueue({neighborRow, neighborCol}); // Add to queue
            }
        }
    }

    // If queue empties without reaching goal, return false
    for (int i = 0; i < rows; i++) delete[] visited[i];
    delete[] visited;
    return false;
}
