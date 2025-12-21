#include "astar.h"
#include "GridMap.h"
#include <iostream>
#include <cmath>
#include <limits>

// ======================= Supporting Functions =======================
bool isValid(int x, int y, GridMap& map) {
    if (!map.isInBounds(x, y)) return false;
    if (map.isObstacle(x, y)) return false;
    if (map.isNoFlyZone(x, y)) return false;
    if (map.isHazard(x, y)) return false;
    return true;
}

double calculateH(int row, int col, int destRow, int destCol) {
    // Manhattan distance fits grid movement cost model (4/8 neighbor mix still acceptable)
    int dr = row - destRow;
    if (dr < 0) dr = -dr;
    int dc = col - destCol;
    if (dc < 0) dc = -dc;
    return static_cast<double>(dr + dc);
}

bool isDestination(int row, int col, int destRow, int destCol) {
    return (row == destRow && col == destCol);
}

void tracePath(Cell** cellDetails, int destRow, int destCol) {
    // Use an array-backed stack to reverse the path
    int capacity = 0;
    int r = destRow, c = destCol;
    while (!(cellDetails[r][c].parentRow == r && cellDetails[r][c].parentCol == c)) {
        capacity++;
        int tr = cellDetails[r][c].parentRow;
        int tc = cellDetails[r][c].parentCol;
        r = tr; c = tc;
    }
    capacity++; // include source

    PathStack stack;
    stack.rows = new int[capacity];
    stack.cols = new int[capacity];
    stack.top = -1;
    stack.capacity = capacity;

    r = destRow; c = destCol;
    while (!(cellDetails[r][c].parentRow == r && cellDetails[r][c].parentCol == c)) {
        stack.top++;
        stack.rows[stack.top] = r;
        stack.cols[stack.top] = c;
        int tr = cellDetails[r][c].parentRow;
        int tc = cellDetails[r][c].parentCol;
        r = tr; c = tc;
    }
    stack.top++;
    stack.rows[stack.top] = r;
    stack.cols[stack.top] = c;

    std::cout << "Path: ";
    for (int i = stack.top; i >= 0; i--) {
        std::cout << "(" << stack.rows[i] << "," << stack.cols[i] << ") ";
    }
    std::cout << std::endl;

    delete[] stack.rows;
    delete[] stack.cols;
}

// ======================= Helpers =======================
static int toIndex(int r, int c, int cols) {
    return r * cols + c;
}

// ======================= BFS (array-based queue) =======================
bool bfsConnectivity(GridMap& map, int startRow, int startCol, int goalRow, int goalCol) {
    int rows = map.getRows();
    int cols = map.getCols();
    if (!map.isInBounds(startRow, startCol) || !map.isInBounds(goalRow, goalCol)) return false;
    if (!map.isFree(startRow, startCol) || !map.isFree(goalRow, goalCol)) return false;

    bool** visited = new bool*[rows];
    for (int i = 0; i < rows; i++) {
        visited[i] = new bool[cols];
        for (int j = 0; j < cols; j++) visited[i][j] = false;
    }

    int maxSize = rows * cols;
    int* qRow = new int[maxSize];
    int* qCol = new int[maxSize];
    int front = 0, back = 0;

    qRow[back] = startRow; qCol[back] = startCol; back++;
    visited[startRow][startCol] = true;

    int dR[] = {-1, 1, 0, 0};
    int dC[] = {0, 0, -1, 1};

    while (front < back) {
        int r = qRow[front];
        int c = qCol[front];
        front++;

        if (r == goalRow && c == goalCol) {
            for (int i = 0; i < rows; i++) delete[] visited[i];
            delete[] visited;
            delete[] qRow;
            delete[] qCol;
            return true;
        }

        for (int d = 0; d < 4; d++) {
            int nr = r + dR[d];
            int nc = c + dC[d];
            if (map.isInBounds(nr, nc) && map.isFree(nr, nc) && !visited[nr][nc]) {
                visited[nr][nc] = true;
                qRow[back] = nr;
                qCol[back] = nc;
                back++;
            }
        }
    }

    for (int i = 0; i < rows; i++) delete[] visited[i];
    delete[] visited;
    delete[] qRow;
    delete[] qCol;
    return false;
}

// ======================= DFS (recursive) =======================
void dfsExplore(GridMap& map, int row, int col, bool** visited) {
    visited[row][col] = true;
    int dR[] = {-1, 1, 0, 0};
    int dC[] = {0, 0, -1, 1};
    for (int d = 0; d < 4; d++) {
        int nr = row + dR[d];
        int nc = col + dC[d];
        if (map.isInBounds(nr, nc) && map.isFree(nr, nc) && !visited[nr][nc]) {
            dfsExplore(map, nr, nc, visited);
        }
    }
}

