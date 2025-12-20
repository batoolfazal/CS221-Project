#include "astar.h"
#include "GridMap.h"
#include <iostream>
#include <cmath>
#include <limits>

// ======================= Supporting Functions =======================
bool isValid(int x, int y, GridMap& map, HazardDetector& detector) {
    if (map.isObstacle(x, y)) return false;
    if (map.isNoFlyZone(x, y)) return false;
    if (detector.isHazard(x, y)) return false;
    return true;
}

double calculateH(int row, int col, int destRow, int destCol) {
    return sqrt((row - destRow)*(row - destRow) + (col - destCol)*(col - destCol));
}

bool isDestination(int row, int col, int destRow, int destCol) {
    return (row == destRow && col == destCol);
}

void tracePath(Cell** cellDetails, int destRow, int destCol) {
    // Reconstruct path length first
    int count = 0;
    int row = destRow;
    int col = destCol;
    while (!(cellDetails[row][col].parentRow == row &&
             cellDetails[row][col].parentCol == col)) {
        count++;
        int tempRow = cellDetails[row][col].parentRow;
        int tempCol = cellDetails[row][col].parentCol;
        row = tempRow;
        col = tempCol;
    }
    count++; // include source

    int* pathRows = new int[count];
    int* pathCols = new int[count];
    int idx = count - 1;

    row = destRow;
    col = destCol;
    while (!(cellDetails[row][col].parentRow == row &&
             cellDetails[row][col].parentCol == col)) {
        pathRows[idx] = row;
        pathCols[idx] = col;
        idx--;
        int tempRow = cellDetails[row][col].parentRow;
        int tempCol = cellDetails[row][col].parentCol;
        row = tempRow;
        col = tempCol;
    }
    pathRows[idx] = row;
    pathCols[idx] = col;

    std::cout << "Path: ";
    for (int i = 0; i < count; i++) {
        std::cout << "(" << pathRows[i] << "," << pathCols[i] << ") ";
    }
    std::cout << std::endl;

    delete[] pathRows;
    delete[] pathCols;
}

// ======================= A* Algorithm =======================
void aStarSearch(GridMap& map, HazardDetector& detector,
                 int ROW, int COL, int srcRow, int srcCol,
                 int destRow, int destCol) {

    if (!isValid(srcRow, srcCol, map, detector) ||
        !isValid(destRow, destCol, map, detector)) {
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

    // Open list implemented as a simple array (f-score, row, col)
    struct OpenNode { double f; int row; int col; };
    int openListSize = ROW * COL;
    OpenNode* openList = new OpenNode[openListSize];
    int openCount = 1;
    openList[0].f = 0.0;
    openList[0].row = srcRow;
    openList[0].col = srcCol;

    bool foundDest = false;

    // Directions: N, S, E, W, NE, NW, SE, SW
    int rowDir[] = {-1, 1, 0, 0, -1, -1, 1, 1};
    int colDir[] = {0, 0, 1, -1, 1, -1, 1, -1};
    double moveCost[] = {1.0,1.0,1.0,1.0,1.414,1.414,1.414,1.414};

    while (openCount > 0) {
        // Find cell with minimum f
        int minIndex = 0;
        for (int k = 1; k < openCount; k++) {
            if (openList[k].f < openList[minIndex].f) minIndex = k;
        }

        int i = openList[minIndex].row;
        int j = openList[minIndex].col;

        // Remove from open list
        openCount--;
        for (int k = minIndex; k < openCount; k++) {
            openList[k] = openList[k+1];
        }

        closedList[i][j] = true;

        // Check all 8 neighbors
        for (int dir = 0; dir < 8; dir++) {
            int newRow = i + rowDir[dir];
            int newCol = j + colDir[dir];

            if (isValid(newRow, newCol, map, detector)) {
                if (isDestination(newRow, newCol, destRow, destCol)) {
                    cellDetails[newRow][newCol].parentRow = i;
                    cellDetails[newRow][newCol].parentCol = j;
                    std::cout << "Destination found!" << std::endl;
                    tracePath(cellDetails, destRow, destCol);
                    foundDest = true;

                    // cleanup
                    for (int r = 0; r < ROW; r++) {
                        delete[] closedList[r];
                        delete[] cellDetails[r];
                    }
                    delete[] closedList;
                    delete[] cellDetails;
                    delete[] openList;
                    return;
                }

                if (!closedList[newRow][newCol]) {
                    double gNew = cellDetails[i][j].g + moveCost[dir];
                    double hNew = calculateH(newRow, newCol, destRow, destCol);
                    double fNew = gNew + hNew;

                    if (cellDetails[newRow][newCol].f == std::numeric_limits<double>::max() || cellDetails[newRow][newCol].f > fNew) {
                        if (openCount < openListSize) {
                            openList[openCount].f = fNew;
                            openList[openCount].row = newRow;
                            openList[openCount].col = newCol;
                            openCount++;
                        }
                        cellDetails[newRow][newCol].f = fNew;
                        cellDetails[newRow][newCol].g = gNew;
                        cellDetails[newRow][newCol].h = hNew;
                        cellDetails[newRow][newCol].parentRow = i;
                        cellDetails[newRow][newCol].parentCol = j;
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
    delete[] openList;
}

