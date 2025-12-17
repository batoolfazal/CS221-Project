#include <iostream>
#include "GridMap.h"

using namespace std;

int main() {
    GridMap map;

    // Load the GIKI map
    if (!map.loadFromFile("data/maps/giki_map.txt")) {
        cout << "Map loading failed\n";
        return 1;
    }

    int startRow = 0, startCol = 0;                    // Start at top-left
    int goalRow = map.getRows() - 1;                  // Goal at bottom-right
    int goalCol = map.getCols() - 1;

    // BFS: Check if goal is reachable
    if (bfs(map, startRow, startCol, goalRow, goalCol)) {
        cout << "BFS: Goal is reachable.\n";
    } else {
        cout << "BFS: Goal is not reachable.\n";
    }

    // DFS: Mark all reachable cells from start
    bool visited[1000][1000] = {false};
    dfs(map, startRow, startCol, visited);
    cout << "DFS traversal completed.\n";

    return 0;
}