void runDfs(GridMap& map, int startRow, int startCol) {
    int rows = map.getRows();
    int cols = map.getCols();
    if (!map.isInBounds(startRow, startCol) || !map.isFree(startRow, startCol)) return;
    bool** visited = new bool*[rows];
    for (int i = 0; i < rows; i++) {
        visited[i] = new bool[cols];
        for (int j = 0; j < cols; j++) visited[i][j] = false;
    }
    dfsExplore(map, startRow, startCol, visited);
    for (int i = 0; i < rows; i++) delete[] visited[i];
    delete[] visited;
}

// ======================= A* Algorithm =======================
void aStarSearch(GridMap& map,
                 int ROW, int COL, int srcRow, int srcCol,
                 int destRow, int destCol) {

    if (!isValid(srcRow, srcCol, map) ||
        !isValid(destRow, destCol, map)) {
        std::cout << "Source or Destination is invalid!" << std::endl;
        return;
    }

    if (isDestination(srcRow, srcCol, destRow, destCol)) {
        std::cout << "Already at Destination!" << std::endl;
        return;
    }

    // Closed list
    bool** closedList = new bool*[ROW];
    for (int i = 0; i < ROW; i++) {
        closedList[i] = new bool[COL];
        for (int j = 0; j < COL; j++) closedList[i][j] = false;
    }

    // Cell details
    Cell** cellDetails = new Cell*[ROW];
    for (int i = 0; i < ROW; i++) {
        cellDetails[i] = new Cell[COL];
        for (int j = 0; j < COL; j++) {
            cellDetails[i][j].f = std::numeric_limits<double>::max();
            cellDetails[i][j].g = std::numeric_limits<double>::max();
            cellDetails[i][j].h = std::numeric_limits<double>::max();
            cellDetails[i][j].parentRow = -1;
            cellDetails[i][j].parentCol = -1;
        }
    }

    // Initialize source cell
    cellDetails[srcRow][srcCol].f = 0.0;
    cellDetails[srcRow][srcCol].g = 0.0;
    cellDetails[srcRow][srcCol].h = 0.0;
    cellDetails[srcRow][srcCol].parentRow = srcRow;
    cellDetails[srcRow][srcCol].parentCol = srcCol;

    // Open list implemented with custom min-heap
    int totalV = ROW * COL;
    MinHeap openHeap(totalV, totalV);
    int srcIndex = toIndex(srcRow, srcCol, COL);
    openHeap.insertKey(srcIndex, srcRow, srcCol, 0.0);

    bool foundDest = false;

    // Directions: N, S, E, W, NE, NW, SE, SW
    int rowDir[] = {-1, 1, 0, 0, -1, -1, 1, 1};
    int colDir[] = {0, 0, 1, -1, 1, -1, 1, -1};
    double moveCost[] = {1.0,1.0,1.0,1.0,1.414,1.414,1.414,1.414};

    while (!openHeap.isEmpty()) {
        HeapNode minNode = openHeap.extractMin();
        int i = minNode.row;
        int j = minNode.col;

        closedList[i][j] = true;

        // Check all 8 neighbors
        for (int dir = 0; dir < 8; dir++) {
            int newRow = i + rowDir[dir];
            int newCol = j + colDir[dir];

            if (isValid(newRow, newCol, map)) {
                int vIndex = toIndex(newRow, newCol, COL);

                if (isDestination(newRow, newCol, destRow, destCol)) {
                    cellDetails[newRow][newCol].parentRow = i;
                    cellDetails[newRow][newCol].parentCol = j;
                    std::cout << "Destination found!" << std::endl;
                    tracePath(cellDetails, destRow, destCol);
                    foundDest = true;

                    for (int r = 0; r < ROW; r++) {
                        delete[] closedList[r];
                        delete[] cellDetails[r];
                    }
                    delete[] closedList;
                    delete[] cellDetails;
                    return;
                }

                if (!closedList[newRow][newCol]) {
                    double gNew = cellDetails[i][j].g + moveCost[dir];
                    double hNew = calculateH(newRow, newCol, destRow, destCol);
                    double fNew = gNew + hNew;

                    if (cellDetails[newRow][newCol].f == std::numeric_limits<double>::max() || cellDetails[newRow][newCol].f > fNew) {
                        cellDetails[newRow][newCol].f = fNew;
                        cellDetails[newRow][newCol].g = gNew;
                        cellDetails[newRow][newCol].h = hNew;
                        cellDetails[newRow][newCol].parentRow = i;
                        cellDetails[newRow][newCol].parentCol = j;

                        if (!openHeap.contains(vIndex)) {
                            openHeap.insertKey(vIndex, newRow, newCol, fNew);
                        } else {
                            openHeap.decreaseKey(vIndex, fNew);
                        }
                    }
                }
            }
        }
    }

    if (!foundDest) std::cout << "Failed to find Destination\n";

    for (int r = 0; r < ROW; r++) {
        delete[] closedList[r];
        delete[] cellDetails[r];
    }
    delete[] closedList;
    delete[] cellDetails;
}

